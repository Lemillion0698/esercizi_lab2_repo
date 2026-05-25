#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>           /* Per le costanti O_* */
#include <sys/stat.h>        /* Per le costanti di modalità S_* */
#include <semaphore.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Controllo che sia stato passato il nome del semaforo
    if (argc < 2) {
        fprintf(stderr, "Uso: %s /nome_semaforo\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* argv[1] nome del semaforo */
    const char *name = argv[1];
    sem_t *sem;

    // CORREZIONE: La costante POSIX standard è O_CREAT, non O_CREATE
    sem = sem_open(name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR, 0);
    if (sem == SEM_FAILED) {
        perror("Errore sem_open");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Errore fork");
        sem_close(sem);
        sem_unlink(name);
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { 
        // --- PROCESSO FIGLIO ---
        printf("[Figlio] Sto simulando un lavoro di 2 secondi...\n");
        sleep(2); 
        
        printf("[Figlio] Lavoro terminato. Invio la sem_post()...\n");
        sem_post(sem); 
        
        // Il figlio chiude il suo descrittore prima di uscire
        sem_close(sem); 
        exit(0); 
    }

    // --- PROCESSO PADRE ---
    printf("[Padre] Entro in sem_wait() e mi metto in attesa del figlio...\n");
    sem_wait(sem); // Attendo la V (post) del figlio
    
    printf("[Padre] Sbloccato! Il figlio ha completato il suo compito.\n");

    // Chiusura del semaforo per il padre
    sem_close(sem);
    
    // Attendo che il processo figlio sia terminato del tutto (evita zombie)
    wait(NULL); 
    
    // Rimozione del semaforo dal sistema (fondamentale per non lasciare rimasugli nel SO)
    if (sem_unlink(name) == -1) {
        perror("Errore sem_unlink");
    } else {
        printf("[Padre] Semaforo rimosso dal sistema con successo.\n");
    }

    exit(0); 
}