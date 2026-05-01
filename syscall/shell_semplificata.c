#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define MAXARG 64
#define TRUE 1

/* Inizializza la shell con un messaggio di benvenuto */
void inizializza(void) {
    printf("Benvenuto nella shell semplificata!\n");
    printf("Scrivi un comando (es. 'ls -l', 'date') o 'exit' per uscire.\n");
}

/* Stampa il prompt per l'utente */
void type_prompt(void) {
    printf("myshell> ");
    fflush(stdout);
}

/* Controlla se il comando inserito è quello di uscita */
int cmdexit(int argc, char *argv[]) {
    if (argc > 0 && strcmp(argv[0], "exit") == 0) {
        return 1;
    }
    return 0;
}

/* Legge l'input dell'utente e lo scompone in argomenti */
int read_cmd_line(int *argc, char* argv[], int maxarg) {
    static char buffer[1024];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("\n");
        exit(EXIT_SUCCESS);
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    *argc = 0;
    char *token = strtok(buffer, " ");
    
    while (token != NULL && *argc < maxarg - 1) {
        argv[*argc] = token;
        (*argc)++;
        token = strtok(NULL, " ");
    }
    
    argv[*argc] = NULL; 

    if (*argc == 0) {
        return -1; 
    }
    
    return 0;
}

/* Decodifica e stampa lo stato di terminazione del processo figlio */
static void print_status(pid_t pid, int status) {
    if (pid != 0) {
        printf("Process %d ", (int)pid);
    }
    
    if (WIFEXITED(status)) {
        printf("Exit value: %d ", WEXITSTATUS(status));
    }
        
    if (WIFSIGNALED(status)) {
        printf("Killed signal: %d ", WTERMSIG(status));
    }
        
    #ifdef WCOREDUMP
    if (WCOREDUMP(status)) {
        printf("-- core dumped ");
    }
    #endif
    
    if (WIFSTOPPED(status)) {
        printf("stopped");
    }
        
    printf("\n");
}

/* Crea il processo figlio ed esegue il comando richiesto */
static void execute(int argc, char* argv[]) {
    pid_t pid;
    int status;
    
    switch(pid = fork()) {
        case -1:
            perror("Cannot fork"); 
            break;
            
        case 0:
            execvp(argv[0], argv);
            perror("Cannot exec"); 
            exit(EXIT_FAILURE);
            
        default:
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid:"); 
                exit(EXIT_FAILURE);
            }
            print_status(pid, status);
    }
}

/* Funzione principale */
int main(void) {
    char *argv[MAXARG];
    int argc;
    
    inizializza();
    
    while (TRUE) {
        type_prompt();
        
        if (read_cmd_line(&argc, argv, MAXARG) != -1) {
            
            if (cmdexit(argc, argv)) {
                exit(EXIT_SUCCESS);
            }
                
            execute(argc, argv);
        }
    }
    
    return 0;
}