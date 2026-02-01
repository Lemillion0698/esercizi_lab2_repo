#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){
    // Verifica che argc sia almeno 3 (nome_programma + param1 + param2)
    if (argc < 3) {
        printf("Errore: inserire due numeri come parametri.\n");
        return 1; // Termina con un codice di errore
    }

    printf("%d\n", atoi(argv[5]) + atoi(argv[6]));
    return 0;
}