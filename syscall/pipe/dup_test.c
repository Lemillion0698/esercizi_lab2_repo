#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void sperimenta_dup_tradizionale(int pipe_write_fd);
void sperimenta_dup2(int pipe_write_fd);

int main() {
    int pfd[2];
    pid_t pid;

    if (pipe(pfd) == -1) {
        perror("Errore creazione pipe");
        return 1;
    }

    pid = fork();
    if (pid < 0) {
        perror("Errore fork");
        return 1;
    }

    if (pid == 0) {
        /* --- PROCESSO FIGLIO (Scrittore con Redirezione) --- */
        close(pfd[0]); // Chiude la lettura che non serve

        printf("[FIGLIO]: Questa printf va ancora a SCHERMO.\n");

        // ---- SCEGLI QUALE METODO SPERIMENTARE (Decommenta uno dei due) ----
        
        sperimenta_dup2(pfd[1]);
        // sperimenta_dup_tradizionale(pfd[1]);
        
        // ------------------------------------------------------------------

        /* Da questo momento in poi, lo Standard Output (1) è collegato alla pipe!
           Qualsiasi funzione che scrive su stdout finirà dritta nel tubo. */
        
        printf("Stringa inviata tramite printf grazie alla redirezione!\n");
        printf("PID del figlio: %d\n", getpid());

        /* Nota: è importante chiudere il descrittore originale dopo la duplicazione,
           altrimenti il kernel terrà il canale di scrittura aperto due volte. */
        close(pfd[1]); 
        exit(0);
    } 
    else {
        /* --- PROCESSO PADRE (Lettore Standard) --- */
        close(pfd[1]); // Chiude la scrittura

        char buffer[256];
        int bytes_read;

        printf("[PADRE]: In attesa di dati dalla pipe...\n\n");

        // Il padre legge normalmente dalla pipe ciò che il figlio ha reindirizzato
        while ((bytes_read = read(pfd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            printf("[PADRE RICEVE]:\n%s", buffer);
        }

        close(pfd[0]);
        wait(NULL);
        printf("\n[PADRE]: Figlio terminato. Fine test.\n");
    }

    return 0;
}

/* Metodo 1: dup2(oldfd, newfd) - Il metodo moderno e atomico */
void sperimenta_dup2(int pipe_write_fd) {
    /* dup2() prende il descrittore della pipe e lo copia ESATTAMENTE sul descrittore 1 (STDOUT_FILENO).
       Se il descrittore 1 era già aperto (ed lo è, perché punta allo schermo), dup2 lo chiude 
       automaticamente prima di fare la copia, in modo totalmente sicuro (atomico). */
    dup2(pipe_write_fd, STDOUT_FILENO);
}

/* Metodo 2: Il metodo classico in due passaggi con dup() */
void sperimenta_dup_tradizionale(int pipe_write_fd) {
    /* dup() duplica un descrittore assegnandogli il MINIMO indice disponibile nella tabella.
       Per forzarlo ad usare il descrittore 1 (stdout), dobbiamo prima liberarlo a mano. */
    close(STDOUT_FILENO); // 1) Libera il posto numero 1
    dup(pipe_write_fd);   // 2) dup() vede che il posto 1 è libero e lo occupa!
}