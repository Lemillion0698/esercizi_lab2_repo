#include <stdio.h>
#include <threads.h>
#include <stdlib.h>

int worker(void* arg){
    thrd_t id = thrd_current(); // recupera l' id del thread
    int tempo_rnd = 1 + (rand() % 10); // genera un tempo casuale tra 1 e 10 secondi
    printf("T%lu start\n", id);
    printf("attende per %d secondi\n", tempo_rnd);
    printf("T<id>: %lu\nend", id);
    return id*10;
}

int main () {
    int N;
    printf("Inserisci il numero di threads desiderati: "); 
    scanf("%d", &N);
    thrd_t* threads = (thrd_t*)malloc(N*sizeof(thrd_t)); // allocco spazio in memoria dinamica
    // per contenere un array di N descittori di threads

    for (int i=0; i<N; i++){
        thrd_create( // crea un thread
            &threads[i], // id del thread
            worker, // funzione del thread
            NULL           // argomento della funzione (nessunno)
        );
    }
    for(int i=0; i<N; i++){
        int result; // conterrà il valore di ritorno della thrd_join
        int ret = thrd_join(threads[i], result);  // attende la terminazione del thread i
        if (ret == thrd_success) { 
            printf("Thread %lu terminato correttamente.\n", threads[i]);
            printf("Il valore di ritorno è: %d\n", &result);
        }
        else
            printf("Errore nel join del thread %lu\n", threads[i]);
    }  

    return 0; 
}