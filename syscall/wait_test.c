#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid;
    int status;

    printf("--- INIZIO PROGRAMMA (PID Padre: %d) ---\n", getpid());

    /* Creazione del processo figlio */
    pid = fork();

    if (pid == -1) {
        /* Errore nella creazione del figlio */
        perror("Errore durante la fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        /* --- CODICE DEL FIGLIO --- */
        printf("Figlio: Il mio PID è %d. Eseguirò un'operazione per 2 secondi...\n", getpid());
        
        /* Simulazione di un'operazione che richiede tempo */
        sleep(2);
        
        printf("Figlio: Operazione conclusa. Termino con codice di stato 42.\n");
        exit(42); /* Il figlio termina e restituisce il valore 42 */
    } 
    else {
        /* --- CODICE DEL PADRE --- */
        printf("Padre: Ho creato il figlio con PID %d. Ora mi metto in attesa usando wait()...\n", pid);
        
        /* La chiamata wait() sospende il padre finché un qualsiasi figlio non termina.
         * Restituisce il PID del figlio terminato e salva lo stato nella variabile 'status'.
         */
        pid_t terminated_pid = wait(&status);
        
        if (terminated_pid == -1) {
            perror("Errore durante la wait");
            exit(EXIT_FAILURE);
        }
        
        /* Controlla se il figlio è terminato in modo volontario (es. tramite exit o return) */
        if (WIFEXITED(status)) {
            /* Estrae il codice di ritorno effettivo passato alla exit() */
            int exit_code = WEXITSTATUS(status);
            printf("Padre: Il figlio con PID %d è terminato normalmente con codice: %d\n", terminated_pid, exit_code);
        } else {
            printf("Padre: Il figlio con PID %d è terminato in modo anomalo.\n", terminated_pid);
        }
    }

    printf("--- FINE PROGRAMMA ---\n");
    return 0;
}