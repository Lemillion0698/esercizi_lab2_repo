#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Ciao UNIPI! Mon ID de processus (PID) est : %d\n", getpid());
    return 0;
}