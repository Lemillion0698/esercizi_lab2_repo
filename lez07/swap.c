#include <stdio.h>

void swap(int*, int*);

int main(void) {
    int a=5, b=10;

    printf("a=%d, b=%d\n", a, b);

    swap(&a, &b);

    printf("a=%d, b=%d\n", a, b);

    return 0;
}

void swap(int* x, int* y){
    int var = *x;
    *x = *y;
    *y = var;
}