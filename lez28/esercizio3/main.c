#include <stdio.h>
#include <threads.h>

// Step 1: Usiamo un int normale per osservare la data race
int counter = 0; 
#define T 5 
#define K 1000000 // Aumentato per rendere visibile la race condition

int incrementa_counter(void* arg){
    for(int i=0; i<K; i++){
        counter++; // Operazione non atomica: READ -> MODIFY -> WRITE
    }
    return thrd_success;
}

int main() {
    thrd_t threads[T]; // array di descrittori di threads

    for(int i=0; i<T; i++){
        if (thrd_create(&threads[i], incrementa_counter, NULL) != thrd_success) {
            fprintf(stderr, "Errore creazione thread %d\n", i);
            return 1;
        }
    }

    for(int j=0; j<T; j++){
        if (thrd_join(threads[j], NULL) == thrd_success) {
            // Nota: %lu per thrd_t non è portabile su tutti i sistemi, 
            // ma per ora va bene per il tuo test su Linux
            printf("Thread %d terminato.\n", j);
        }
    }

    printf("\nRisultato finale: %d\n", counter);
    printf("Risultato atteso: %d\n", T * K);
    
    if (counter != T * K) {
        printf("DATA RACE RILEVATA!\n");
    } else {
        printf("Risultato corretto (fortuna o K troppo basso).\n");
    }

    return 0;
}