#include <stdio.h>

/*typedef struct dip{
    int codice;
    int eta;
    float stipendio;
    enum tipo = {dirigente, quadro, operaio, impiegato};
} Dipendent;

int main (){

    int N=6;
    Dipendenti  dipendente[N]; // array di 6 dipententi

    // inserimento dei dipendenti

    for (int i=0; i<N; i++){
        sc
    }

    return 0;
}*/

typedef enum {
    dirigente,
    quadro,
    operaio,
    impiegato
} ruolo_t;

typedef struct
{
    int id;
    int eta; 
    float salario;
    ruolo_t ruolo;
} dipendente_t;

int main(){
    int i, id_dip, eta_dip, ruolo_dip, eta;
    float media = 0, salario_dip;

    dipendente_t dipendenti[6];
    scanf("%d", &eta);

    for(i=0; i<6; i++){
        scanf("%d", &id_dip);
        scanf("%d", &eta_dip);
        scanf("%f", &salario_dip);
        scanf("%d", &ruolo_dip);
        dipendenti[i].id = id_dip;
        dipendenti[i].eta = eta_dip;
        dipendenti[i].salario = salario_dip;
        dipendenti[i].ruolo = ruolo_dip;

        /* calcolo della media. fatto durante l'inserimento */
        media  += salario_dip;
    }
    media /= 6;

    for(i=0; i<6; i++){
        if((dipendenti[i].salario) < media && (dipendenti[i].eta <= eta)) {
            printf("%d ", dipendenti[i].id);
            printf("%d ", dipendenti[i].eta);

            switch(dipendenti[i].ruolo){
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