#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

#define Buffer_circolare 10
#define N_INCREMENTS 3
#define MEZZO_SECONDO 5.0e8
#define N_THREADS 2
typedef struct{
    int buffer;
    int indice_inserimento;
    int indice_estrazione;
    mtx_t lock;
} SharedData;


int produttore(void* arg){
    SharedData data = *(SharedData*) arg;
    data.indice_inserimento = 1;

    mtx_lock(&(data.lock));

    for(int i= data.indice_inserimento; i<=N_INCREMENTS; i++){
        while(data.buffer == Buffer_circolare){
            mtx_unlock(&(data.lock));
            thrd_sleep(&(struct timespec){.tv_sec = MEZZO_SECONDO}, NULL);
        }
        data.buffer += data.indice_inserimento; // incremento il buffer condiviso
    }
         
    return thrd_success;
}

int consumatore(void* arg){
    SharedData data = *(SharedData* )arg;
    
    do {
        scanf("Inserisci un nuovo numero: %d",&data.indice_estrazione);
    } while(data.indice_estrazione > data.buffer); // finché l'utente inserisca
    // un numero maggiore di quello che c'è nel buffer

    int somma_globale = data.buffer - data.indice_estrazione;
    data.buffer = somma_globale;

    while(data.buffer == 0){
        thrd_sleep(&(struct timespec){.tv_sec = MEZZO_SECONDO}, NULL);
    }

    return thrd_success;
}

int main() {

    thrd_t ids[N_THREADS];
    SharedData t_args[N_THREADS];

    if(thrd_create(&ids[0], produttore, &t_args[0]) != thrd_success){
        fprintf(stderr, "Thread prodottore fallito");
        return EXIT_FAILURE;
    }

    if(thrd_create(&ids[1], consumatore, &t_args[1]) != thrd_success){
        fprintf(stderr, "Thread consumotore fallito");
        return EXIT_FAILURE;
    }
    int* res = NULL; 
    thrd_join(ids[0], &res);

    if(*res == thrd_success){
        thrd_join(ids[1], NULL);
    }

    
    return EXIT_SUCCESS;
}