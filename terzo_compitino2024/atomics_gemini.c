#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#define N_THREADS 4
#define N_INCREMENTS 100000

atomic_int counter = 0; // Contatore atomico

// Funzione eseguita da ciascun thread
int thread_func(void* arg){
    int indice_thread = *(int *)arg;

    // OTTIMIZZAZIONE: Controlliamo se è pari o dispari UNA SOLA VOLTA
    if((indice_thread % 2) == 0) {
        // PARI: Incrementa
        for(int i=0; i<N_INCREMENTS; i++){
            atomic_fetch_add(&counter, 1);
        }
    } else {
        // DISPARI: Decrementa
        for(int i=0; i<N_INCREMENTS; i++){
            atomic_fetch_add(&counter, -1);
        }
    }
    
    return thrd_success;
}

int main() {
    thrd_t threads[N_THREADS];
    int thread_ids[N_THREADS]; // CORREZIONE: Array per salvare gli ID in sicurezza

    // CREAZIONE DEI THREADS
    for (int i = 0; i < N_THREADS; i++){
        thread_ids[i] = i; // Ogni thread ha la sua "cella" di memoria sicura
        
        // Passiamo l'indirizzo della cella specifica, non della variabile del ciclo!
        if(thrd_create(&threads[i], thread_func, &thread_ids[i]) != thrd_success){
            fprintf(stderr, "Errore: thread %d non creato\n", i);
            return 1; 
        }
    }

    // UNIONE DEI THREADS (JOIN)
    for (int i = 0; i < N_THREADS; i++){
        if(thrd_join(threads[i], NULL) != thrd_success) { 
            fprintf(stderr, "Errore: il main non ha aspettato il thread %d\n", i);
            return 1;
        }
    }

    printf("counter finale = %d\n", counter);

    return 0; 
}