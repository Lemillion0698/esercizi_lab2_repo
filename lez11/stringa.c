#include <stdio.h>
#include <string.h>
/*int main(){

    char s1[]="ciao"; // crea un array modificabile, allocato sullo stack
    printf("s1 = %s\n", s1);
    s1[2] = 'A';
    printf("s1 = %s\n", s1);

    return 0;
}*/
/*
int main(){
    char* s1 = "ciao";
    printf("s1 = %s\n", s1);
    s1[2] = 'A';
    printf("s1 = %s\n", s1);

    return 0;
}*/

/*int main(){

    char s1[] = "ciao";
    char* s2 = s1; // in questo caso, s2 è un alias di s1

    s2[2] = 'A';

    printf("s1 = %s\n", s1);
    printf("s2 = %s\n",s2);
    return 0;
}*/

int main(){

    char s1[] = "ciao";
    char s2[5]; 

    /*int i=0;
    while(s1[i] != '\0'){
        s2[i] = s1[i]; // procedendo così, s2 è solo una copia di s1
        i++;
    }
    s2[i] = '\0';

    s2[2] = 'A';*/

    strcpy(s2, s1);// anche strcpy crea solo una copia della stringa sorgente
    s2[2] = 'A';
    printf("s1 = %s\n", s1);
    printf("s2 = %s\n",s2);

    return 0;
}