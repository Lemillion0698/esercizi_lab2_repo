#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

#define N 5 // numero di threads

//struct timespec ts ; // errore, perché tutti i threads condividono la stessa
// area di memoria per ts, vero ?

int calcolo (void* arg){
    thrd_t ID = thrd_current();

    // inizializzazione del tempo di attesa di ogni thread
    struct timespec ts;
    ts.tv_sec = 1 + (rand()%4);
    ts.tv_nsec = 0;

    printf("ID del thread %lu partito (dormirà %ld sec)\n", ID, ts.tv_sec);
    thrd_sleep(&ts, NULL);
    printf("thread con ID %lu ha terminato\n", ID);
    return thrd_success;
}
int main(){
    srand(time(NULL)); // così rand() genererà una sequenza diversa di numeri ogni 
    // volta che avvierò il programma
    thrd_t T[N]; // array che accoglierà i descrittori di threads

    // creazione e detach dei threads
    for(int i=0; i<N; i++){
        thrd_create(&T[i], calcolo, NULL);
        printf("Thread %lu creato.\n", T[i]);
		fflush(stdout);

        thrd_detach(T[i]);// stacco il thread dal flusso di esecuzione principale 
        // cioè, a partire da ora, è gestito dal sistema operativo(è indipendente)
    }

    // terminazione dei threads con la join
    for(int i=0; i<N; i++){
        if(thrd_join(T[i],NULL) != thrd_success){
            printf("Il thread T%d NON è stato unito\n", i);
        } else{
            printf("Il thread T%d è stato correttamente unito\n", i);
        }
    }

    printf("main terminato con successo\n");
    return EXIT_SUCCESS; 
}