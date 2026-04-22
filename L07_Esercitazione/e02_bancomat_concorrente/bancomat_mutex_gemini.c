#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h> // per srand

#define FAMIGLIA 8
#define N_TRANSAZIONI 10000

int saldo = 1000;
mtx_t mutex;

int transazione(void* arg) {
    // Nota: il seed dovrebbe essere diverso per ogni thread se usassimo rand_r
    for(int i = 0; i < N_TRANSAZIONI; i++) {
        // Genero il numero DENTRO il ciclo per simulare transazioni diverse
        int variazione = (rand() % 21) - 10; 
        
        mtx_lock(&mutex);
        saldo += variazione; // Uso += perché la variazione ha già il segno
        mtx_unlock(&mutex);
    }
    return thrd_success;
}

int main() {
    srand(time(NULL)); // Inizializzo il generatore di numeri casuali

    if(mtx_init(&mutex, mtx_plain) != thrd_success) {
        fprintf(stderr, "Errore: mtx_init\n");
        return EXIT_FAILURE;
    }

    thrd_t td[FAMIGLIA];
    int creati = 0;

    for(int i = 0; i < FAMIGLIA; i++) {
        if(thrd_create(&td[i], transazione, NULL) != thrd_success) {
            fprintf(stderr, "Errore: thrd_create thread %d\n", i);
            break; // Esco dal ciclo di creazione
        }
        creati++;
    }

    // Join solo dei thread effettivamente creati
    for(int i = 0; i < creati; i++) {
        thrd_join(td[i], NULL);
    }

    printf("Il saldo finale è: %d\n", saldo);

    mtx_destroy(&mutex);
    return EXIT_SUCCESS;
}