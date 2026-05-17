#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pfd[2];
    pid_t pid1, pid2;

    // 1. Creazione della pipe
    if (pipe(pfd) == -1) {
        perror("Errore creazione pipe");
        return 1;
    }

    // 2. FORK DEL PRIMO FIGLIO (Esegue "who")
    pid1 = fork();
    if (pid1 < 0) {
        perror("Errore fork 1");
        return 1;
    }

    if (pid1 == 0) {
        /* --- PRIMO FIGLIO (Scrittore) --- */
        // Reindirizza lo stdout sulla pipe
        dup2(pfd[1], STDOUT_FILENO);

        // Chiusura dei descrittori ridondanti
        close(pfd[0]);
        close(pfd[1]);

        // Esegue il comando "who" 
        // (restituisce la lista degli utenti connessi)
        execlp("who", "who", NULL);
        
        perror("Errore execlp who");
        exit(1);
    }

    // 3. FORK DEL SECONDO FIGLIO (Esegue "wc -l")
    pid2 = fork();
    if (pid2 < 0) {
        perror("Errore fork 2");
        return 1;
    }

    if (pid2 == 0) {
        /* --- SECONDO FIGLIO (Lettore) --- */
        // Reindirizza lo stdin dalla pipe
        dup2(pfd[0], STDIN_FILENO);

        // Chiusura dei descrittori ridondanti
        close(pfd[0]);
        close(pfd[1]);

        // Esegue il comando "wc -l" 
        // (conta le righe ricevute in input)
        execlp("wc", "wc", "-l", NULL);
        
        perror("Errore execlp wc");
        exit(1);
    }

    /* --- PROCESSO PADRE --- */
    // Chiusura tassativa per evitare il blocco (deadlock) di wc
    close(pfd[0]);
    close(pfd[1]);

    // Attesa dei due figli
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("[PADRE]: Comando 'who | wc -l' eseguito correttamente.\n");

    return 0;
}