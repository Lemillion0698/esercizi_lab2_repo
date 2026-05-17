#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // FONDAMENTALE per le macro O_WRONLY, O_CREAT, O_TRUNC

int main() {
    int fd;

    /* 1. APERTURA DEL FILE "pippo"
       - O_WRONLY: Apriamo il file in sola scrittura.
       - O_CREAT: Se il file non esiste, lo crea al momento.
       - O_TRUNC: Se il file esiste già, ne cancella il contenuto (lo svuota).
       - 0644: Permessi di lettura/scrittura per il proprietario, sola lettura per gli altri (rw-r--r--).
    */
    fd = open("pippo", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if (fd == -1) {
        perror("Errore nell'apertura del file pippo");
        return 1;
    }

    // Questa stringa finirà normalmente sul terminale
    printf("1. Questo messaggio viene stampato a SCHERMO.\n");

    /* 2. LA REDIREZIONE CON dup2
       Prendiamo il descrittore del file 'pippo' (fd) e lo copiamo sopra 
       lo Standard Output (STDOUT_FILENO, che corrisponde all'indice 1).
       Il kernel chiude il vecchio legame con lo schermo e lo sovrascrive.
    */
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("Errore nella dup2");
        close(fd);
        return 1;
    }

    /* 3. PULIZIA DESCRITTORE ORIGINALE
       Chiudiamo 'fd' perché la riga 1 della nostra tabella punta già a pippo.
       Tenere aperto fd creerebbe solo un duplicato inutile.
    */
    close(fd);

    /* 4. VERIFICA DELLA REDIREZIONE
       Da questo punto in poi lo schermo è disconnesso. Qualsiasi funzione 
       che punta a stdout scriverà silenziosamente nel file 'pippo'.
    */
    printf("2. Questo messaggio finirà dentro il file 'pippo'!\n");
    printf("3. Anche questa riga viene catturata dal file.\n");
    
    
    // Funziona anche con la write esplicita sul descrittore 1
    write(STDOUT_FILENO, "4. Scrittura diretta tramite write(1, ...)\n", 43);

    return 0;
}