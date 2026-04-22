#include <stdio.h>
#include <string.h>

int main(){

    char s[100], *tok;
    scanf("%[^\n]", s);
    printf("Stringa: %s\n", s);

    tok = strtok(s, ",");
    printf("Token: %s\n", tok);
    /*
    while ((tok = strtok(NULL,",")) != NULL){
        printf("Token: %s\n", tok);
    }*/

    printf("Stringa: %s\n", s);
    return 0;
}