#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

#define N 5 

int recupera_id(void* arg){
    int ID = *(int*) arg; // Recupera l'indice (0, 1, 2, 3, 4)
    thrd_t my_real_id = thrd_current(); // Recupera il vero ID di sistema del thread
    
    printf("Thread numero %d partito! Il mio ID reale di sistema è: %lu\n", ID, my_real_id);
    return thrd_success;
}

int main(){
    thrd_t threads[N]; 
    int ids[N]; 

    // Creazione dei thread
    for(int i=0; i<N; i++){
        ids[i] = i; // Passiamo semplicemente l'indice del ciclo (0, 1, 2, 3, 4)
        
        if(thrd_create(&threads[i], recupera_id, &ids[i]) != thrd_success){
            fprintf(stderr, "Thread T%d non creato\n", i);
            return EXIT_FAILURE;
        }
    }

    // Terminazione dei thread
    for(int i=0; i<N; i++){
        // SISTEMATE LE STAMPE INVERTITE
        if(thrd_join(threads[i], NULL) == thrd_success){
            printf("Il thread T%d è terminato correttamente\n", i); 
        } else {
            printf("Il thread T%d NON è terminato correttamente\n", i); 
        }
    }
    
    return EXIT_SUCCESS;
}