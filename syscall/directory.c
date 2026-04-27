#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#define N 1024

/* Funzione per stampare il contenuto della directory corrente (.) */
void processdir(void) {
    DIR *d; 
    struct dirent *file;

    /* 1. opendir: apre la directory. Restituisce un puntatore DIR o NULL in caso di errore */
    if ((d = opendir(".")) == NULL) { 
        perror("Errore opening syscall"); 
        exit(EXIT_FAILURE); 
    }

    printf("Contenuto della directory:\n");
    
    /* 2. readdir: legge i file uno ad uno. 
       È importante settare errno = 0 prima di ogni lettura per distinguere
       la fine della directory (EOF) da un vero errore */
       //errno=0; // si può settare anche all'inizio del ciclo 
    while ((errno = 0, file = readdir(d)) != NULL) { // ogni volta, setta errno a 0 e readdir restituisce il puntatore ad una struttura che descrive il prossimo file nella directory
        /* Stampa il nome del file (d_name) contenuto nella struttura dirent */
        printf(" - %s", file->d_name); // settare errno a 0 è una pratica per sapere alla file del ciclo se i file sono stati letti correttamente o se c'è stato un errore
        printf(", il numero di i-node associato è : %lu\n", file->d_ino);
    }

    /* Controllo se il ciclo while è terminato per un errore o perché i file sono finiti */
    if (errno != 0) { 
        perror("Errore durante readdir"); 
    } else {
        printf("Lettura completata con successo.\n");
        printf ("errno = %d\n", errno);
    }

    /* 3. closedir: chiude la directory liberando le risorse */
    if (closedir(d) == -1) { 
        perror("Errore closing syscall");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv) {
    char buf[N]; 

    /* Controllo argomenti: il programma richiede il percorso di una directory come parametro */
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <percorso_directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* 4. getcwd: salva in 'buf' il percorso assoluto della directory corrente in cui ci troviamo */
    if (getcwd(buf, N) == NULL) {
        perror("Errore getcwd"); 
        exit(EXIT_FAILURE); 
    }
    printf("Directory di partenza: %s\n\n", buf);

    printf("Mi sposto nella directory passata come argomento: %s\n", argv[1]);
    
    /* 5. chdir: cambia la directory corrente spostando il processo nel percorso specificato */
    if (chdir(argv[1]) == -1) { 
        perror("Errore chdir");
        exit(EXIT_FAILURE); 
    }

    /* Chiamiamo la funzione che stamperà il contenuto della nuova directory corrente */
    processdir();

    /* 6. chdir: ritorno alla directory di partenza utilizzando il path salvato da getcwd all'inizio */
    printf("\nRitorno alla directory originale: %s\n", buf);
    if (chdir(buf) == -1) { 
        perror("Errore chdir di ritorno");
        exit(EXIT_FAILURE); 
    }

    return EXIT_SUCCESS;
}