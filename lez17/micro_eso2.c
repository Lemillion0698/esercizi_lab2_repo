#include <stdio.h>
#include <string.h>

void filtro_sottostringa(const char* str) {
    char buf; // Dimensione del buffer costante [6]

    // Legge da stdin riga per riga fino a EOF [2, 3]
    while (fgets(buf, sizeof buf, stdin)) {
        
        // Verifica se la riga contiene la sottostringa [5]
        if (strstr(buf, str) != NULL) {
            // Scrive la riga intera su stdout [3]
            fputs(buf, stdout); 
        }
    }
    
    // Verifica se il ciclo è terminato per un errore di lettura [7]
    if (ferror(stdin)) {
        perror("Errore durante la lettura da stdin");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <sottostringa>\n", argv);
        return 1; // Ritorno di errore nel main [3]
    }

    filtro_sottostringa(argv[8]);
    return 0;
}