#include <stdio.h>
#include <stdlib.h>

/* Definizione del tipo enumerativo per i ruoli */
typedef enum {
    dirigente,
    quadro,
    operaio,
    impiegato
} ruolo_t;

/* Definizione della struttura dipendente */
typedef struct {
    int id;
    int eta;
    float salario;
    ruolo_t ruolo;
} dipendente_t;

int main(int argc, char *argv[]) {
    int i, id_dip, eta_dip, ruolo_dip, eta;
    float media = 0, salario_dip;
    dipendente_t dipendenti[6]; // Il numero di dipendenti è fissato a 6 [1]

    /* Lettura dell'età filtro per la stampa finale */
    scanf("%d", &eta);

    /* Ciclo di inserimento per 6 dipendenti */
    for (i = 0; i < 6; i++) {
        int trovato;
        do {
            trovato = 0;
            scanf("%d", &id_dip);
            
            /* CONTROLLO UNIVOCITÀ ID: scorre gli ID già inseriti (da 0 a i-1) */
            for (int j = 0; j < i; j++) {
                if (id_dip == dipendenti[j].id) {
                    printf("Id %d del dipendente esistente, reinserirlo!\n", id_dip);
                    trovato = 1;
                    break; // Esce dal for di ricerca [2]
                }
            }

            /* Se l'ID è nuovo, procede con la lettura degli altri dati */
            if (!trovato) {
                scanf("%d", &eta_dip);
                scanf("%f", &salario_dip);
                scanf("%d", &ruolo_dip);
                
                dipendenti[i].id = id_dip;
                dipendenti[i].eta = eta_dip;
                dipendenti[i].salario = salario_dip;
                dipendenti[i].ruolo = ruolo_dip;
            }
        } while (trovato); // Se l'ID era già esistente, ripete la richiesta [2]

        /* Calcolo della somma degli stipendi per la media */
        media += salario_dip;
    }

    /* Calcolo della media aritmetica */
    media /= 6;
    printf("%d\n ", media);

    /* Stampa dei risultati filtrati */
    for (i = 0; i < 6; i++) {
        // Stampa solo se lo stipendio è inferiore alla media e l'età è <= al filtro
        if (dipendenti[i].salario < media && dipendenti[i].eta <= eta) {
            printf("%d ", dipendenti[i].id);
            printf("%d ", dipendenti[i].eta);
            
            /* Traduzione del ruolo (enum) in etichetta testuale */
            switch (dipendenti[i].ruolo) {
                case dirigente:
                    printf("dirigente\n");
                    break;
                case quadro:
                    printf("quadro\n");
                    break;
                case operaio:
                    printf("operaio\n");
                    break;
                default:
                    printf("impiegato\n");
            }
        }
    }
    return 0;
}