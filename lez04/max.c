#include <stdio.h>

int max(int, int);

int main(){

    int a=5, b=10;

    printf("%d\n", max(a,b));
    return 0;
}

int max(int a, int b){
    if(a>b)
        return a;
    return b;
} 