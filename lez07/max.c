#include <stdio.h>

int max(int a, int b){
    static int countA=0; 
    static int countB=0;

    if(a>b){
        countA++; 
        printf("countA=%d, countB=%d\n", countA, countB);
        return a;
    }
    countB++;
    printf("countA=%d, countB=%d\n", countA, countB);
    return b;
}