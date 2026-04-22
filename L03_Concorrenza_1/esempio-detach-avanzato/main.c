#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <time.h>

atomic_int thread_attivi = 0;
#define N 5 // numero di threads

int thread_func(void* arg){
    // recupero l'ID del thread attualmente in esucuzione dal Sistema Operativo
    thrd_t ID = thrd_current(); 
    int indice = *(int* )arg;
    printf("L'ID del thread T%d è %lu\n", indice, ID); 

    // tempo di attesa
    struct timespec ts = {.tv_sec = 1+(rand() % 4), .tv_nsec=0};
    printf("Il thread T%d si mette in attesa per %ld secondi\n", indice, ts.tv_sec);
    thrd_sleep(&ts, NULL);

    // decremento della variabile atomica
    atomic_fetch_sub(&thread_attivi, 1);
    printf("Il thread %d ha portato la variabile atomica a %d\n", indice, thread_attivi);

    return thrd_success;
}

int main(){
    srand(time(NULL));

    thrd_t threads_ids[N]; // contenitore dei descrittori di N threads
    int indici_threads[N]; // array per contenere gli indici dei threads

    // Creazione dei N threads
    for(int i=0; i<N; i++){
        indici_threads[i]=i;
        if(thrd_create(&threads_ids[i], thread_func, &indici_threads[i]) != thrd_success){
            fprintf(stderr, "Il thread T%d non è stato creato con successo\n", i);
            return EXIT_FAILURE;
        }

        atomic_fetch_add(&thread_attivi, 1); // il main incrementa in modo sicuroin modo sicuro la variabile atomica
        
        // stacco del thread i dal flusso di esecuzione principale
        thrd_detach(threads_ids[i]); 
        printf("Il thread T%d si stacca dal processo main\n", indici_threads[i]);
    }

    // Tentativo di chiamare la join
    for(int i=0; i<N; i++){
        indici_threads[i]=i;
        if(thrd_join(threads_ids[i], NULL) != thrd_success){
            fprintf(stderr, "Il thread T%d NON si è unito al main\n", indici_threads[i]);
            //return EXIT_FAILURE; chiude il programma 
        }
    }

    #ifdef BARRIER
    while (thread_attivi > 0){
        for(int i=0; i<N; i++){
            printf("Thread attivi: %d\n", atomic_load(&thread_attivi));
            struct timespec ts = 
            { .tv_sec = 0, .tv_nsec = 5.0e8};
            thrd_sleep(&ts, NULL);
        }
    }
    #endif

    // valore della variabile atomica
    printf("Infine, la variabile atomica vale: %d\n", thread_attivi);

    return EXIT_SUCCESS;
}