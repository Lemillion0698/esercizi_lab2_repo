#include <stdio.h>

int main() {
    int n;

    // Leggi la dimensione dei vettori
    printf("Inserisci la dimensione dei vettori: ");
    scanf("%d", &n);

    int vett1[n], vett2[n];
    int result = 0;

    // Leggi i valori del primo vettore
    printf("Inserisci %d elementi per il primo vettore:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &vett1[i]);
    }

    // Leggi i valori del secondo vettore
    printf("Inserisci %d elementi per il secondo vettore:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &vett2[i]);
    }

    // Calcola il prodotto scalare
    for (int i = 0; i < n; i++) {
        result += vett1[i] * vett2[i];
    }

    printf("Il prodotto scalare è: %d\n", result);

    return 0;
}
