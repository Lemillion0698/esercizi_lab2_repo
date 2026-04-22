#include <stdio.h>

size_t file_size(const char* path) {
    // 1. Apertura in modalità binaria per contare i byte esatti
    FILE* fp = fopen(path, "rb");
    if (fp == NULL) {
        perror("Errore apertura file");
        return (size_t)-1; // Valore convenzionale per errore
    }

    // 2. Posizionamento alla fine del file
    if (fseek(fp, -10, SEEK_END) != 0) {
        perror("Errore fseek");
        fclose(fp);
        return (size_t)-1;
    }

    // 3. Ottenimento della posizione corrente (che corrisponde alla dimensione)
    long size = ftell(fp);
    if (size == -1L) {
        perror("Errore ftell");
        fclose(fp);
        return (size_t)-1;
    }

    // 4. CHIUSURA del file prima di uscire
    if (fclose(fp) == EOF) {
        perror("Errore fclose");
    }

    return (size_t)size;
}
int main(){

    printf("%zu\n", file_size("successo.csv"));
// zu, perché size_t può essere più grande di un intero semplice su sistemi 
// a 64 bit
    return 0;
}