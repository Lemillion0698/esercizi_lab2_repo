#include <stdio.h>
int N=4;

int main(){
    float v[N];
    int dim=0, i;
    int swapped = 1;

    do {
        scanf("%f", &v[dim]);
        dim++;
    } while (v[dim-1] != 0);

    while(swapped){
        swapped = 0;
        for (i=0; i<dim; i++)
        {
            if(v[i] < v[i+1]){
                float aux = v[i];
                v[i] = v[i+1];
                v[i+1] = aux;
                swapped=1;
            }
        }
    }
    for(i=0; i<dim; i++)
        printf("%.2f ", v[i]);

    return 0;
}