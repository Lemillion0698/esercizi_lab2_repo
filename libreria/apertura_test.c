#include <stdio.h>   /* Richiesto per FILE, fopen, fclose, perror  */
#include <stdlib.h>  /* Richiesto per le macro EXIT_SUCCESS e EXIT_FAILURE */

int main(void) {
    /*  Tentativo di apertura del file in modalità lettura binaria ("rb")  */
    FILE *fp = fopen("input.dat", "rb");
    
    /* Controllo obbligatorio del valore di ritorno del puntatore */
    if (!fp) {
        /* Stampa l'errore di sistema associato a fopen (es. "No such file or directory")  */
        perror("fopen"); 
        return EXIT_FAILURE; /* Ritorna un codice di fallimento al sistema operativo (equivalente a 1)  */
    }

    /* --- [Spazio utile per l'utilizzo del file tramite fp] ---  */
    printf("[Successo] Il file 'input.dat' e' stato aperto correttamente.\n");
    /* Qui andrebbero inserite le funzioni di lettura come fgetc, fgets o fread */
    /* --------------------------------------------------------- */

    /*  Chiusura sicura del flusso con controllo del valore di ritorno  */
    if (fclose(fp) == EOF) {
        /* Se fclose restituisce EOF, si è verificato un errore (es. problemi di sincronizzazione del buffer)  */
        perror("fclose");
        return EXIT_FAILURE;
    }

    printf("[Successo] Il file e' stato chiuso correttamente senza perdite di dati.\n");
    return EXIT_SUCCESS;
}