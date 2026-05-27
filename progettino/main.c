#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct {
    int token_len ;
    int value_len ;
} mr_pair_header_t ;

#define DIM 50
char buffer[DIM];

ssize_t readn ( int fd , void * buf , size_t n ){

}
ssize_t writen (int fd , const void * buf , size_t n ) {
    char* tok = strtok(buf, " ");
    while ((tok = strktok(NULL, " ")) != NULL){

    }
}


int main(){
    int fd[2]; // array che conterrà i fd di un processo, necessari per operare sulla pipe
    pid_t pid; // contenitore del risultato della fork

    // 1. Creazione della pipe
    if(pipe(&fd[0]) == -1){
        perror("Errore nella creazione della pipe");
        exit(EXIT_FAILURE);
    }

    // 2. Creazione del figlio (il lettore)
    pid = fork();
    switch (pid) {
        case -1:
            perror("Errore fork");
            exit(EXIT_FAILURE);
            break;
        case 0: // Siamo nel processo figlio, dobbiamo leggere dalla pipe
            dup2(fd[0], STDIN_FILENO); // ridirezione dello std di lettura sul fd di lettura del processo figlio
            close(fd[0]); // è già duplicato, non serve più
            close(fd[1]); // non scrive

            // Lettura sulla pipe 
            readn(STDIN_FILENO, buffer, DIM);
        break;        
        default: // Siamo nel processo padre, dobbiamo scrivere sulla pipe
            close(fd[0]); // non legge

            // scrive nella pipe
            if(writen(fd[1], buffer, DIM) == 0){
                perror("Errore scrittura nella pipe");
                exit(EXIT_FAILURE); // termine del programma
            } else {
                close (fd[1]); // ho terminato di scrivere nella pipe; se non chiudo il fd,
                // il processo figlio non riceverà mai l'EOF ed aspetterà all'infinito 
            }

            wait(pid); // aspetta che il figlio finisca, per evitare uno zombie
            break;
    }

    return 0;
}