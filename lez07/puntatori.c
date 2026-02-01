#include <stdio.h>
/*
int main(){

    double a=0;
    double *aPtr=&a, *aPtr1=&a;

    printf("Il valore di a è %lf, l'indirizzo di a è %p\n", a, &a);

    (*aPtr) = 10;
    printf("Il valore di a è %.3f, l'indirizzo di a è %p\n", a, &a);

    (*aPtr1)=20;
    printf("Il valore di a è %f, l'indirizzo di a è %p\n", a, &a);

    return 0;
}*/

int main(void){

    int a=15;
    double *aPtr = &a; // incompatibilità tra il tipo del puntatore (double*) ed
    //il tipo della variabile puntata (int)
    
    printf("Il valore di a è %d, l'indirizzo di a è %p\n", a, &a);

    printf("Il valore di aPtr è %p\n", aPtr);

    printf("Il valore memorizzato a %p è %f\n", aPtr, *aPtr);

    return 0;
}
