#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pfd[2]; // Descrittori della pipe
    pid_t pid;
    char msg[] = "Messaggio dal Padre al Figlio tramite Pipe!";
    char buffer[100];

    // 1. Creazione della pipe (prima della fork!)
    if (pipe(pfd) == -1) {
        perror("Errore nella creazione della pipe");
        exit(EXIT_FAILURE);
    }

    // 2. Creazione del processo figlio
    pid = fork();

    if (pid < 0) {
        perror("Errore nella fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { 
        /* --- PROCESSO PADRE (Scrittore) --- */
        
        // Regola d'oro: chiudere il lato che non si usa
        close(pfd[0]); // Il padre non deve leggere

        printf("[PADRE]: Scrittura del messaggio nella pipe...\n");
        // Scriviamo includendo il carattere terminatore '\0' per facilitare la stampa nel figlio
        write(pfd[1], msg, strlen(msg) + 1);

        close(pfd[1]); // Abbiamo finito di scrivere, chiudiamo il descrittore
        
        wait(NULL);    // Aspettiamo che il figlio finisca per pulire lo stato
        printf("[PADRE]: Figlio terminato. Esco.\n");
    } 
    else { 
        /* --- PROCESSO FIGLIO (Lettore) --- */
        
        // Regola d'oro: chiudere il lato che non si usa
        close(pfd[1]); // Il figlio non deve scrivere

        printf("[FIGLIO]: In attesa di ricevere dati...\n");
        
        // La read() è bloccante: il figlio aspetta finché non c'è qualcosa da leggere
        int bytes_read = read(pfd[0], buffer, sizeof(buffer));

        if (bytes_read > 0) {
            printf("[FIGLIO]: Ricevuto: \"%s\"\n", buffer);
        }

        close(pfd[0]); // Lettura completata, chiudiamo il descrittore
        exit(EXIT_SUCCESS);
    }

    return 0;
}