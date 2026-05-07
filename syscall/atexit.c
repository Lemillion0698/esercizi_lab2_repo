#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Variabile globale per il nome del file, così che cleanup() possa usarla */
const char *tempfile = "file_temporaneo_di_prova.txt";

/* * Funzione di pulizia registrata con atexit.
 * Verrà eseguita automaticamente alla terminazione del programma.
 */
static void cleanup(void) {
    printf("\n[CLEANUP] Esecuzione della funzione cleanup() in corso...\n");
    
    /* Tenta di eliminare il file temporaneo */
    if (unlink(tempfile) == 0) {
        printf("[CLEANUP] File temporaneo '%s' eliminato con successo.\n", tempfile);
    } else {
        perror("[CLEANUP] Errore durante l'eliminazione del file");
    }
    
    /* Stampa sullo standard error come mostrato nella slide 36 */
    fprintf(stderr, "closing ...\n");
}

static void puissance1(void){
    printf("\n Esecuzione della funzione puissance1() in corso...\n");
    printf("Voici la puissance 1\n");
}

static void puissance2(void){
    printf("\n Esecuzione della funzione puissance2() in corso...\n");
    printf("Voici la puissance 2\n");
}

int main(void) {
    printf("--- INIZIO PROGRAMMA ---\n");

    /* 1. Creiamo fisicamente il file temporaneo per poter testare l'unlink */
    FILE *f = fopen(tempfile, "w");
    if (f != NULL) {
        fprintf(f, "Questi sono dati temporanei.\n");
        fclose(f);
        printf("1. File temporaneo '%s' creato su disco.\n", tempfile);
    }

    /* 2. Registriamo la funzione di pulizia con atexit() */
    if ((atexit(cleanup)) != 0) {
        fprintf(stderr, "Errore: impossibile registrare la funzione di cleanup.\n");
        exit(EXIT_FAILURE);
    }
    printf("2. Funzione 'cleanup' registrata con successo tramite atexit().\n");

    /* 3. Fine del main */
    printf("3. Il main sta per terminare con 'return 0'. Attendi...\n");
    
    /* * Quando il programma esegue return (o chiama exit()), 
     * il sistema invocherà automaticamente la funzione cleanup() 
     */
    atexit(puissance1);
    atexit(puissance2);
    exit (0);
}