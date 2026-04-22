#include <stdio.h>
#include <string.h>

int main(){
// NB: è necessario che s sia un array di caratteri sennò non può essere
// modificata
    char s[100], *tok; // dichiaro un array di caratteri s e un puntatore a carattere tok
    //tok verrà utilizzato per memorizza l'indirizzo di ogni token estratto della stringa
    scanf("%[^\n]", s);
//  %[^\n] istruisce scanf a leggere l'intera riga, spazi inclusi, fermandosi
// solo quando incontra il carattere di nuova riga (\n)
    tok = strtok(s, ","); // strtok cerca nella stringa s il primo delimitatore
// (la virgola ,). Quando lo trova,lo sostituisce con un carattere null (\0)
// per "isolare" il primo token e restituisce il puntatore all'inizio di tale
// token 

    printf("%s\n", tok); 

    while((tok = strtok(NULL, ",")) != NULL)// serve a recuperare tutti i token successivi al primo
        printf("%s\n", tok);// stampa ogni token successivo al primo trovato
//NB: Nelle chiamate successive alla prima, il primo argomento di strtok deve
// essere NULL. Questo segnale indica alla funzione di continuare l'analisi 
//della stringa originale dal punto in cui si era interrotta precedentemente
    printf("la stringa modificata è: %s\n", s);
    printf("l'indirizzo a cui punta è :%p\n", s);

    return 0;
}