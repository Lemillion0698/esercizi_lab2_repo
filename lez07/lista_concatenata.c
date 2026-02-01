#include <stdio.h>

struct n{
    int val;
    struct n * next;
};

typedef struct n Node; 


int main(){
    Node* head = NULL; // lista vuota

    Node elem;
    elem.val = 10;
    elem.next = NULL;

    head = &elem; // lista con un elemento

    Node elem1; // elemento da aggiungere in coda
    elem1.val=11;
    elem1.next=NULL;
    elem.next=&elem1; // aggiungo in coda

    Node elem2; // elemento da aggiungere in testa
    elem2.val=9;
    elem2.next=head; // aggiungo in testa
    head=&elem2;

    Node* l=head;
    while(l != NULL){
        printf("%d ", l->val);
        l=l->next;
    }
    printf("\n");
    return 0;
}