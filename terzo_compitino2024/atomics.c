#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#define N_THREADS 5
#define N_INCREMENTS 100000

atomic_int counter = 0; // Contatore atomico

// Funzione eseguita da ciascun thread
int thread_func(void* arg){
    int indice_thread = *(int *)arg;

    /*for(int i=0; i<N_INCREMENTS; i++){
        if((indice_thread % 2) == 0){ // non serve fare il controllo sull'indice ad ogni iterazione, visto che indice_thread non cambia
            atomic_fetch_add(&counter, 1);
        } else{
            atomic_fetch_add(&counter, -1);
        }
    }*/

    if((indice_thread % 2) == 0){
        for(int i=0; i<N_INCREMENTS; i++){
            atomic_fetch_add(&counter, 1);
        }
    } else {
        for(int i=0; i<N_INCREMENTS; i++){
            atomic_fetch_add(&counter, -1);
        }
    }
    return thrd_success;
}

int main() {
    thrd_t threads[N_THREADS];
    int thread_ids[N_THREADS]; // CORREZIONE: Array per salvare gli ID in sicurezza, è necessario ? si perché 
    // in questo modo, ogni thread sà di sicuro quale valore dell'indice prendre, ogni thread avendo velocità 
    // arbitraria, non posso fidarmi del valore di una variabile che cambia ad ogni iterazione.

    // CREAZIONE DEI THREADS
    for (int i=0; i<N_THREADS; i++){
        thread_ids[i] = i; // Ogni id di thread ha la sua "cella" di memoria sicura
        if(thrd_create(&threads[i], thread_func, &thread_ids[i]) != thrd_success){
            fprintf(stderr, "Errore: thread %d non creato\n", i);
            return 1; 
        }
    }

    // UNIONE DEI THREADS (JOIN)
    for (int i=0; i<N_THREADS; i++){
        if(thrd_join(threads[i], NULL) != thrd_success) { 
            fprintf(stderr, "Errore: il main non ha aspettato il thread %d\n", i);
            return 1;
        }
    }

    printf("counter = %d\n", counter);

    return 0; 
}