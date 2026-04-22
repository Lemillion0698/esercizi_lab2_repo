#include <stdio.h>
#include <threads.h>
#include <time.h>

// Definiamo il tempo di attesa di 200ms
struct timespec ts_200ms = {.tv_sec = 0, .tv_nsec = 200000000};
struct timespec ts = {.tv_sec=2, .tv_nsec=0};

int stampa_tick(void* arg) {
    for (int i = 0; i < 15; i++) { // 15 volte * 200ms = 3 secondi
        printf("tick\n");
        // thrd_sleep richiede un puntatore alla struttura
        thrd_sleep(&ts_200ms, NULL);
    }
    return thrd_success;
}

int main() {
    thrd_t logger;
    
    if (thrd_create(&logger, stampa_tick, NULL) != thrd_success) {
        fprintf(stderr, "Errore nella creazione del thread\n");
        return 1;
    }

    // Rendiamo il thread indipendente
    thrd_detach(logger);

    // Il main stampa e termina immediatamente
    printf("main exiting\n");
    thrd_sleep(&ts, NULL);
    
    return 0; 
    // ATTENZIONE: Qui il processo muore. 
    // Probabilmente non vedrai alcun "tick" o ne vedrai solo uno!
}