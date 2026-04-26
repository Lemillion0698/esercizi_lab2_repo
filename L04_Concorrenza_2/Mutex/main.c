#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "lista.h"
#include "spinlock.h"



#define MAX_THREADS 700
#define ITERAZIONI 500
#define MUTEX_VERSION 
//#define SPINLOCK_VERSION


//lista_t lista_condivisa; // lista condivisa tra tutti i threads, è un problema dichiarla fuori dal main ?
// NON è UNA BUONA PRATICA : In C, le variabili globali non inizializzate vengono posizionate nel segmento BSS della memoria e inizializzate automaticamente a zero. 
//Quindi, i tuoi puntatori head e tail partiranno a NULL e la lunghezza a 0. Il codice per caso funzionerà.

// libreria per uno spinlock custum
atomic_flag lock = ATOMIC_FLAG_INIT;

// dichiarazione della variabile mutex
mtx_t mutex;

int thread_func (void* arg){
    lista_t* lista_condivisa = (lista_t* )arg;

    for(int i=0; i<ITERAZIONI; i++){
        #ifdef MUTEX_VERSION
        mtx_lock(&mutex);
            inserisci_in_coda(lista_condivisa, i); // è problematico inserire un elemento mentre il mutex ha il lock ?
        mtx_unlock(&mutex);
        #elif defined (SPINLOCK_VERSION) 
            spinlock(&lock);
            inserisci_in_coda(lista_condivisa, i); // è problematico inserire un elemento mentre lo spinlock è attivo ?
            spinunlock(&lock);
        #endif
    }
    return thrd_success;
}

void inizializza_mutex(mtx_t* mutex){
    if(mtx_init(mutex, mtx_plain) != thrd_success){
        fprintf(stderr, "Errore inizializzazione mutex.\n");
        exit(1);
    }
}

void distruggi_mutex(mtx_t* mutex){
    mtx_destroy(mutex);
}

int main(){
    thrd_t threads[MAX_THREADS]; // array vuoto di identificatori di threads
    lista_t lista;

    inizializa_lista(&lista);

    #ifdef MUTEX_VERSION
        inizializza_mutex(&mutex);
    #endif

    // Creazione dei threads
    for(int i=0; i<MAX_THREADS; i++){
        if(thrd_create(&threads[i],thread_func, &lista)!= thrd_success){
            fprintf(stderr, "Il thread T%d NON è stato creato con successo\n", i);
            return EXIT_FAILURE;
        }
    }

    // Terminazione dei threads
    for(int i=0; i<MAX_THREADS; i++){
        if(thrd_join(threads[i], NULL) != thrd_success){
            fprintf(stderr, "Il thread T%d NON è stato terminato con successo\n", i);
            return EXIT_FAILURE;
        }
    }

    stampa_report(&lista);

    #ifdef MUTEX_VERSION
        distruggi_mutex(&mutex);
    #endif

    printf("Termine Programma.\n");
    
    return EXIT_SUCCESS;
}

