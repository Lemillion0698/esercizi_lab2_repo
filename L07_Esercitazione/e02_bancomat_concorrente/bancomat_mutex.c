#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

#define FAMIGLIA 8
#define N_TRANSAZIONI 10000

int saldo = 1000;

mtx_t mutex; // dichiarazione di un mutex

int transazione(void* arg){
    //int num_casual = (rand() % 21) - 10;// genera un numero casual tra -10 e 10; sbagliato 
    // qui perché non rispetta la traccia: "In ogni transazione, il thread genera un numero casuale"
    for(int i=0; i < N_TRANSAZIONI; i++){
        int num_casual = (rand() % 21) - 10;// genera un numero casual tra -10 e 10
        mtx_lock(&mutex);
        saldo -= num_casual;
        mtx_unlock(&mutex);
    }
    return EXIT_SUCCESS;
}

int main(){
    srand(time(NULL)); // trucco per avere un saldo diverso ad ogni esecuzione
    // inizializzazione del mutex e gestione dell'errore
    if(mtx_init(&mutex, mtx_plain) != thrd_success){
        fprintf(stderr, "mutex non inizializzato con successo");
        return 1;
    }

    // creazione dei threads
    thrd_t td[FAMIGLIA];
    for(int i=0; i<FAMIGLIA; i++){
        if(thrd_create(&td[i], transazione, NULL) != thrd_success){
            fprintf(stderr, "creazione thread fallita");
            return 1;
        }
    }

    // terminazione dei threads
    for(int i=0; i<FAMIGLIA; i++){
        thrd_join(td[i], NULL);
    }

    // stampa del risultato finale
    printf("Il saldo finale è: %d\n", saldo);

    mtx_destroy(&mutex); // destruzione del mutex
    
    return 0;
}