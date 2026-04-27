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
		case 0: //nel figlio, (1)
			printf("%d: eseguendo figlio\n",getpid());
			exit(25);
		default: //nel padre
			printf("Sono nel campo default\n");
			sleep(10);
			if(waitpid(pid,&status,0)==-1){
				perror("Errore nella wait.");
				exit(2);
			}
			if (WIFEXITED(status))
				printf("%d: il figlio ha finito con status %d\n",getpid(), WEXITSTATUS(status));
	}

}
//(1) perché fa sia il caso in cui pid = 0 che quello di default ? pid ha due valori ?