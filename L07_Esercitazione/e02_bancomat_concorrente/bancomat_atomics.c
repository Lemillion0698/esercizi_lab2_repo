#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h> // per time(NULL)
#include <stdatomic.h>

#define FAMIGLIA 8
#define N_TRANSAZIONI 10000

atomic_long saldo = 1000; // in tipo atomic_long garantisce che questa variabile
// sia modificata da un solo thread alla volta

int transazione(void* arg){ // operazione di ciacun thread
    for(int i=0; i<N_TRANSAZIONI; i++){
        int num_casuale = (rand()%21)-10;
        atomic_fetch_add(&saldo, num_casuale); 
    }
    return thrd_success;
}

int main(){
    srand(time(NULL)); // trucco per avere un saldo diverso ad ogni esecuzione
    // creazione dei threads
    thrd_t td[FAMIGLIA];
    for(int i=0; i<FAMIGLIA; i++){
        if(thrd_create(&td[i], transazione, NULL) != thrd_success){
            fprintf(stderr, "creazione thread fallita");
            return EXIT_FAILURE;
        }
    }

    // terminazione dei threads
    for(int i=0; i<FAMIGLIA; i++){
        thrd_join(td[i], NULL);
    }

    // stampa del risultato finale
    printf("Il saldo finale è: %ld\n", saldo);
    return EXIT_SUCCESS;
}