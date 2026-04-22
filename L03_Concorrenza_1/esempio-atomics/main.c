#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <threads.h>

int globale_intera = 0;
atomic_int globale_atomica = 0;
// poi provare con una struct che ha tutte queste variabili int
#define N_THREADS 5 // numero di threads
#define ITERAZIONI 1000000 // numero di iterazioni

int incrementa(void* arg){
    for (int i=0; i<ITERAZIONI; i++){
        globale_intera++;
        globale_atomica++;
    }
    return thrd_success;
}

int main(){

    thrd_t ids[N_THREADS]; // array di identificatori dei threads

    // creazione dei threads
    for(int i=0; i<N_THREADS; i++){
        if(thrd_create(&ids[i], incrementa, NULL) != thrd_success){
            fprintf(stderr, "Il thread T%d non è stato creato\n", i);
            return EXIT_FAILURE;
        }
    }

    // terminazione dei threads
    for(int i=0; i<N_THREADS; i++){
        int result = thrd_join(ids[i], NULL);
        if(result != thrd_success){
            printf("Il thread T%d NON ha terminato correttamente\n", i);
        } else{
            printf("Il thread T%d ha terminato correttamente\n", i);
        }
    }

    // stampa dei valori finali
    printf("globale_intera = %d\n", globale_intera);
    printf("globale atomica = %d\n", globale_atomica);
    
    return EXIT_SUCCESS;
}