#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

#define INCREMENTS 1000
// #define contatore_globale 0 // sbagliato perché sostituisce il contatore_globale con la stringa "0", è come una costante ? sì, è 
// una direttiva del compilatore, non una variabile del codice C

// Variabile globale REALE (condivisa tra tutti i threads)
int contatore_globale = 0;

// Mutex globale
mtx_t mutex;

int incrementa(void* arg){ // la strategia è di passare a questa funzione il contatore globale come argomento
    //int conta = *(int* )arg; // sbagliato perché è una variabil locale al thread,...
    // mtx_lock(&mutex); (1) regola d'oro dei mutex: ogni thread tiene il lock per il minor tempo possibile
    for(int i=0; i<INCREMENTS; i++) {
        //conta++;//...la sua modifica non ha effetto su contatore_globale
        mtx_lock(&mutex);
        contatore_globale++; // ogni thread a sua volta, incrementa il contatore globale (1) la serializzazione dei
        // threads dura solo per il tempo di un'iterazione, quindi è ottimo
        mtx_unlock(&mutex);
    }
    //(1)mtx_unlock(&mutex); // non è buono, perché metto i threads in serie : un thread prende il lock per INCREMENTS
    // iterazioni poi lo lascia ad un altro, che farà la stessa cosa e così via ... non è più concorenza reale (1)
    return thrd_success;
}

int main(int argc, char** argv){
    int N = (argc > 1) ? atoi(argv[1]) : 4; // per impostare per default un num di threads

    // INIZIALIZZAZIONE DEL MUTEX
    if(mtx_init(&mutex, mtx_plain) != thrd_success){
        fprintf(stderr, "mutex non inizializzato con successo");
        return 1;
    }

    // CREAZIONE DEI THREADS
    thrd_t threads[N]; // array di identificatori di threads

    for (int i=0; i<N; i++){
        if(thrd_create(&threads[i], incrementa, NULL) != thrd_success){
            fprintf(stderr, "thread %d non creato", i+1);
            return 1;
        }
    }

    // Unisco tutti i threads al thread principale (main)
    //int* res = (int*) malloc(N * sizeof(int)); // allocazione dinamica di un 
    // array che conterrà i diversi valori di ritorno dei threads  NON NECESSARIO

    for(int i=0; i<N; i++){
        int res;
        if(thrd_join(threads[i], &res) != thrd_success){ 
            fprintf(stderr, "il main non ha aspettato la terminazione del thread %d", i+1);
            return 1;
        }
    }

    // STAMPA DEL CONTATORE GLOBALE
    printf("Contatore globale = %d\n", contatore_globale);
    // free (res);
    return 0;
}