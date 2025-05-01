

#include <stdio.h>
#include <stdlib.h>


// ---------------------- Singly Linked List ----------------------
struct Node {
    int data;
    struct Node* next;
};


void insertSLL(struct Node** head, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = *head;
    *head = newNode;
    printf("Inserted %d into Singly Linked List.\n", data);
}


void deleteSLL(struct Node** head, int key) {
    struct Node *temp = *head, *prev = NULL;
    if (temp != NULL && temp->data == key) {
        *head = temp->next;
        free(temp);
        printf("Deleted %d from Singly Linked List.\n", key);
        return;
    }
    while (temp != NULL && temp->data != key) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    prev->next = temp->next;
    free(temp);
    printf("Deleted %d from Singly Linked List.\n", key);
}


void traverseSLL(struct Node* head) {
    printf("Singly Linked List: ");
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}


// ---------------------- Doubly Linked List ----------------------
struct DNode {
    int data;
    struct DNode* next;
    struct DNode* prev;
};


void insertDLL(struct DNode** head, int data) {
    struct DNode* newNode = (struct DNode*)malloc(sizeof(struct DNode));
    newNode->data = data;
    newNode->next = *head;
    newNode->prev = NULL;
    if (*head != NULL) (*head)->prev = newNode;
    *head = newNode;
    printf("Inserted %d into Doubly Linked List.\n", data);
}


void deleteDLL(struct DNode** head, int key) {
    struct DNode* temp = *head;
    while (temp != NULL && temp->data != key)
        temp = temp->next;
    if (temp == NULL) return;
    if (temp->prev) temp->prev->next = temp->next;
    if (temp->next) temp->next->prev = temp->prev;
    if (*head == temp) *head = temp->next;
    free(temp);
    printf("Deleted %d from Doubly Linked List.\n", key);
}


void traverseDLL(struct DNode* head) {
    printf("Doubly Linked List: ");
    while (head != NULL) {
        printf("%d <-> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}


// ---------------------- Circular Linked List ----------------------
struct CNode {
    int data;
    struct CNode* next;
};


void insertCLL(struct CNode** head, int data) {
    struct CNode* newNode = (struct CNode*)malloc(sizeof(struct CNode));
    struct CNode* temp = *head;
    newNode->data = data;
    newNode->next = *head;
    if (*head == NULL) {
        newNode->next = newNode;
        *head = newNode;
    } else {
        while (temp->next != *head)
            temp = temp->next;
        temp->next = newNode;
    }
    printf("Inserted %d into Circular Linked List.\n", data);
}


void deleteCLL(struct CNode** head, int key) {
    if (*head == NULL) return;
    struct CNode *temp = *head, *prev = NULL;
    while (temp->data != key) {
        if (temp->next == *head) return;
        prev = temp;
        temp = temp->next;
    }
    if (temp == *head) *head = temp->next;
    if (prev != NULL) prev->next = temp->next;
    free(temp);
    printf("Deleted %d from Circular Linked List.\n", key);
}


void traverseCLL(struct CNode* head) {
    if (head == NULL) return;
    struct CNode* temp = head;
    printf("Circular Linked List: ");
    do {
        printf("%d -> ", temp->data);
        temp = temp->next;
    } while (temp != head);
    printf("HEAD\n");
}


// ---------------------- Main Function ----------------------
int main() {
    struct Node* sll = NULL;
    struct DNode* dll = NULL;
    struct CNode* cll = NULL;


    printf("\n---- Singly Linked List Operations ----\n");
    insertSLL(&sll, 10);
    insertSLL(&sll, 20);
    insertSLL(&sll, 30);
    traverseSLL(sll);
    deleteSLL(&sll, 20);
    traverseSLL(sll);


    printf("\n---- Doubly Linked List Operations ----\n");
    insertDLL(&dll, 40);
    insertDLL(&dll, 50);
    insertDLL(&dll, 60);
    traverseDLL(dll);
    deleteDLL(&dll, 50);
    traverseDLL(dll);


    printf("\n---- Circular Linked List Operations ----\n");
    insertCLL(&cll, 70);
    insertCLL(&cll, 80);
    insertCLL(&cll, 90);
    traverseCLL(cll);
    deleteCLL(&cll, 80);
    traverseCLL(cll);
    return 0;
}
