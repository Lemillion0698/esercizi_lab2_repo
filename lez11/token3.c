#include <stdio.h>
#include <string.h>

int main(){

    char* s = "12,13,14,15"; // la stringa puntata da s si trova in una zona
// lettura ... 
    char *tok;
    printf("Stringa: %s\n", s);

    tok = strtok(s, ","); // ..., non la posso modificare
    printf("Token: %s\n", tok);
    
    while ((tok = strtok(NULL,",")) != NULL){
        printf("Token: %s\n", tok);
    }

    printf("Stringa: %s\n", s);
    return 0;
}