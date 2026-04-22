#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

#define N 100000
#define T 4

double arr[N];

// Struttura per definire il "chunk" di lavoro di ogni thread
typedef struct {
    int start;
    int end;
    long *result_cell; // Puntatore alla cella dedicata in partial_sums
} thread_data_t;

int sommapartial(void* arg) {
    thread_data_t *d = (thread_data_t *)arg;
    long local_sum = 0;

    // Ogni thread lavora solo sul suo intervallo [start, end)
    for (int i = d->start; i < d->end; i++) {
        local_sum += (long)arr[i]; 
    }

    // Scrittura nella memoria dedicata: nessuna race condition!
    *(d->result_cell) = local_sum;

    return thrd_success;
}

int main() {
    // Inizializzazione array
    for(int i=0; i<N; i++) arr[i] = 1.0;

    thrd_t ids[T];
    thread_data_t t_args[T];
    long partial_sums[T];

    int chunk_size = N / T;

    for (int i = 0; i < T; i++) {
        t_args[i].start = i * chunk_size;
        // Gestione dell'ultimo chunk (nel caso N non sia divisibile per T)
        t_args[i].end = (i == T - 1) ? N : (i + 1) * chunk_size;
        t_args[i].result_cell = &partial_sums[i];

        if (thrd_create(&ids[i], sommapartial, &t_args[i]) != thrd_success) {
            fprintf(stderr, "Errore creazione thread %d\n", i);
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < T; i++) {
        thrd_join(ids[i], NULL);
    }

    // Riduzione finale (sequenziale nel main)
    long valore_finale = 0; // Fondamentale inizializzare!
    for (int i = 0; i < T; i++) {
        valore_finale += partial_sums[i];
    }

    printf("Il valore finale è: %ld\n", valore_finale);
    return EXIT_SUCCESS;
}