#include <stdio.h>
#include <threads.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 5 // numero di thread

int thread_func(void* arg) {
    thrd_t id = thrd_current(); // recupera l' id del thread
    int tempo_rnd = 1 + (rand() % 4);   // genera un tempo casuale tra 1 e 4 secondi
    printf("Inizio Thread: %lu, resta attivo " // stampa l'inizio del thread
            "per %d sec\n", id, tempo_rnd);
    struct timespec ts =    // imposta il tempo di attesa
        { .tv_sec = tempo_rnd, .tv_nsec = 0 }; 
    thrd_sleep(&ts, NULL); // attende il tempo impostato
    printf("Fine Thread: %lu\n", id); // stampa la fine del thread ; cioè il thread termina anche prima della chiamata della join ? no, il thread termina dopo la return thrd_success
    return thrd_success;             // ritorna il valore di successo
}

int main(){
    thrd_t threads[N]; // array di thread, non è array di descrittori 
    // di thread ? // no, è un array di identificatori di threads, che sono nomi (in realtà sono numeri) a quelli 

    for (int i=0; i<N; i++){
        thrd_create( // crea un thread
            &threads[i], // id del thread
            thread_func, // funzione del thread
            NULL           // argomento della funzione (nessunno)
        );

        printf("Thread %lu creato.\n", threads[i]);
        fflush(stdout); // scarica dal buffer sullo standard output (a video, sul terminale) quello che c'è scritto cioè "Thread thread[i] creato.\n", in questo caso ? sì

        //thrd_detach(threads[i]);    // imposta il thread come detatched (isolato dal main)
                                    // il thread non deve essere esplicitamente
                                    // terminato il sistema operativo si occupa
                                    // di liberare le risorse allocate
    }

    for(int i=0; i<N; i++){
        int ret = thrd_join(threads[i], NULL); // attende la terminazione del thread i ; noto che senza la join, il main termina senza che ogni thread abbia 
    //stampato "Fine Thread: %lu\n", id , è normale ? sì
        if(ret == thrd_success)     // verifica il risultato del join
            printf("Thread %lu terminato correttamente.\n", threads[i]);
        else
            printf("Errore nel join del thread %lu\n", threads[i]);
    }                           

    printf("Termine Programma.\n");
    return 0;
}

 // CURIOSITA' : Quando faccio ctrl + s, dove viene salvato il contenuto del mio file, (ad es questo) sul disco o sul buffer ?  sovrascrivere sul contenuto
 // del file, nel disco