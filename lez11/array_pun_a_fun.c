#include <stdio.h>

int sum(int, int);
int mul(int, int);

int main(){

    int (*f[2]) (int, int) = {sum, mul};

    printf("3+6=%d\n", (*f[0])(3,6));

    printf("3*6=%d\n", (*f[1])(3,6));

    return 0;
}

int sum(int x, int y){
    return x+y;
}

int mul(int x, int y){
    return x*y;
}