#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

#define N 3 

int main() {
    // Array di stringhe dei comandi (come nel tuo esempio)
    char* comandi[N] = {"ls -la", "grep .c", "wc -l"}; 
    
    // REGOLA DELLA SLIDE: Se ci sono N comandi, servono N-1 pipe
    int pfd[N - 1][2]; 
    pid_t pid[N];

    // 1. Creazione di tutte le pipe necessarie
    for (int i = 0; i < N - 1; i++) {
        if (pipe(pfd[i]) == -1) {
            perror("Errore creazione pipe");
            exit(EXIT_FAILURE);
        }
    }

    // 2. Ciclo di generazione dei processi figli
    for (int i = 0; i < N; i++) {
        pid[i] = fork();

        if (pid[i] == -1) {
            perror("Errore fork");
            exit(EXIT_FAILURE);
        }

        if (pid[i] == 0) {
            /* --- PROCESSO FIGLIO i (LOGICA GENERALE) --- */

            // Se non è il primo comando, legge dalla pipe precedente
            if (i > 0) {
                dup2(pfd[i - 1][0], STDIN_FILENO);
            }

            // Se non è l'ultimo comando, scrive nella pipe successiva
            if (i < N - 1) {
                dup2(pfd[i][1], STDOUT_FILENO);
            }

            // CHIUSURA DI TUTTI I DESCRITTORI DELLE PIPE NEL FIGLIO
            // Dopo dup2, i canali originali non servono più a nessuno dei figli!
            for (int j = 0; j < N - 1; j++) {
                close(pfd[j][0]);
                close(pfd[j][1]);
            }

            /* --- TOKENIZZAZIONE DINAMICA DEL COMANDO ---
               Trasformiamo ad esempio "ls -la" in {"ls", "-la", NULL} */
            char *args[10];
            int arg_idx = 0;
            
            // Usiamo strtok per spezzare la stringa sugli spazi
            char *token = strtok(comandi[i], " ");
            while (token != NULL) {
                args[arg_idx++] = token;
                token = strtok(NULL, " ");
            }
            args[arg_idx] = NULL; // FONDAMENTALE per execvp

            // Esecuzione del comando reale tokenizzato
            execvp(args[0], args);
            
            // Se arriviamo qui, la execvp ha fallito
            perror("Errore execvp");
            exit(EXIT_FAILURE);
        }
    }

    /* --- PROCESSO PADRE --- */
    
    // REGOLA D'ORO: Il padre deve chiudere TUTTE le sue copie delle pipe,
    // altrimenti i figli rimarranno bloccati all'infinito (Deadlock).
    for (int i = 0; i < N - 1; i++) {
        close(pfd[i][0]);
        close(pfd[i][1]);
    }

    // Attesa sincronizzata di tutti i figli per evitare processi zombie
    for (int i = 0; i < N; i++) {
        waitpid(pid[i], NULL, 0);
    }

    return 0;
}