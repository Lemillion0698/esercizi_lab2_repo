#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

#define N 5 // numero di threads

int recupera_id(void* arg){
    thrd_t ID = *(thrd_t*) arg;
    thrd_t ID_cal = thrd_current();
    printf("ID ricevuto = %lu\n", ID);
    printf("ID_calcolato = %lu\n", ID_cal);
    return thrd_success;
}

int main(){

    thrd_t threads[N];//array "vuoto" (ci saranno dentro numeri indefiniti) 
    //di descrittori di threads ?
     
    /*for(int i=0; i<N; i++){
        printf("Il descrittore appena creato è %lu\n", threads[i]);
    }*/

    // creazione dei threads
    for(int i=0; i<N; i++){
        //ids[i]=thrd_current(); // sbagliato perché sto passando a tutte le celle
// dell'array ids l'ID del thread main che sta eseguendo la thrd_current (calcolato dal SO) 
        if(thrd_create(&threads[i], recupera_id, &threads[i]) != thrd_success){
        // passo come argomento piuttosto &threads[i] perché voglio vedere il descrittore del thread
        // creato al momento della dichiarazione di "threads[N]" ?
            fprintf(stderr, "Thread T%d non creato\n", i);
            return EXIT_FAILURE;
        }
    }

    // terminazione dei threads
    for(int i=0; i<N; i++){
        if(thrd_join(threads[i],NULL) != thrd_success){
            printf("Il thread T%d NON è terminato correttamente\n", i);
        } else {
            printf("Il thread T%d è terminato correttamente\n", i);
        }
    }
    
    return EXIT_SUCCESS;
}