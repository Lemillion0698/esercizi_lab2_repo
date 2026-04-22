#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

#define M_PRODUTTORI 5          // Numero di produttori
#define N_CONSOMATORI 10        // Numero di consumatori
#define K_BUFFER 5              // Dimensione del buffer, capacità massima di bibite che possa contenere il buffer
#define MEZZOSECONDO 5.0e8      // Mezzo secondo in nanosecondi

int buffer = 0;                 // Numero di bibite nel distributtore
mtx_t mutex;                    // Mutex per la mutua esclusione
cnd_t pieno, vuoto;             // Variabili di Condizione

int produttore(void* arg) {     // Funzione produttore
    int id = *(int *)arg;       // ID del produttore
    while (1){                  // In un ciclo infinito
        thrd_sleep(             // Simula il tempo di produzione
            &(struct timespec)
            {.tv_nsec = MEZZOSECONDO},
            NULL
        );

        mtx_lock(&mutex);       // Blocca il mutex
        while (buffer == K_BUFFER) // Se il buffer risulta pieno, aspetta che il un thread consumatore prelevi almeno una bibita ? sì
            cnd_wait(&pieno, &mutex); // Aspetta che il buffer si svuoti

        buffer++;                   // Aggiunge una bibita
        printf( "Produttore %d "   // Stampa azione a video
                "ha aggiunto una bibita. "
                "Totale: %d\n", id, buffer
        );

        cnd_broadcast(&vuoto);      // Sveglia TUTTI i consumatori
        mtx_unlock(&mutex);         // Sbocca il mutex
    }
}

int consumatore(void* arg){     // Funzione consumatore
    int id = *(int* )arg;       // ID del consumatore
    while (1) {                 

            thrd_sleep(
                &(struct timespec)
                {.tv_nsec = 1},
                NULL
            );

            mtx_lock(&mutex);       // Blocca il mutex
            while (buffer == 0)     // Se il buffer risulta vuoto, aspetta che un thread produttore aggiunga almeno una bibita ? sì
                cnd_wait(&vuoto, &mutex); // Aspetta che il buffer si riempia
            
            buffer--;
            printf(  "Condumatore %d "      // Stampa azione a video
                     "ha preso una bibita. "
                     "Totale: %d\n", id, buffer
            );

            cnd_broadcast(&pieno);      // Sveglia tutti i produttori
            mtx_unlock(&mutex);         // Sblocca il mutex
    }
}


int main(){
    srand(time(NULL));

    thrd_t produttori[M_PRODUTTORI];
    thrd_t consumatori[N_CONSOMATORI];
    int id_prod[M_PRODUTTORI], id_cons[N_CONSOMATORI];

    mtx_init(&mutex, mtx_plain);
    cnd_init(&pieno);
    cnd_init(&vuoto);

    // Creazione produttori
    for (int i=0; i < M_PRODUTTORI; i++){
        id_prod[i] = i + 1;
        thrd_create(&produttori[i], produttore, &id_prod[i]);
    }

    // Creazione consumatori
    for (int i = 0; i < N_CONSOMATORI; i++){
        id_cons[i] = i + 1;
        thrd_create(&consumatori[i], consumatore, &id_cons[i]);
    }

    // Attende i thread (in un' app reale, servirebbe una condizione di terminazione)
    for (int i=0; i < M_PRODUTTORI; i++) {
        thrd_join(produttori[i], NULL);
    }
    for(int i=0; i < N_CONSOMATORI; i++){
        thrd_join(consumatori[i], NULL);
    }

    mtx_destroy(&mutex);
    cnd_destroy(&pieno);
    cnd_destroy(&vuoto);
    
    return 0;
}

