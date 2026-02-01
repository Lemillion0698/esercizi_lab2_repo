#include <stdio.h>

int main(){
    int dim;
    scanf("%d", &dim);
    
    int arr[dim];

    for(int i=0; i<dim; i++){
        scanf("%d", &arr[i]);
    }

    printf("Array = [ ");
    for(int i=0; i<dim; i++){ 
        printf ("%d ", arr[i]);
    }
    printf("]\n");

    // inversione senza array di appoggio
    int tmp=0;
    for(int i=0; i<dim/2; i++){
        tmp = arr[i];
        arr[i] = arr[dim-1-i];
        arr[dim-1-i] = tmp;
    }

    printf("Array invertito = [ ");
    for(int i=0; i<dim; i++){ 
        printf ("%d ", arr[i]);
    }
    printf("]\n");

    return 0;

}