#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define FIFO_SERVER "/tmp/fifo_server"
#define MAX_BUF 256

int main() {
    pid_t pid;

    // 1. Il Server crea la FIFO pubblica (Well-Known FIFO)
    if (mkfifo(FIFO_SERVER, 0666) == -1) {
        if (errno != EEXIST) {
            perror("[SERVER] Errore nella mkfifo");
            return 1;
        }
    }

    pid = fork();
    if (pid < 0) {
        perror("Errore fork");
        return 1;
    }

    if (pid > 0) {
        /* =================================================================
           --- PROCESSO PADRE: IL SERVER (Frammento corrispondente alla slide 42) ---
           ================================================================= */
        int fd_server;
        char buf[MAX_BUF];
        int n_letti;

        printf("[SERVER] In ascolto sulla FIFO: %s\n", FIFO_SERVER);
        
        // Il server si blocca qui finché il primo client non apre la FIFO in scrittura
        fd_server = open(FIFO_SERVER, O_RDONLY);
        if (fd_server == -1) {
            perror("[SERVER] Errore apertura FIFO");
            return 1;
        }

        printf("[SERVER] Client connesso! Inizio ricezione dati...\n\n");

        /* --- IL CUORE DELLA SLIDE 42 ---
           Il ciclo legge continuamente dalla FIFO. 
           La read() restituisce:
           - > 0: byte letti con successo
           - == 0: EOF (End of File), significa che TUTTI i client hanno chiuso la FIFO
        */
        while ((n_letti = read(fd_server, buf, MAX_BUF - 1)) > 0) {
            buf[n_letti] = '\0'; // Assicura la terminazione della stringa
            
            // Simulazione dell'elaborazione della richiesta del client
            printf("[SERVER RICEVE]: %s", buf);
        }

        if (n_letti == -1) {
            perror("[SERVER] Errore durante la read");
        }

        printf("\n[SERVER] Tutti i client si sono disconnessi (Rilevato EOF).\n");
        
        // Pulizia
        close(fd_server);
        unlink(FIFO_SERVER);
        wait(NULL); 
        printf("[SERVER] FIFO rimossa. Server spento.\n");
    } 
    else {
        /* =================================================================
           --- PROCESSO FIGLIO: IL CLIENT (Simulatore di richieste) ---
           ================================================================= */
        int fd_client;
        
        // Il client attende un secondo per dare tempo al server di mettersi in ascolto
        sleep(1); 

        printf("[CLIENT] Tento di connettermi al server...\n");
        fd_client = open(FIFO_SERVER, O_WRONLY);
        if (fd_client == -1) {
            perror("[CLIENT] Errore apertura FIFO");
            exit(1);
        }

        // Il client invia una serie di messaggi distanziati nel tempo
        char msg1[] = "Richiesta 1: Login utente\n";
        printf("[CLIENT] Invio: %s", msg1);
        write(fd_client, msg1, strlen(msg1));
        sleep(1);

        char msg2[] = "Richiesta 2: Download file_dati.txt\n";
        printf("[CLIENT] Invio: %s", msg2);
        write(fd_client, msg2, strlen(msg2));
        sleep(1);

        char msg3[] = "Richiesta 3: Logout\n";
        printf("[CLIENT] Invio: %s", msg3);
        write(fd_client, msg3, strlen(msg3));

        printf("[CLIENT] Ho terminato le richieste. Chiudo la connessione.\n");
        close(fd_client);
        exit(0);
    }

    return 0;
}