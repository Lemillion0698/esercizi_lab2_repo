#include <stdio.h>
#include <stdlib.h>
#include <threads.h>


#define N 100000 // dimensione dell'array da "partizionare"
#define T 4 // numero di threads
 
// Struttura per definire il "chunck" di lavoro di ogni thread
typedef struct
{
    int start;
    int end;
    long* result_cell; // puntatore alla cella dedicata in partial_sums
} thread_data_t;


long arr[N]; // array da "partizionare"

/*int sommapartial(void* arg){ // sbalgliato perché ogni thread sommerà la stessa porzione dell'array
    int dim = *(int* )arg;
    int result=0;
    for(int i=0; i<dim; i++){
        result += arr[i];
    }
    return result;
}*/

int sommapartial(void* arg){ // quello che fa ogni thread
    thread_data_t* d = (thread_data_t* )arg;
    long local_sum = 0;

    // Ogni thread lavora solo sul suo intervallo [start, end[
    for(int i = d->start; i < d->end; i++){
        local_sum += arr[i];
    }

    // Scrittura nella memoria dedicata: nessuna race condition !
    *(d->result_cell) = local_sum; 

    return thrd_success;
}

int main(){
    // Inizializzazione array
    for(int i=0; i<N; i++)
        arr[i]=2.0;

    thrd_t ids[T]; // array di identificatori di threads
    thread_data_t t_args[T]; // array usato da ogni thread per sapere su quali 
    // dati lavorare senza interferire sui quelli di altri threads; 
    long partial_sums[T]; // per ricevere il risultato delle somme partiali  

    int chunk_size = N / T; // dimensione di ogni blocco dell'array arr

    // creazione dei threads
    for(int i=0; i<T; i++){
        t_args[i].start = i * chunk_size;
        // Gestione dell'ultimo chunk (nel caso N non sia divisibile per T)
        t_args[i].end = (i == T - 1) ? N : (i + 1) * chunk_size; 
        t_args[i].result_cell = &partial_sums[i];

        if(thrd_create(&ids[i], sommapartial, &t_args[i]) != thrd_success){
            fprintf(stderr, "La creazione del thread %d non è avvenuta con successo\n", i);
            return EXIT_FAILURE;
        }
    }
    
    // terminazione dei threads
    for(int i=0; i<T; i++){

        if(thrd_join(ids[i], NULL) != thrd_success){
            fprintf(stderr, "La terminazione del thread %d non è avvenuta con successo\n", i);
            return EXIT_FAILURE;
        }
    }

    // somma dei 4 valori contenuti in partial_sums
    long valore_finale=0;
    for(int i=0; i<T; i++){
        valore_finale += partial_sums[i];
    }

    printf("Il valore finale è: %ld\n", valore_finale);

    return 0;
}

