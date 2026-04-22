#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include "lista.h"

// MACRO PER CONFIGURARE L'AMBIENTE
#define ITERAZIONI 3
#define NUMERO_PRODUTTORI 5
#define NUMERO_CONSUMATORI 5
#define SLOT_LISTA 2 // dimensione del buffer
#define DEBUG // (trucco)serve ad attivare o disattivare tutte le stampe del 
// programma in un solo colpo semplicemente commentando o rimuovendo questa riga
// sappiamo che nei programmi multithread, la printf è l'unico modo per sapere
// dei comportamenti dei threads 

lista_t lista; // lista condivisa globale
mtx_t mutex; // mutex globale
cnd_t lista_vuota;
cnd_t lista_piena;

int produttore(void* arg){
    int cont_produttore=0;
    do{
        mtx_lock(&mutex); // il produttore acquisisce il lock
        if (lista.numero_elementi < lista.capienza_massima)
        {
            push(&lista, 5);
            cont_produttore ++;
            if(lista.numero_elementi == 1){
                cnd_broadcast(&lista_vuota);
            } 
         /*if (lista.numero_elementi == lista.capienza_massima){
                mtx_lock(&mutex);// ERRORE FATALE: All'inizio del ciclo do-while avevo già acquisito il mutex. Se la lista è piena, provo ad acquisirlo di nuovo (mtx_lock(&mutex)).
                // In C11, i mutex di tipo mtx_plain non sono ricorsivi: se provo a bloccare un mutex che possiedo già, il thread si congela per sempre in attesa di se stesso 
                //(Deadlock).
                        cnd_broadcast(&lista_vuota);
                    }
                while (lista_piena != thrd_success){// ERRORE CONCETTUALE: lista_piena è una struttura dati (cnd_t), non puoi confrontarla con thrd_success. Il controllo corretto 
                    //per il while deve essere sullo stato della lista: while (lista.numero_elementi == lista.capienza_massima).
                    cnd_wait(&lista_piena, &mutex);
                }*/
        } else{
           while (lista.numero_elementi == lista.capienza_massima) {
					
			#ifdef DEBUG 
				printf("Lista piena\n");
			#endif
					
				cnd_wait(&lista_piena, &mutex);
			}
        }
        
        mtx_unlock(&mutex);
        
        } while (cont_produttore < ITERAZIONI);
    // Sempre nel produttore, se la lista non è piena, fai la push, fai i tuoi controlli if, ma alla fine del ciclo ti sei dimenticato di chiamare mtx_unlock(&mutex). 
    // Il primo produttore che entra si tiene il lock per sempre, e nessun altro thread (né produttore né consumatore) potrà mai fare nulla.
    return thrd_success;
}

int consumatore (void* arg){
    int cont_consumatore = 0;
    do{
        mtx_lock(&mutex);
        if(lista.numero_elementi > 0){ // SENZA una cnd_wait, il consumatore non 
// aspetta mai che un produttore crei un nuovo elemento nelle lista, avrà come
// conseguenza che un consumatore brucierà iterazioni in caso di lista vuota
            pop(&lista);

        } else {
				while (lista.numero_elementi == 0) {
		
					#ifdef DEBUG 
						printf("Lista vuota\n");
					#endif
		
					cnd_wait(&lista_vuota, &mutex);
				}
		    }
        mtx_unlock(&mutex);
        cont_consumatore++;
    } while (cont_consumatore < ITERAZIONI);
    return thrd_success;
}

int main(){

    // inizializzazione delle strutture di sincronizzazione
    mtx_init(&mutex, mtx_plain);
    cnd_init(&lista_vuota);
    cnd_init(&lista_vuota);
    inizializa_lista(&lista, SLOT_LISTA);

    thrd_t threads_produttori[NUMERO_PRODUTTORI];// array di threads produttori
    thrd_t threads_consumatori[NUMERO_CONSUMATORI]; // array di threads consumatori

    // creazione dei threads produttori e consumatori
    for(int i=0; i<NUMERO_CONSUMATORI; i++){
        if(thrd_create(&threads_consumatori[i], consumatore, NULL) != thrd_success){
            fprintf(stderr,"Il thread consumatore T%d non è stato creato con successo\n", i);
            return EXIT_FAILURE;
        }
    }
    for(int i=0; i<NUMERO_PRODUTTORI; i++){ 
        if(thrd_create(&threads_produttori[i], produttore, NULL) != thrd_success){
            fprintf(stderr,"Il thread produttore T%d NON è stato creato con successo\n", i);
            return EXIT_FAILURE;
        }
    }
    
    // terminazione dei threads produttori e consumatori
    for(int i=0; i<NUMERO_CONSUMATORI; i++){
        if(thrd_join(threads_consumatori[i], NULL) != thrd_success){
            fprintf(stderr,"Il thread consumatore T%d NON è stato terminato con successo\n", i);
            return EXIT_FAILURE;
        }
    }
    for(int i=0; i<NUMERO_PRODUTTORI; i++){
        if(thrd_join(threads_produttori[i], NULL) != thrd_success){
            fprintf(stderr,"Il thread produttore T%d NON è stato terminato con successo\n", i);
            return EXIT_FAILURE;
        }
    }

    // destruzione delle strutture di sincronizzazione
    mtx_destroy(&mutex);
    cnd_destroy(&lista_piena);
    cnd_destroy(&lista_vuota);

    return EXIT_SUCCESS;
}