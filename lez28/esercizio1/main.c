#include <stdio.h>
#include <threads.h>
#include <time.h>

#define N 5

typedef struct {
    thrd_t threads;
    int tempo;
}  argomento;

int worker(void* arg){
    int id = *(int*)arg; // converto arg in puntatore a int "!(int*)" " poi lo derefferenzio "*" " per ottenere il valore a cui punta; passo un id ad ogni thread
    printf("T<%d>: start\n", id);
    thrd_sleep(&(struct timespec){.tv_sec = 5}, NULL); // ogni thread dorme per 5 secondi
    printf("T<%d>: end\n", id);
    return id*10;
}

int main(){

    thrd_t threads[N]; // array di descrittori di N threads
    int ids[N]; // array per memorizzare gli identificatori
    argomento primo; 
    
     // creazione dei threads
    for (int i=0; i<N; i++){
        ids[i]=i+1;
        if(thrd_create(&threads[i], worker, &ids[i]) != thrd_success){ // con &ids passo un valore a arg della worker ? / sì
            fprintf(stderr, "thrd_create failed on %d\n", ids[i]);
            return 1;
        }
    }
    
    // attesa del main e terminazione di ogni thread
    for(int i=0; i<N; i++){
        int res=0; // per ricevere il valore di ritorno del thread terminato
        int result = thrd_join(threads[i], &res);
        if(result != thrd_success){
            printf("Errore nel join del thread T<%d>\n", i);
        } else{
            printf("thread T%d terminato, il valore restituito è res = %d\n", i+1, res);
        }
    }

    return 0;
}