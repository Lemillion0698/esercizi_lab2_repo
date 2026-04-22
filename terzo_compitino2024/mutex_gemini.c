#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

#define N_INCREMENTS 1000  // Usiamo il nome richiesto dal testo
#define N_THREADS 4

// 1. Variabile globale REALE (condivisa tra tutti i thread)
int contatore_globale = 0;

// Mutex globale
mtx_t mutex;

int incrementa(void* arg) {
    // Il parametro arg non ci serve, accediamo direttamente alla variabile globale.
    // Usiamo (void)arg per silenziare i warning del compilatore sulle variabili non usate.
    (void)arg; 

    for(int i = 0; i < N_INCREMENTS; i++) {
        // 2. Il LOCK va DENTRO il ciclo. 
        // Proteggiamo SOLO l'istruzione critica, permettendo ai thread 
        // di alternarsi (vero parallelismo).
        mtx_lock(&mutex);
        
        contatore_globale++; // Modifichiamo direttamente la variabile globale
        
        mtx_unlock(&mutex);
    }
    
    return 0; // 0 indica successo (equivalente a thrd_success per i return)
}

int main(int argc, char** argv) {
    int N = (argc > 1) ? atoi(argv[1]) : N_THREADS;

    // INIZIALIZZAZIONE DEL MUTEX
    if(mtx_init(&mutex, mtx_plain) != thrd_success) {
        fprintf(stderr, "Errore: mutex non inizializzato con successo\n");
        return 1;
    }

    // CREAZIONE DEI THREADS
    thrd_t threads[N]; 

    for (int i = 0; i < N; i++) {
        // 3. Passiamo NULL come parametro arg, tanto la funzione usa la variabile globale
        if(thrd_create(&threads[i], incrementa, NULL) != thrd_success) {
            fprintf(stderr, "Errore: thread %d non creato\n", i + 1);
            return 1;
        }
    }

    // UNIONE DEI THREADS (JOIN)
    for(int i = 0; i < N; i++) {
        int rc; // 4. Usiamo una semplice variabile locale, non serve la malloc!
        
        if(thrd_join(threads[i], &rc) != thrd_success) { 
            fprintf(stderr, "Errore: il main non ha aspettato il thread %d\n", i + 1);
            return 1;
        }
    }

    // STAMPA DEL CONTATORE GLOBALE
    printf("Contatore globale atteso: %d\n", N * N_INCREMENTS);
    printf("Contatore globale finale: %d\n", contatore_globale);

    // 5. CLEANUP: Distruzione del mutex a fine programma
    mtx_destroy(&mutex);

    return 0;
}