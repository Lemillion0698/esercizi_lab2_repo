#include <stdio.h>
#include <stdlib.h>

#define N 6

typedef struct {
    int elements[N];
    int n;
} Stack;


int main(){
    int a=10;
    void* aP = &a;
    Stack* s = (Stack*)malloc(sizeof(Stack));

    printf("%d\n", *(int*)aP);

    return 0;
}