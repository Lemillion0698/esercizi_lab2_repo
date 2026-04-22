#include <stdio.h>
#include <threads.h>

#define N 5 // numero di thread

int thread_func(void* arg){
    thrd_t id = thrd_current(); // recupera l'id del thread
    printf("Thread ID: %lu\n", id); // stampa l'ID del thread
    return 0; 
}
// NB: Nelle implementazioni comuni (come su Linux/Ubuntu che stai usando), 
// thrd_t è spesso un intero molto grande e senza segno (unsigned long).
int main(){
    thrd_t threads[N];     // array descrittori dei thread

    for (int i = 0; i < N; i++){
        thrd_create(    // crea un thread
            &threads[i], // id del thread
            thread_func, // funzione del thread
            NULL        // nessun argomento alla funzione
        ); 
    }
    
    for (int i=0; i<N; i++) {
        thrd_t ret = thrd_join(threads[i], NULL); // attende la terminazione del thread i
        if (ret == thrd_success){  // verifica il risultato del join
            printf("Thread con ID: %lu terminato correttamente.\n", threads[i]);
        } else {
            printf("Errore nel join del threads %d\n", i);
        }
    }
    return 0;
}
