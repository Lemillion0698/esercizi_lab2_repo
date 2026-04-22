#include <stdio.h>
#include <threads.h>

// Step 2: Usiamo un intero atomico per rendere l'incremento indivisibile

int counter = 0; // variabile condivisa
mtx_t mutex; // mutex globale, perché ?
/*Il mutex è dichiarato fuori dal main per un motivo di visibilità:
    Tutti i thread devono poter accedere allo stesso identico oggetto mutex per coordinarsi.
    Se lo dichiarassi locale al main, dovresti passarlo come argomento alla funzione incrementa_counter tramite un puntatore (usando il parametro void* arg).
    Dichiarandolo globale, la funzione incrementa_counter lo vede direttamente. È la soluzione più semplice per questo tipo di esercizi scolastici.
*/
#define T 5
#define K 100000

int incrementa_counter(void* arg){
    for(int i=0; i<K; i++){
        //  INIZIO SEZIONE CRITICA
        mtx_lock(&mutex); // il thread i thenta di acquisire la chiave(lock),
        // se non ce l'ha già un altro thread, la acquisisce con successo
        counter++; // Solo un thread alla volta esegue questo incremento
        mtx_unlock(&mutex); // il thread i restituisce la chiave
        //  FINE SEZIONE CRITICA
    }
    return thrd_success; // è obbligatoria, questa riga ? perché non un return 1 ?
    //thrd_success: È una costante definita in <threads.h> (solitamente vale 0)
}

int main(){
    thrd_t threads[T]; // array di descrittori di threads

    // Inizializzazione del mutex
    if(mtx_init(&mutex, mtx_plain) != thrd_success){
        fprintf(stderr, "Errore di inizializzazione del mutex\n");
        return 1;
    }

    // Creazione dei threads
    for(int i=0; i<T; i++){
        if (thrd_create(&threads[i], incrementa_counter, NULL) != thrd_success) {
            fprintf(stderr, "Errore creazione thread %d\n", i);
            return 1;
        }
    }

    // Attesa e terminazione
    for(int j=0; j<T; j++){
        if (thrd_join(threads[j], NULL) == thrd_success) {
            printf("Thread %d terminato.\n", j);
        }
    }

    // Risultati
    printf("\nRisultato finale: %d\n", counter);
    printf("Risultato atteso: %d\n", T * K);
    
    if (counter != T * K) {
        printf("DATA RACE RILEVATA!\n");
    } else {
        printf("Risultato CORRETTO grazie al mutex.\n");
    }

    // Distruzione del mutex
    mtx_destroy(&mutex);

    return 0;
}