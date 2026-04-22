#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

typedef struct {
    const int *vec_a;
    const int *vec_b;
    size_t n;
} vector_pair_t;

// Dati globali di esempio
vector_pair_t data = {
    .vec_a = (const int[]){1, 2, 3, 4, 5},
    .vec_b = (const int[]){10, 20, 30, 40, 50},
    .n = 5
};

int dot_product_worker(void *arg) {
    // < FRAMMENTO DI CODICE #1 >
    // Implementare il calcolo e l'aggiornamento del dato
    int indice_thread = *(int* )arg; // i thread riceve l'indice su cui operare
    *(int* )arg = (data.vec_a[indice_thread])*(data.vec_b[indice_thread]);
    return thrd_success;
}

int main() {
    thrd_t ids[5]; // array di identificatori di threads
    int partial_results[5] = {0, 1, 2, 3, 4};

    // < FRAMMENTO DI CODICE #2 >
    // Creazione thread, join e accumulo finale

    // Creazione di n threads, e accumulo dei valori finali
    for(int i=0; i<data.n; i++){
        thrd_create(&ids[i], dot_product_worker, &partial_results[i]);
    }

    // join
    for(int i=0; i<data.n; i++){
        thrd_join(ids[i], NULL); // il main aspetta la terminazione di ogni thread
    }

    // Ottengo il risultato finale
    int valore_finale=0;
    for(int i=0; i<data.n; i++){
        valore_finale += partial_results[i];
    }
    printf("Valore finale = %d\n", valore_finale);

    return 0;
}