#include <stdio.h>
#include "max.h"

//extern int countA;

int main(){
    int a=5, b=10;

    do{ 
    scanf("%d %d",&a, &b);
    printf("Max(%d,%d)=%d\n",a,b,max(a, b));
    } while (a!=b);
    
    //printf("Hai inserito a>b %d volte\n", countA);

    return 0;
}