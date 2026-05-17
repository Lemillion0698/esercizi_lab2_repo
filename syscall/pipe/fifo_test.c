#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> // FONDAMENTALE per mkfifo()
#include <fcntl.h>    // FONDAMENTALE per O_RDONLY e O_WRONLY
#include <sys/wait.h>
#include <errno.h>

#define FIFO_PATH "/tmp/mio_fifo"

int main() {
    pid_t pid;

    /* 1. CREAZIONE DELLA FIFO
       La syscall mkfifo() crea il file speciale nel file system.
       0666 indica i permessi di lettura e scrittura per tutti.
    */
    printf("[MAIN]: Tento di creare la FIFO in %s...\n", FIFO_PATH);
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        // Se la FIFO esiste già da un test precedente, non lo consideriamo un errore grave
        if (errno != EEXIST) {
            perror("Errore imperdonabile nella mkfifo");
            return 1;
        }
        printf("[MAIN]: La FIFO esiste già, uso quella esistente.\n");
    } else {
        printf("[MAIN]: FIFO creata con successo!\n");
    }

    // Facciamo una fork per creare un processo Scrittore e uno Lettore
    pid = fork();
    if (pid < 0) {
        perror("Errore fork");
        return 1;
    }

    if (pid == 0) {
        /* --- PROCESSO FIGLIO (Scrittore) --- */
        printf("[SCRITTORE]: Sto per aprire la FIFO in scrittura...\n");
        printf("[SCRITTORE]: ATTENZIONE: Se il lettore non è ancora pronto, mi bloccherò qui!\n");
        
        // La open() su una FIFO è BLOCCANTE di default
        int fd_write = open(FIFO_PATH, O_WRONLY);
        if (fd_write == -1) {
            perror("Errore open in scrittura");
            exit(1);
        }
        printf("[SCRITTORE]: Aperta! Il lettore è arrivato. Scrivo il messaggio...\n");

        char msg[] = "Messaggio segreto passato tramite FIFO!";
        write(fd_write, msg, sizeof(msg));

        close(fd_write);
        printf("[SCRITTORE]: Ho finito, chiudo e termino.\n");
        exit(0);
    } 
    else {
        /* --- PROCESSO PADRE (Lettore) --- */
        // Ritardiamo volutamente la partenza del lettore per farti vedere il blocco dello scrittore
        printf("[LETTORE]: Dormo per 3 secondi prima di aprire la FIFO...\n");
        sleep(3);

        printf("[LETTORE]: Sto per aprire la FIFO in lettura...\n");
        int fd_read = open(FIFO_PATH, O_RDONLY);
        if (fd_read == -1) {
            perror("Errore open in lettura");
            return 1;
        }
        printf("[LETTORE]: Aperta! Leggo i dati...\n");

        char buffer[100];
        read(fd_read, buffer, sizeof(buffer));
        printf("[LETTORE]: Ho letto dalla FIFO: \"%s\"\n", buffer);

        close(fd_read);
        wait(NULL); // Aspetta il figlio

        /* 3. RIMOZIONE DELLA FIFO
           A differenza delle pipe normali che spariscono da sole, le FIFO rimangono 
           sul disco fisso finché non le cancelli esplicitamente con la unlink().
        */
        printf("[LETTORE]: Rimuovo il file della FIFO dal sistema...\n");
        unlink(FIFO_PATH);

        printf("[LETTORE]: Fine test.\n");
    }

    return 0;
}