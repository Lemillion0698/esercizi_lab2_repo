#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>
#include <stdbool.h>


int main(){
    int pid; // perché è di tipo int e non pid_t ?
    int status; // salverà lo stato del processo figlio
    printf("Inizio\n");
    pid = fork(); // creazione di un processo figlio 

    switch (pid)
    {
    case -1:
        perror("Errore nella fork");
        exit (1);
        break;
    case 0: // figlio
        printf("Siamo nel processo figlio con PID = %d\n", getpid());
        FILE* f = fopen("output.txt", "w"); // apro in scrittura il file output.txt
        execl("output.txt", "ls -la"); // eseguo il comando "ls -la" sul file
        printf("Processo %d, figlio.\n", getpid());
        exit(10);
    
    default: // padre
        sleep(5);
        if ((waitpid(pid, status, WNOHANG)) != -1){
            if ((WIFEXITED(status)) == true) {
                /*Il figlio terminato con exit o return*/
                printf("stato %d\n", WEXITSTATUS(status));
            }
        }
        break;
    }
    return 0;
}