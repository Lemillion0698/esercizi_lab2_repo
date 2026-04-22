#include <stdio.h>
#include <string.h>

int main(void) {
    FILE* fp = fopen("data.csv", "r");
    if(!fp) { perror("data.csv"); return 1;}
    char buffer[512];
    while (fgets(buffer, sizeof buffer, fp)) {
        /* rimuovi newbuffer */
        buffer[strcspn(buffer, "\r\n")] ='\0';
        /* split semplice (no quote) */
        char* p = strtok(buffer, ",");
        while (p) {
            printf("[%s]", p);
            p = strtok(NULL, ",");
            putchar('\n');
        }
    }
    fclose(fp);

}