#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h> // Necessario per intercettare EINTR
#include <ctype.h>  // Necessario per toupper()

#define DIM 100

ssize_t readn(int fd, void* buf, size_t n){

}

ssize_t writen(int fd, void* buf, size_t n){

}

void trasforma_in_maiuscolo(char* s) {
    int i = 0;
    while (s[i] != '\0') {
        // Assegniamo il valore di ritorno e facciamo il cast a (unsigned char) 
        // per garantire la massima portabilità con caratteri estesi.
        s[i] = (char)toupper((unsigned char)s[i]);
        i++;
    }
}

int main(){

    char* str = "Buongiorno mi chiamo Mbayanga"; // Stringa da elaborare
    char buffer[DIM]; // buffer di dimensione DIM

    int fd_A[2]; // descrittori per la pipe A
    int fd_B[2]; // descrittori per la pipe B
    int fd_C[2]; // descrittori per la pipe C
    
    // Creazione delle 3 pipe
    if(pipe(fd_A) || pipe(fd_B) || pipe(fd_C)){
        perror("Errore nella creazione di una pipe");
        exit(EXIT_FAILURE);
    }

    // Creazione de processo e gestione del processo Mapper 
    pid_t pid_M = fork();
    switch (pid_M){
        case -1:
            perror("Errore processo Mapper");
            exit (EXIT_FAILURE);
            break;
        case 0: // SIAMO NEL PROCESSO MAPPER
            // PRIMA PARTE LETTURA DALLA PIPE A E ELABORAZIONE DELLA STRINGA 

            dup2(fd_A[0], STDIN_FILENO); // duplico il descrittore di lettura della pipe A sullo standard di input del processo Mapper
            // in altre parole, quello che viene letto dalla pipe A sarà letto anche dal processo Mapper
            close(fd_A[0]); // non serve più, è già duplicato nello standard input del processo principale

             // Pulizia preventiva del buffer di ricezione
            memset(buffer, 0, DIM);

            if (readn(STDIN_FILENO, buffer, DIM) < 0) {
                perror("Errore lettura dalla pipe A");
                exit(EXIT_FAILURE);
            }
            printf("[PROCESSO FIGLIO MAPPER] Ho letto dalla pipe A: %s\n", buffer);

            // TRASFORMA LA STRINGA PRESENTE SUL BUFFER
            trasforma_in_maiuscolo(buffer);
            printf("[PROCESSO FIGLIO MAPPER] Ho trasformato in maiuscolo la stringa: %s\n", buffer);

            // SECONDA PARTE: SCRITTURA SULLA PIPE B

            dup2(fd_B[1], STDOUT_FILENO); // duplico il descrittore di scrittura della pipe B sullo std di output del processo B,
            // cioè quello che va scritto sulla pipe B, andrà anche sullo standard di output del processo Mapper
            close(fd_B[1]); // duplicato, non serve più
            if(writen(STDOUT_FILENO, buffer, DIM) < 0){
                perror("Errore scrittura sulla pipe B");
                exit(EXIT_FAILURE);
            }
            printf("[PROCESSO FIGLIO MAPPER] Ho scritto sulla pipe B: %s\n", buffer);
            close(STDOUT_FILENO); // se non chiudo, il processo Reducer non riceverrà mai l'EOF e asppetterà all'infinito
        default: // SIAMO NEL PROCESSO PADRE
            // Srive sulla pipe A
            strncpy(str, buffer, DIM-1); // inizializzo il buffer
            if(writen(fd_A[1], buffer, DIM)){ // scrivo sulla pipe A
                perror("Errore scrittura sulla pipe A");
                exit(EXIT_FAILURE);
            }
            close(fd_A[1]); // se non chiudo, il processo Mapper non riceverrà mai l'EOF e asppetterà all'infinito

            //Aspetta la terminazione dei processi Mapper
            waitpid(pid_M, NULL, 0); // per evitare che Mapper sia zombie
            break;
    }

    // Creazione e gestione del processo Reducer
    pid_t pid_R = fork(); 
    switch (pid_R){
        case -1:
            perror("Errore processo Reducer");
            exit (EXIT_FAILURE);
            break;
        case 0: // Sono nel processo Reducer
            
        default:
            // Legge dalla pipe C e stampa a video il messaggio finale
                if(readn(fd_C[0], buffer, DIM)<0){
                    perror("Errore lettura Processo principale");
                    exit(EXIT_FAILURE);
                }
                printf("Il messaggio finale è: %s", buffer);

                //Aspetta la terminazione dei processi Mapper
                waitpid(pid_R, NULL, 0); // per evitare che Mapper sia zombie
            break;
        }

    return 0;
}