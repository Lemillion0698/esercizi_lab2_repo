#include <stdio.h>
#include <unistd.h>

int main(){
    int pid;
    printf("Inizio\n");
    pid = fork();
    printf("%d: Ho ricevuto: %d\n", getpid(), pid);
    //printf("%d: PID padre è: %d\n", getpid(), getppid()); // il processo padre ha anch'esso stesso il suo padre
    return 0;
}