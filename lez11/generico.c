#include <stdio.h>
#include <stdlib.h>

int compInt(const void* a, const void* b){
    int* ac=(int*)a;
    int* bc=(int*)b;

    return *ac - *bc;
}

int main(){

    int a[5] = {17, 9, 2, 43, 0};

    qsort(a, 5, sizeof(int), compInt);

    for(int i=0; i<5; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
    
    return 0;
}