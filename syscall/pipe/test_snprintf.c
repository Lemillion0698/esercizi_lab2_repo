#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define N 50 // Dimensione massima del buffer per snprintf

int main() {
    int pfd[2];
    pid_t pid;

    if (pipe(pfd) == -1) {
        perror("Errore pipe");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("Errore fork");
        return 1;
    }

    if (pid > 0) { 
        /* --- PADRE (Lettore) --- */
        close(pfd[1]); // Chiude scrittura

        int lung_letta;
        char *buffer_ricevuto;

        // Legge la lunghezza inviata dal figlio
        if (read(pfd[0], &lung_letta, sizeof(int)) > 0) {
            
            // Alloca lo spazio e legge il messaggio
            buffer_ricevuto = malloc(lung_letta);
            read(pfd[0], buffer_ricevuto, lung_letta);

            printf("[PADRE]: Messaggio ricevuto -> %s", buffer_ricevuto);
            free(buffer_ricevuto);
        }

        close(pfd[0]);
        wait(NULL);
    } 
    else { 
        /* --- FIGLIO (Scrittore) --- */
        char msg[N];
        int lung, k;

        close(pfd[0]); // Chiude lettura

        /* ESPERIMENTO snprintf:
           Componiamo una stringa che include il PID del figlio.
           snprintf garantisce di NON scrivere più di N caratteri nel buffer 'msg'.
        */
        snprintf(msg, N, "Da %d: Hi!! Sono il figlio.\n", getpid());

        // Calcoliamo la lunghezza reale della stringa prodotta (incluso \0)
        lung = strlen(msg) + 1;

        printf("[FIGLIO]: Ho generato la stringa: %s", msg);
        printf("[FIGLIO]: Lunghezza calcolata: %d byte\n", lung);

        // PRIMA SCRITTURA: La lunghezza
        k = write(pfd[1], &lung, sizeof(int));
        if (k == -1) perror("Errore write lung");

        // SECONDA SCRITTURA: Il messaggio reale
        k = write(pfd[1], msg, lung);
        if (k == -1) perror("Errore write msg");

        close(pfd[1]);
        exit(0);
    }

    return 0;
}