#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pid_t pid1, pid2;

    // 1. Creazione della pipe
    if (pipe(fd) == -1) {
        perror("Errore nella creazione della pipe");
        exit(EXIT_FAILURE);
    }

    // 2. Creazione del Primo Figlio (esegue: ls -la)
    pid1 = fork();
    if (pid1 == -1) {
        perror("Errore fork 1");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // --- PROCESSO FIGLIO 1 ---
        
        // Ridireziona lo Standard Output (1) verso l'estremità di scrittura della pipe (fd[1])
        dup2(fd[1], STDOUT_FILENO); // L'indice 1 (STDOUT_FILENO), che prima 
        // puntava allo schermo, viene sovrascritto e adesso punta alla stessa 
        // risorsa a cui punta fd[1], ovvero il canale di scrittura della pipe.
        // Ovvero, la system call dup2 duplica il riferimento memorizzato all'indice fd[1] della tabella dei descrittori di file (locale al processo) scrivendolo all'indice 1 (STDOUT_FILENO) della medesima tabella. In questo modo, entrambi i descrittori condividono ed indirizzano la stessa entry della tabella dei file aperti globale di sistema, la quale mantiene lo stato di interazione (offset e flag di I/O) con il canale di scrittura del file speciale di tipo pipe allocato nel kernel.

        // Chiude i descrittori della pipe che non servono più in questo processo
        close(fd[0]); // Non legge dalla pipe
        close(fd[1]); // Ormai duplicato in STDOUT_FILENO

        // Esegue il comando "ls -la"
        execlp("/usr/bin/ls", "ls", "-la", NULL);
        
        // Se execlp fallisce:
        perror("Errore execlp ls");
        exit(EXIT_FAILURE);
    }

    // 3. Creazione del Secondo Figlio (esegue: grep ".c")
    pid2 = fork();
    if (pid2 == -1) {
        perror("Errore fork 2");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // --- PROCESSO FIGLIO 2 ---
        
        // Ridireziona lo Standard Input (0) affinché legga dall'estremità di lettura della pipe (fd[0])
        dup2(fd[0], STDIN_FILENO);

        // Apre (o crea) il file output.txt in modalità scrittura
        // O_TRUNC svuota il file se esiste già, 0644 imposta i permessi standard di lettura/scrittura
        int file_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (file_fd == -1) {
            perror("Errore apertura file output.txt");
            exit(EXIT_FAILURE);
        }

        // Ridireziona lo Standard Output (1) verso il file appena aperto
        dup2(file_fd, STDOUT_FILENO);

        // Chiude tutti i descrittori ridondanti o inutilizzati
        close(fd[0]);   // Ormai duplicato in STDIN_FILENO
        close(fd[1]);   // Non scrive nella pipe
        close(file_fd); // Ormai duplicato in STDOUT_FILENO

        // Esegue il comando "grep .c"
        execlp("/usr/bin/grep", "grep", ".c", NULL);
        
        
        // Se execlp fallisce:
        perror("Errore execlp grep");
        exit(EXIT_FAILURE);
    }

    // --- PROCESSO PADRE ---
    
    // REGOLA AUREA: Il padre deve chiudere entrambi i lati della pipe.
    // Se non chiude fd[1], il Figlio 2 (grep) non riceverà mai l'EOF (End Of File)
    // e rimarrà bloccato all'infinito in attesa di input.
    close(fd[0]);
    close(fd[1]);

    // Il padre attende la fine di entrambi i processi figli per evitare processi "zombie"
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Esecuzione completata con successo. Controlla il file 'output.txt'\n");

    return 0;
}