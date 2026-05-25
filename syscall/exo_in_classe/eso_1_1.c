#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

int main(){

    pid_t pid;
    //int* status; // SBAGLIATO ! è un puntatore "selvaggio": punta ad un area di memoria casuale 
    int status;
    int fd; // contenitore del file descriptor restituito dalla open()
    
    pid = fork();

    switch (pid){
        case -1:
            perror("Errore fork");
            exit(1);
            break;
        case 0:
        // Il PROCESSO FIGLIO esegue il comando "ls -la" e ridirige l'output in un file
        // NOTA: l'apertura del file è incompleta: se "output1.txt" NON ESISTE già nella cartella, la open() fallirà resituendo -1    
        //fd = open("output1.txt", O_WRONLY); // apro il file solo in scrittura
            if((fd = open ("output1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1){
                // O_CREAT : indico al SO di creare il file se non esiste
                // O_TRUNC : se esiste già, svuotalo
                perror("output1.txt in apertura");
                exit(EXIT_FAILURE);
            }
            
            // Devo ora ridirigere l'output del comando "ls -la" nel file output1.txt 
            dup2(fd, STDOUT_FILENO); // ho duplicato il file descriptor di output1.txt sullo std output

            close(fd); // chiudo fd : non serve più, è duplicato nello std output

            // Eseguo il comando "ls -la"
            //execlp("/bin/echo", "ls", "-", "la", (char*)NULL);
            // in questo modo, il processo figlio esegue il comando echo invece di ls
            // "-la" è in un' unica opzione, non va spezzata
            execlp("/bin/ls", "ls", "-la", (char*)NULL);

            // Nel caso in cui la execlp fallisce
            perror("Errore nella execlp");
            exit(1);
            break;
        default: 
            // Il PROCESSO PADRE attende il figlio
            if(wait(&status) == -1){
                perror("Errore nella wait\n");
                exit(1);
            }
            break;
        
        }
    return 0;
}