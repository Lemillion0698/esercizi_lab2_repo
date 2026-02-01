#include <stdio.h>
// CONVERSIONI FRA TIPI

/*int main() {
    int a=0; 
    double b=0;
    scanf("%d %lf",&a,&b);
    printf("a=%d\nb=%f\n", a, b);
    return 0;
}*/


/*int main() {
    int a=5;
    float b=a; // conversione implicita di a in float (upcast)
    printf("a=%d\nb=%f\n", a, b);
    return 0;
} 
*/

int main(){
    int a=5;
    float b = a/2; // divisione intera, il valore 2 viene poi convertito 
    //implicitamente in float durante l'assegnazione, ottengo b= 2.000000
    printf("a=%d\nb=%f\n", a, b);
    return 0;
}
/*int main(){
    int a = 5;
    float b = a / 2.0; // 2.0 per ottenere una divisione tra float
    printf("a=%d\nb=%f\n", a, b);
    return 0;
}
*/

/*int main(){
    int a=5;
    float b = (float)a/2; // conversione esplicita di a
    printf("a=%d\nb=%f\n", a, b);
    return 0;
}*/

// ARRAY MONODIMENSIONALI

/*int main() {
    int n=12345;
    int cifre[10]; 
    int dim=0;

    for(; n != 0; dim++, n /= 10){
        cifre[dim] = n % 10;
    }

    for(int i=0; i<dim; i++){
        printf("%d\n", cifre[i]);
    }

    return 0;
}*/

// ARRAY BIDIMENSIONALI

/*#define LEN 20

struct Studente
{
    char nome[LEN];
    int eta;
   
};

int main(){

    //struct Studente s ={"Antonio", 15};
    struct Studente s =
    {
        15, "Antonio" // ogni membro deve essere inizializzato con una
        // variabile compatibile col suo tipo
    };
    

    printf("Nome: %s, Età: %d\n", s.nome, s.eta);
    printf("Dimensione in memoria: %lu\n", sizeof(s));

    return 0;
}
*/

/*int main(){

    int a[3][2] = {{1,2}, {3,4}, {5,6}};

    printf("%p\n", a);
    printf("%p, %p, %p\n", a[0], a[1], a[2]);
}*/
// Comandi condizionali

/*(int main(){
    if (5) { // 0 è false
             // qualsiasi altro valore è true
        printf("ramo if\n");
    } else{
        printf("ramo else\n");
    }
    return 0;
} */

/*int main (){
    int a = 2;
    // Operatore di pre-incremento ++a
   // printf ("%d\n", ++a); // Stampa 3: l'operatore ++ incrementa a (2+1) e poi restituisce il valore
    //printf ("%d\n", a); // Stampa 3: a è stata già modificata nella riga precedente
    
    // Operatore di post-incremento a++
    printf ("%d\n", a++); // Stampa 2: restituisce il valore attuale di a
    printf ("%d\n", a); // Stampa 3: poi incrementa a di 1
    return 0;
}*/