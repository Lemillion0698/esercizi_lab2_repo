#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h> /* Necessaria per read(), write() e close() */

#define N 1000

int main(){

    int fd, lung;   /* fd, n byte letti */
    char buf[N];    /* dove salvare i dati */
    
    /* Tento di aprire in sola lettura */
    if ( (fd = open("prova.c", O_RDONLY)) == -1){
        perror("prova.c, in apertura");
        exit(EXIT_FAILURE);
    }
    
    /* file aperto OK */
    /* Legge fino a N byte alla volta, finché ci sono dati */
    while ((lung = read(fd, buf, N)) > 0){
        /* Scrivo i byte letti sullo standard output (file descriptor 1) */
        if (write(1, buf, lung) == -1) {
            perror("prova.c: errore in scrittura");
            exit(EXIT_FAILURE); 
        }
    }
    printf("\n");
    /* Controllo se il ciclo è terminato a causa di un errore di lettura */
    if (lung == -1){
        perror("prova.c: errore in lettura"); 
        exit(EXIT_FAILURE);
    }

    /* Chiusura del file descriptor */
    if (close(fd) == -1) {
        perror("prova.c: errore in chiusura"); 
        exit(EXIT_FAILURE);
    }
    printf("\nQuello che c'è nel buffer: \n%s\n", buf);

    return EXIT_SUCCESS;
}