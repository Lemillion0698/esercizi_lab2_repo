#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>   /* Per mmap, PROT_READ, MAP_SHARED, MAP_ANONYMOUS */
#include <semaphore.h>  /* Per sem_t, sem_init, sem_wait, sem_post, sem_destroy */
#include <sys/wait.h>   /* Per wait */

int main(int argc, char *argv[]) {
    // Evitiamo i warning del compilatore per parametri non utilizzati
    (void)argc;
    (void)argv;

    // Allocazione di una zona di memoria RAM condivisa e anonima
    sem_t *sem = mmap(NULL, sizeof(sem_t),
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    if (sem == MAP_FAILED) {
        perror("Errore mmap");
        exit(EXIT_FAILURE);
    }

    // Inizializzazione del semaforo anonimo:
    // - Secondo parametro = 1 (pshared): abilita la sincronizzazione TRA PROCESSI distinti
    // - Terzo parametro = 0: valore iniziale del semaforo (bloccante)
    if (sem_init(sem, 1, 0) == -1) {
        perror("Errore sem_init");
        munmap(sem, sizeof(sem_t));
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Errore fork");
        sem_destroy(sem);
        munmap(sem, sizeof(sem_t));
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { 
        // --- PROCESSO FIGLIO ---
        printf("[Figlio] Sto simulando un'operazione di 2 secondi...\n");
        sleep(2); 
        
        printf("[Figlio] Operazione conclusa. Incremento il semaforo (sem_post)...\n");
        sem_post(sem); 
        
        exit(0); 
    }

    // --- PROCESSO PADRE ---
    printf("[Padre] Mi metto in attesa sul semaforo (sem_wait)...\n");
    sem_wait(sem); // Il padre si blocca qui finché il figlio non esegue sem_post
    
    printf("[Padre] Svegliato! Il figlio ha terminato il suo compito.\n");
    
    // Attendo la terminazione formale del processo figlio per evitare processi zombie
    wait(NULL);
    
    // De-allocazione e pulizia delle risorse
    sem_destroy(sem);
    munmap(sem, sizeof(sem_t));
    
    printf("[Padre] Memoria e semaforo distrutti. Fine del programma.\n");
    exit(0);
}