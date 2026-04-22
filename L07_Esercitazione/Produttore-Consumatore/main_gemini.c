#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

#define Buffer_circolare 10
#define N_INCREMENTS 3
#define N_THREADS 2

typedef struct{
    int buffer;
    int indice_inserimento; // Mantenuto come da traccia
    int indice_estrazione;  // Mantenuto come da traccia
    mtx_t lock;
} SharedData;

int produttore(void* arg){
    SharedData* data = (SharedData*) arg; // CORREZIONE 1: Usa il puntatore!

    // Inizializzo l'indice. Non uso il mutex qui perché il consumatore non lo legge
    data->indice_inserimento = 1; 

    for(int i = 0; i < N_INCREMENTS; i++){
        // Devo proteggere il buffer condiviso
        mtx_lock(&(data->lock));
        
        // Simulo l'attesa se il buffer ha raggiunto il limite (versione semplificata senza sleep complicate)
        if(data->buffer < Buffer_circolare) {
            data->buffer += data->indice_inserimento;
            printf("Produttore: Buffer aumentato a %d\n", data->buffer);
            data->indice_inserimento++; // Incremento l'indice per la prossima volta
        }
        
        mtx_unlock(&(data->lock));
        
        // Pausa fuori dal mutex
        thrd_sleep(&(struct timespec){.tv_sec = 0, .tv_nsec = 500000000}, NULL);
    }
    return thrd_success;
}

int consumatore(void* arg){
    SharedData* data = (SharedData* )arg; // CORREZIONE 1: Usa il puntatore!
    
    for(int i = 0; i < N_INCREMENTS; i++) {
        // La scanf blocca il programma. Va fatta FUORI dal mutex, 
        // altrimenti il produttore resterebbe bloccato in attesa dell'utente!
        printf("\nInserisci un nuovo numero da sottrarre: ");
        fflush(stdout);
        scanf("%d", &(data->indice_estrazione)); // Salvo nel campo della struct
        
        mtx_lock(&(data->lock));
        
        // Opero sul buffer usando il valore appena acquisito
        if (data->buffer >= data->indice_estrazione) {
            data->buffer -= data->indice_estrazione;
            printf("Consumatore: Buffer ridotto a %d\n", data->buffer);
        } else {
            printf("Consumatore: Valore troppo alto, ignoro.\n");
        }

        mtx_unlock(&(data->lock));
    }
    return thrd_success;
}

int main() {
    thrd_t ids[N_THREADS];
    SharedData dati_condivisi; // CORREZIONE 2: Una sola struct per tutti

    dati_condivisi.buffer = 0;
    
    // CORREZIONE 3: Inizializzazione del mutex
    if(mtx_init(&(dati_condivisi.lock), mtx_plain) != thrd_success) {
        return EXIT_FAILURE;
    }

    // Passo L'INDIRIZZO della stessa struct a entrambi i thread
    thrd_create(&ids[0], produttore, &dati_condivisi);
    thrd_create(&ids[1], consumatore, &dati_condivisi);

    // CORREZIONE 4: Join corretta
    thrd_join(ids[0], NULL);
    thrd_join(ids[1], NULL);

    mtx_destroy(&(dati_condivisi.lock));

    return EXIT_SUCCESS;
}