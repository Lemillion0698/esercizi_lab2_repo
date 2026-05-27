#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h> // Necessario per intercettare EINTR

#define DIM 50

/* * Legge esattamente 'n' byte da un file descriptor.
 * Gestisce letture parziali e interruzioni di sistema (EINTR).
 */
ssize_t readn(int fd, void *buf, size_t n) {
    size_t nleft = n;          // 1. Byte rimasti da leggere
    ssize_t nread;             // 2. Byte letti nell'ultima singola read()
    char *ptr = buf;           // 3. Puntatore alla posizione corrente nel buffer

    while (nleft > 0) {        // 4. Ciclo finché mancano byte da leggere
        if ((nread = read(fd, ptr, nleft)) < 0) { // 5. Chiamata di sistema read()
            if (errno == EINTR) {
                nread = 0;     // 6. Chiamata interrotta da segnale: resetta e riprova
            } else {
                return -1;     // 7. Errore grave irrecuperabile
            }
        } else if (nread == 0) {
            break;             // 8. Raggiunto l'End-of-File (EOF)
        }
        
        nleft -= nread;        // 9. Aggiorna il conteggio dei byte mancanti
        ptr += nread;          // 10. Sposta in avanti il puntatore nel buffer
    }
    return (n - nleft);        // 11. Restituisce il numero totale di byte letti
}

/* * Scrive esattamente 'n' byte su un file descriptor.
 * Gestisce scritture parziali e interruzioni di sistema (EINTR).
 */
ssize_t writen(int fd, const void *buf, size_t n) { // ssize_t perché in caso di errore, può restituire un numero negativo
    size_t nleft = n; // (1) perché nleft non ha lo stesso tipo di writen, cioè ssize_t ? per essere compatibile col tipo di n
    ssize_t nwritten;
    const char *ptr = buf;

    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) { //(2) la write deve prelevare dal buffer ogni volta, una parola e mandarlo nella pipe ?
            /*Assolutamente NO ! Questo è un concetto cruciale. La write() non sa cosa sia una "parola", non cerca spazi vuoti, non legge il 
            testo. La pipe è un tubo di byte puri. Se passi alla write il valore nleft = 30, la write prende semplicemente i successivi 30 byte 
            contigui in memoria a partire dall'indirizzo indicato da ptr e prova a scaricarli nella pipe, che si tratti di una frase, di mezza 
            parola o di dati binari.*/
            if (nwritten < 0 && errno == EINTR) { //(3) cos'è EINTR ? c'è qualche alternativa ?
                /*EINTR sta per Error: Interrupted system call. Significa che mentre il processo era bloccato in attesa che la write() 
                completasse l'operazione, il sistema operativo ha inviato un segnale software al processo (ad esempio SIGINT se premi Ctrl+C, o 
                un timer SIGALRM). Il sistema interrompe la write() e imposta errno = EINTR*/
                nwritten = 0; // Chiamata interrotta da un segnale, ripeti il ciclo
            } else {
                return -1;    // Errore grave di scrittura
            }
        }
        nleft -= nwritten; // (4) alla prossimo iterazione scrivo "nfleft - nwritten" caratteri ?
    /* Non esattamente, nei sistemi operativi, l'I/O a basso livello ragiona ed esegue calcoli solo ed esclusivamente in byte, non in caratteri. 
        Es: Se inizialmente dovevi scrivere 50 byte (nleft = 50) e la write ne scrive solo 20 (nwritten = 20), l'operazione "nleft -= nwritten" 
        aggiorna il valore a 30 byte rimasti. Alla prossima iterazione sposterai il puntatore in avanti di 20 byte e chiederai alla write di 
        scriverne i successivi 30.*/
        ptr += nwritten; // (5) faccio andare avanti il puntatore ptr, di nwritten locazioni nel buffer ?
       /*Esatto. Questa si chiama aritmetica dei puntatori. Poiché ptr è un puntatore a char (e ogni char occupa esattamente 1 byte), fare 
      "ptr += nwritten" sposta l'indirizzo di memoria in avanti di esattamente nwritten byte. In questo modo, la write() successiva inizierà 
      a leggere il buffer dal punto esatto in cui si era interrotta la scrittura precedente, evitando di inviare dati duplicati o sovrapposti.*/
    }
    return n; // Successo: tutti gli n byte sono stati scritti
}

int main() {
    int fd[2]; 
    pid_t pid; 
    char buffer[DIM]; // Spostato localmente nel main (buona pratica)

    // 1. Creazione della pipe
    if (pipe(fd) == -1) { // Nota: passare "fd" equivale a &fd[0]
        perror("Errore nella creazione della pipe");
        exit(EXIT_FAILURE);
    }

    // 2. Creazione del figlio (il lettore)
    pid = fork();
    switch (pid) {
        case -1:
            perror("Errore fork");
            exit(EXIT_FAILURE);
            break;

        case 0: // PROCESSO FIGLIO: Legge dalla pipe
            dup2(fd[0], STDIN_FILENO); 
            close(fd[0]); // Chiude il descrittore originale duplicato
            close(fd[1]); // Chiude il lato di scrittura non utilizzato dal figlio

            // Pulizia preventiva del buffer di ricezione
            memset(buffer, 0, DIM);

            // Lettura robusta sulla pipe
            if (readn(STDIN_FILENO, buffer, DIM) < 0) {
                perror("Errore lettura nella pipe");
                exit(EXIT_FAILURE);
            }

            printf("[FIGLIO] Ho letto dalla pipe: %s\n", buffer);
            break;        

        default: // PROCESSO PADRE: Scrive nella pipe
            close(fd[0]); // Chiude il lato di lettura non utilizzato dal padre

            // Inizializziamo il buffer con dati reali da spedire
            strncpy(buffer, "Messaggio di test per la pipe!", DIM - 1);
            buffer[DIM - 1] = '\0'; // Garantisce la terminazione della stringa

            // Scrittura robusta nella pipe (Verifica se < 0 per l'errore)
            if (writen(fd[1], buffer, DIM) < 0) {
                perror("Errore scrittura nella pipe");
                close(fd[1]);
                exit(EXIT_FAILURE); 
            }
            
            // Chiude il fd di scrittura: notifica l'EOF al figlio
            close(fd[1]); 

            // Attesa deterministica e corretta del figlio specifico
            if (waitpid(pid, NULL, 0) == -1) {
                perror("Errore nella waitpid");
                exit(EXIT_FAILURE);
            }
            printf("[PADRE] Il figlio è terminato correttamente.\n");
            break;
    }

    return 0;
}