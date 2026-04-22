#include <stdio.h>

int sum(int, int);
int mul(int, int);

int main(){

    int (*f)(int, int);

    f=sum;
    printf("3+6=%d\n", (*f)(3,6));

    f=mul;
    printf("3*6=%d\n", (*f)(3,6));
    return 0;
}

int sum(int x, int y){
    return x+y;
}

int mul(int x, int y){
    return x*y;
}