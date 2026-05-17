#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h> // <--- FONDAMENTALE per _POSIX_PIPE_BUF

//#define _POSIX_PIPE_BUF 512

int main(void) {
    int pfd[2];
    long int v;
    long int v1;

    // Creazione pipe 
    if (pipe(pfd) == -1) {
        perror("Errore pipe");
        return 1;
    }

    // Usiamo il nome standard POSIX con l'underscore 
    // Se il tuo compilatore ancora non lo vede, usa direttamente il valore 512
    #ifdef _POSIX_PIPE_BUF
        printf("Limite minimo POSIX (_POSIX_PIPE_BUF) = %d byte\n", _POSIX_PIPE_BUF);
    #else
        printf("Macro _POSIX_PIPE_BUF non definita (Standard POSIX: 512)\n");
    #endif

    // Verifica capacità reale del sistema 
    errno = 0;
    v = fpathconf(pfd[0], _PC_PIPE_BUF); // _PC_PIPE_BUF è il selettore, non il valore!
    v1 = fpathconf(pfd[1], _PC_PIPE_BUF);//perché sia col passare sia il descrittore pfd[0] che pfd[1], ottengo la stessa capacità reale della pipe?(1)

    if (v == -1) {
        if (errno != 0) { // errno è stato modificato ? (2)
            perror("Errore fpathconf");
        } else {
            printf("Capacità reale = illimitata\n");
        }
    } else {
        printf("Capacità reale della pipe su questo sistema = %ld byte\n", v);
    }
//(2) sì, perché anché in caso di successo della fpathconf, potrebbe restituire -1, quindi l'unico modo per sapere se effettivamente c'è stato un errore
// in fase di compilazione è di verifare che errno (che prima, per sicurezza abbiamo azzerato) è stata modificata.
    printf ("v1 = %ld\n", v1);

    close(pfd[0]);
    close(pfd[1]);
    return 0;
}

/*(1) la pipe è un unico oggetto (un buffer) nel kernel.

Ecco i motivi tecnici:

Un solo buffer condiviso: Quando chiami pipe(pfd), il kernel non crea due canali separati, ma alloca un unico buffer di memoria. I descrittori pfd[0] 
e pfd[1] sono semplicemente due "porte" di accesso diverse allo stesso identico oggetto.

Proprietà del canale, non del descrittore: La "capacità della pipe" (_PC_PIPE_BUF) è una proprietà del file speciale (la pipe) a cui i descrittori 
puntano. Poiché entrambi i descrittori puntano alla stessa risorsa, interrogarne uno o l'altro restituisce necessariamente lo stesso limite di sistema.
*/