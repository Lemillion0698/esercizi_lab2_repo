#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

#define N 5 // numero di threads
int count = 0;
mtx_t mutex;
cnd_t cond_var; 

void barrier(){
    mtx_lock(&mutex);
    count ++;
    if (count == N){
        count = 0; 
        cnd_broadcast(&cond_var);
        mtx_unlock(&mutex);
        //return;    
    } else if (count < N) {
        cnd_wait(&cond_var, &mutex);
        mtx_unlock(&mutex);
        //return;
    }
    return;
}

int lavoro(void* arg){
    int id = *(int*) arg;
    struct timespec ts;
    int tempo_attesa=ts.tv_sec = 1 + (rand()%5); // tempo casuale tra 1 e 5 secondi
    ts.tv_nsec = 0; 

	printf("Thread %d: inizio, aspetterò per %d secondi\n", id, (tempo_attesa));
    thrd_sleep(&ts, NULL); // il thread attuale si addormenta per un tempo casuale
    barrier();
    printf("Il thread %d ha finito il suo lavoro\n", id);

    return thrd_success;
}

int main(){
    thrd_t threads[N];
    int* ids_threads =(int*) malloc(N*sizeof(int));

    //inizializzazione del mutex
    if(mtx_init(&mutex, mtx_plain) != thrd_success){
        fprintf(stderr, "mutex non inizializzato\n");
        return EXIT_FAILURE;
    }

    if(cnd_init(&cond_var) != thrd_success){
        fprintf(stderr, "condition variable non inizializzata\n");
        return EXIT_FAILURE;
    }

    // creazione dei threads
    for(int i=0; i<N; i++){
        ids_threads[i]=i;
        if(thrd_create(&threads[i], lavoro, &ids_threads[i]) != thrd_success){
            fprintf(stderr, "il thread %d NON è stato creato con successo\n", ids_threads[i]);
            return EXIT_FAILURE;
        }
    }

    // terminazione dei threads
    for(int i=0; i<N; i++){
        if(thrd_join(threads[i], NULL) != thrd_success){
            fprintf(stderr, "il thread %d NON si è unito al main\n", ids_threads[i]);
            return EXIT_FAILURE;
        }
    }

    // destruzione delle strutture di sincronizzazione
    mtx_destroy(&mutex);
    cnd_destroy(&cond_var);

    return EXIT_SUCCESS;
}