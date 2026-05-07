#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid;
    int status;

    printf("Inizio del programma (PID Padre: %d)\n", getpid());
    
    /* Creazione del processo figlio */
    pid = fork(); 

    switch (pid) {
        case -1:
            /* Errore nella fork */
            perror("Errore nella fork");
            exit(EXIT_FAILURE); 
            //break;
        case 0: 
            /* --- CODICE DEL FIGLIO --- */
            printf("Figlio (PID: %d): Invoco la shell per eseguire il comando con redirezione.\n", getpid());
            
            /* * execl lancia /bin/sh.
             * "sh" è il nome convenzionale (argv[0]).
             * "-c" dice alla shell di eseguire la stringa che segue.
             * "ls -la > output.txt" è l'intero comando, redirezione inclusa!
             */
            //execlp("sh", "sh", "-c", "ls -la > output.txt", (char *)NULL); // se togliessi la -c ? sh: 0: cannot open ls -la > output.txt: No such file
            char *argomenti[] = {"sh", "-c", "ls -la > output.txt", NULL};
            execv("/bin/sh", argomenti);
            /* Questa riga viene raggiunta SOLO se /bin/sh non esiste o execl fallisce */
            perror("Errore nella execl");
            exit(EXIT_FAILURE);
            //break;
        default: 
            /* --- CODICE DEL PADRE --- */
            
            /* Il padre attende la fine del figlio */
            if (waitpid(pid, &status, 0) == -1) {
                /* Mettere 0 significa richiedere il comportamento di base (nessuna opzione speciale). Il comportamento di base impone che il processo padre si blocchi 
                (si sospenda) e attenda indefinitamente che il figlio abbia cambiato stato (cioè che sia terminato). */
                perror("Errore nella waitpid");
                exit(EXIT_FAILURE);
            }
            
            if (WIFEXITED(status)) {
                printf("Padre: Il figlio è terminato correttamente. Exit status: %d\n", WEXITSTATUS(status));
                printf("Padre: Controlla il file 'output.txt'!\n");
            } else {
                printf("Padre: Il figlio è terminato in modo anomalo.\n");
            }
           // break;
    }

    return 0;
}