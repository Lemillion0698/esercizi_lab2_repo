#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

thrd_t ID_interno;


int interno(void* arg){
    ID_interno = thrd_current();// salvo l'ID del thread interno nella 
    // variabile globale

    //struct timespec ts = {.tv_sec = 3, .tv_nsec =0};
    printf("Sono il thread interno, comincio il lavoro\n");
    //thrd_sleep(&ts,NULL); // aspetta per 3 secondi
    thrd_sleep(&(struct timespec){.tv_sec=3}, NULL);
    printf("Sono il thread interno, ho finito il lavoro\n");

    return thrd_success;
}

int thread_func(void* arg){

    printf("Entro nel thread ESTERNO\n");

    if(thrd_create(&ID_interno, interno, NULL) != thrd_success){
        fprintf(stderr, "Creazione del thread %lu fallita", ID_interno);
        return EXIT_FAILURE;    
    }

    printf("Esco dal thread ESTERNO\n");
    
    return thrd_success;
}

int main(){

    thrd_t ID_esterno;

    if(thrd_create(&ID_esterno, thread_func, NULL) != thrd_success){
        fprintf(stderr, "Creazione del thread %lu fallita", ID_esterno);
        return EXIT_FAILURE; 
    }

    
    // VERIFICA DELLA TERMINAZIONE DEL THREAD ESTERNO

    thrd_t ret = thrd_join(ID_esterno, NULL);
    if( ret == thrd_success){
        printf("Il thread con ID %lu ha terminato correttamente\n", ID_esterno);
    } else {
        printf("Errore nel join del thread %lu\n", ret);
    }
    
    // VERIFICA DELLA TERMINAZIONE DEL THEAD INTERNO
    ret = thrd_join(ID_interno, NULL);
    if (ret == thrd_success){
        printf("Il thread con ID %lu ha terminato\n", ID_interno);
    } else {
        printf("Errore nel join del thread %lu\n", ret);
    }
    

    return EXIT_SUCCESS;
}