#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid;
    int status; /* conterra' lo stato del processo figlio */

    printf("--- INIZIO PROGRAMMA (PID Padre: %d) ---\n", getpid());

    /* Creazione del processo figlio */
    if ( (pid = fork()) == -1 ) {
        perror("main: fork"); 
        exit(EXIT_FAILURE); 
    }

    if (pid > 0) { 
        /* --- CODICE DEL PADRE ---  */
        printf("Padre: Il mio PID è %d. Ho creato il figlio con PID %d.\n", getpid(), pid);
        
        /* Il padre attende 2 secondi (nella slide erano 20) prima di chiamare waitpid  */
        sleep(20); 
        
        /* waitpid attende che il figlio specifico cambi di stato */
        pid = waitpid(pid, &status, 0); 
        
        /* WIFEXITED restituisce true se il figlio è terminato normalmente (con exit o return) */
        if (WIFEXITED(status)) { 
            /* WEXITSTATUS recupera il codice di ritorno passato alla exit() dal figlio */
            printf("Padre: stato %d\n", WEXITSTATUS(status));
        }
    }
    else { 
        /* --- CODICE DEL FIGLIO ---  */
        printf("Figlio: Processo %d, figlio.\n", getpid()); 
        
        /* Il figlio termina con lo stato 20  */
        exit(20); /* termina con stato 20  */
    }

    return 0;
}