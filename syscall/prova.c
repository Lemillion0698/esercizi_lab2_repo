#include<stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

// CONTROLLA -1; stampa errore e termina
#define ec_meno1(s, m) \
    if ((s) == -1) {perror(m); exit(EXIT_FAILURE);}

// controlla NULL; stampa errore e termina (NULL)
#define ec_null(s,m) \
    if((s)==NULL) {perror(m); exit(EXIT_FAILURE);}

// controlla -1; stampa errore ed esegue c
#define ec_meno1_c(s, m, c)\
    if((s)==-1) {perror(m); c;}

int main(){
    int fd;
    /*tento di aprire in sola lettura*/
    if(( fd = open("studio_processi.md", O_EXCL)) == -1){
        perror("studio_processi.md, in apertura");
        exit(EXIT_FAILURE); // termina
    }

    printf("fd = %d\n", fd);

    return EXIT_SUCCESS;
}