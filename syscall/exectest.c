#include <stdio.h>
#include <unistd.h>

int main(void) {
    /* Stampa la prima parte della frase senza andare a capo (nessun \n) */
    printf("The quick brown fox jumped over ");
    
    /* Forza lo svuotamento del buffer di stampa prima che avvenga la sostituzione */
    fflush(stdout); // senza questa chiamata, la printf rimarrebbe bloccata nel buffer e verrebbe spazzata via (distrutta) dalla execl prima di poter essere stampata a schermo
    
    /* Sostituisce lo spazio di indirizzamento corrente con l'eseguibile /bin/echo */
    execl("/bin/echo", "Mbayanga", "the", "lazy", "dogs", (char*)NULL);  // primo argomento è il file eseguibile da caricare sul disco: "/bin/echo"
    // "echo" è il nome del nuovo programma (che decido io di dare) lanciato dal programma sorgente (exectest.c) dopo la  execl
    /* Se il programma arriva a leggere questa riga, significa che execl ha fallito */
    perror("execl"); 
    return 1; // perché se non riesce la exec*() a lanciare un nuovo programma, segnalerebbe un errore
}