#include <stdio.h>
#include <threads.h>
#include <stdlib.h>
#include <time.h> // (1) è necessaria questa libreria ? non basta stdlib.h ?

#define N 5 // Numero di thread richiesto

// Struttura per rispettare il vincolo: passare ID e tempo senza variabili globali
typedef struct {
    int id;
    int wait_time;
} thread_data_t; // (2) ma questa struttura non è globale ? quindi contiene variabili 
// globali, o sbaglio ?

// Funzione worker richiesta dall'obiettivo
int worker(void *arg) {
    // Recupero dei dati tramite cast dal puntatore void*
    thread_data_t *data = (thread_data_t *)arg;// (3) cosa rappresenta arg ? un puntatore generico
    // al thread ? si poteva recuperare l'id del thread ed il tempo di attesa tramite 
    // la funzionbe thrd_current ?
    int id = data->id;
    int tempo = data->wait_time;

    // Stampa di inizio come richiesto: "T<id>: start"
    printf("T%d: start\n", id);

    // Attesa di un breve intervallo (in secondi)
    struct timespec ts = { .tv_sec = tempo, .tv_nsec = 0 };
    thrd_sleep(&ts, NULL);

    // Stampa di fine come richiesto: "T<id>: end"
    printf("T%d: end\n", id);

    // Vincolo: restituire un int significativo (es. id * 10)
    return id * 10;
}

int main() {
    thrd_t threads[N];              // Array di descrittori di thread [1]
    thread_data_t args[N];          // Array di strutture per i parametri; ogni struct
    // contiene l'identificatore e il tempo di attesa di un thread ?
    //int results[N];                 // Array per memorizzare i valori di ritorno; in 
    // questo codice, abbiamo piuttosto usato una variabile temporanea res, ma è uguale

    // Inizializzazione del seme per tempi casuali (facoltativo ma utile)
    //srand(time(NULL));

    // Fase 1: Creazione degli N thread
    for (int i = 0; i < N; i++) {
        args[i].id = i;             // ID da 0 a N-1
        args[i].wait_time = 1 + (rand() % 3); // Tempo di attesa tra 1 e 3 secondi

        if (thrd_create(&threads[i], worker, &args[i]) != thrd_success) {
            fprintf(stderr, "Errore nella creazione del thread %d\n", i);
            return thrd_error;
        }
    }

    // Fase 2: Il main fa join su tutti i thread e stampa il valore di ritorno
    for (int i = 0; i < N; i++) {
        int res; // variabile temporanea per raccogliere i valori di ritorno di ogni singolo thread
        // thrd_join attende il thread specifico e recupera il valore restituito [1]
        if (thrd_join(threads[i], &res) == thrd_success) {
            printf("Main: Thread T%d terminato. Valore restituito: %d\n", i, res);
        } else {
            printf("Errore nel join del thread %d\n", i);
        }
    }

    printf("Main: tutti i thread hanno terminato.\n");
    
    return 0;
}