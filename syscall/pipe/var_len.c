#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pfd[2];
    pid_t pid;

    // Array di messaggi con lunghezze diverse
    char *messaggi[] = {
        "Breve",
        "Questo messaggio è decisamente più lungo",
        "Fine",
        NULL
    };

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
        /* --- PADRE (Scrittore) --- */
        close(pfd[0]);

        for (int i = 0; messaggi[i] != NULL; i++) {
            int len = strlen(messaggi[i]) + 1; // +1 per lo zero terminale

            // STEP 1: Invia la lunghezza (intero)
            write(pfd[1], &len, sizeof(int));
            
            // STEP 2: Invia il corpo del messaggio
            write(pfd[1], messaggi[i], len);

            printf("[PADRE]: Inviato messaggio %d (lunghezza: %d)\n", i, len);
            sleep(1); // Solo per visualizzare meglio lo sfasamento temporale
        }

        close(pfd[1]);
        wait(NULL);
        //snprintf
    } 
    else {
        /* --- FIGLIO (Lettore) --- */
        close(pfd[1]);

        int len_ricevuta;
        char *buffer;

        // Continua a leggere finché la pipe non viene chiusa (read ritorna 0)
        // Leggiamo prima la dimensione del prossimo messaggio
        while (read(pfd[0], &len_ricevuta, sizeof(int)) > 0) { // risponde a "Quanto è grande il pacco che sta arrivando?"
                /* è tramite il descrittore pfd[1], che la read conosce quanto spazi debba allocare ? no, pfd[1] è chiuso lato lettore */    
            // Alloca dinamicamente lo spazio necessario per il messaggio in arrivo
            buffer = (char *)malloc(len_ricevuta);
            // Legge esattamente len_ricevuta byte
            if (read(pfd[0], buffer, len_ricevuta) > 0) {// risponde a "Prendi il pacco e mettilo nel magazzino (buffer)"
    /*...e quale messaggio deve scrivere sul buffer ? perché i dati sono letti secondo la logica FIFO ?*/            
                printf("[FIGLIO]: Ricevuto messaggio di %d byte: \"%s\"\n", len_ricevuta, buffer);
            }

            free(buffer); // Fondamentale liberare la memoria dopo ogni messaggio
        }

        printf("[FIGLIO]: Pipe chiusa dallo scrittore. Esco.\n");
        close(pfd[0]);
        exit(0);
    }

    return 0;
}