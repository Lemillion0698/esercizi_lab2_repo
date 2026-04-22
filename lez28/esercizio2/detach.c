#include <stdio.h>
#include <threads.h>
#include <time.h>

struct timespec ts = {.tv_sec = 3, .tv_nsec=0};
struct timespec ts2= {.tv_sec=0, .tv_nsec=200000L};
struct timespec ts10 = {.tv_sec=10, .tv_nsec=0};

int stampa_tick(void* arg){
    
    while (!thrd_sleep(&ts, NULL)){
        printf("tick\n");
        thrd_sleep(&ts2, NULL);
    
    }
    return thrd_success;
}

int main(){
    thrd_t logger; // variabile che contiene il descrittore del thread
    thrd_create(&logger, stampa_tick, NULL);
    thrd_detach(logger); // il "t"hread main" potrebbe terminare prima che
    // il tread logger abbia terminato la sua esecuzione, viene gestito dal
    // sistema operativo (staccato dal flusso di esecuzione principale)
    thrd_sleep(&ts10,NULL);
    printf("main exiting\n");
    return 0;
}