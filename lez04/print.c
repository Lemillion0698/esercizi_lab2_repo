#include <stdio.h>

#define LEN 20

struct Studente {
    char nome[LEN];
    int eta;
};

int main(){

    struct Studente s = {"Antonio", 15};

    printf("Nome4: %s, Età: %d\n", s.nome, s.eta);
    printf("Dimensione in memoria: %lu\n", sizeof(s));
    
    return 0;
}