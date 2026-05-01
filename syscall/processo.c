#include <wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int pid;
	int status;
	pid=fork();

	switch (pid){
		case -1: perror("Errore fork");
			exit(1);
			break;
		case 0: //nel figlio, (1)
			printf("%d: eseguendo figlio\n",getpid());
			fprintf(stdout, "Processo %d, mio padre è %d\n", (int)getpid(), (int)getppid());
			exit(25); // quando exit ha parametro diverso da 1, non interrompe l'esecuzione del programma ?
			break;
		default: //pid = PID del padre ? perché la stampa a video inizia con questo campo, invece di quella del case 0 ? 
			printf("Sono nel campo default con pid = %d\n", pid);
			sleep(10);
			if(waitpid(pid,&status,0)==-1){
				perror("Errore nella wait.");
				exit(2);
			}
			if (WIFEXITED(status))
				printf("%d: il figlio ha finito con status %d\n",getpid(), WEXITSTATUS(status));
			break;
	}
	return EXIT_SUCCESS;

}
//(1) perché fa sia il caso in cui pid = 0 che quello di default ? pid ha due valori ?