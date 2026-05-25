#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>

#define N 3 // un esempio di un numero di processi figli
char* comandi[N] = {"ls -la", "grep '.c'", "wc -l"}; // array di stringhe(comandi)


int main(){
    int fd[2]; // allocazione dei contenitori dei due file descriptor della pipe
    pid_t* pid = (pid_t*)malloc (N*sizeof(pid_t)); // contenitore del valore di ritorno della fork()

    // Creazione della pipe
    if(pipe(&fd[0]) == -1){
        perror("Errore pipe\n");
        exit(EXIT_FAILURE);
    }

    // Creazione dei figli
    for(int i=0; i<N; i++){
        pid[i] = fork(); // creazione di un figlio, dal padre

        if(pid[i] == -1){
            perror("Errore fork");
            printf("%d\n",i);
            exit(EXIT_FAILURE);
        }

        if(pid[i] == 0){
            // --- PROCESSO FIGLIO i ---

            switch (i) {
                case 0: // // è il primo processo figlio, allora scrive (sulla pipe) soltanto nella pipe
                    // Ridireziona lo std output (1) verso l'estremità di scrittura della pipe (fd[1]) 
                    dup2(fd[1], STDOUT_FILENO); 

                    //Chiudo i descrittori della pipe che non servono più in questo processo
                    close(fd[0]); // Non legge dalla pipe
                    close(fd[1]); // Già duplicato nello std di output

                    // Esegue il primo comando
                    execvp(comandi[0], comandi);
                    // Se execlp fallisce:
                    perror("Errore execlp ls");
                    exit(EXIT_FAILURE);
                break;

                case N-1: // è l'ultimo processo, allora legge nella pipe e basta
                    // Ridireziona lo std input (0) verso l'estremità di lettura della pipe
                    dup2(fd[0], STDIN_FILENO);

                    //Chiudo i descrittori della pipe che non servono più in questo processo
                    close(fd[1]); // non scrive sulla pipe
                    close (fd[0]); // è già duplicato nello std input

                    // esegue l'ultimo comando 
                    execvp(comandi[N-1], comandi);
                break;
                
                default: // un qualsisi processo intermedio, scrive e legge
                    dup2(fd[0], STDIN_FILENO);
                    dup2(fd[1], STDOUT_FILENO);

                    // entrambi i fd sono duplicati, non servono più
                    close(fd[0]);
                    close(fd[1]);

                    //esegue il suo comando
                    execvp(comandi[i], comandi);
                break;
            }
            
        }
    }

    // -- PROCESSO PADRE --

    // chiudo entrami i descrittori : se non chiudo fd[1], un figlio non riceverà mai l'EOF e rimarrà
    // bloccato all'infinito in attesa di input
    close (fd[0]);
    close (fd[1]);

    // Il padre attende la fine di tutti processi per evitare processi "zombie": processi che
    // continuano la loro esecuzione mentre il padre ha terminato la sua
    
    for(int i=0; i<N; i++){
        waitpid(pid[i], NULL, 0);
    }
    return 0;
}