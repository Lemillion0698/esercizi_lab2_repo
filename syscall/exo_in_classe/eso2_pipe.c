#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DIM 100

int main(){
    // Regola d'oro : si crea la pipe, prima di qualsisi fork, per evitare di avere situazioni di deadlock
    int pfd[2];
    char buffer [DIM];
    int result_pipe = pipe(&pfd[0]);

    if(result_pipe == -1){
        perror("Errore pipe");
        return 1;
    }

    int pid = fork (); // creazione del primo figlio

    if(pid == 0){ 
        // Siamo nel primo figlio, lui scrive
        close(pfd[0]);
        execp("\bin\ls -la", "ls -la", (char*) NULL); // esecuzione del comando "ls -la"
        write(pfd[1], buffer, strlen(buffer));
    }
    int pid2 = fork(); // creazione del secondo figlio

    if(pid2 == 0){
        // Siamo nel secondo figlio
        close(pfd[1]); // chiudo il descrittore di lettura perché legge e basta
    }

    return 0;
}