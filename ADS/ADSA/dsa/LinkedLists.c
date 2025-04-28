#include <stdio.h>
#include <stdlib.h>

// Singly Linked List
struct Node {
    int data;
    struct Node* next;
};

void insertSLL(struct Node** head, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = *head;
    *head = newNode;
    printf("Inserted %d in Singly Linked List\n", data);
}

void deleteSLL(struct Node** head, int key) {
    struct Node *temp = *head, *prev = NULL;
    while (temp != NULL && temp->data != key) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    if (prev == NULL) *head = temp->next;
    else prev->next = temp->next;
    free(temp);
    printf("Deleted %d from Singly Linked List\n", key);
}

void traverseSLL(struct Node* head) {
    printf("SLL: ");
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

// Doubly Linked List
struct DNode {
    int data;
    struct DNode* prev;
    struct DNode* next;
};

void insertDLL(struct DNode** head, int data) {
    struct DNode* newNode = (struct DNode*)malloc(sizeof(struct DNode));
    newNode->data = data;
    newNode->next = *head;
    newNode->prev = NULL;
    if (*head != NULL) (*head)->prev = newNode;
    *head = newNode;
    printf("Inserted %d in Doubly Linked List\n", data);
}

void deleteDLL(struct DNode** head, int key) {
    struct DNode* temp = *head;
    while (temp != NULL && temp->data != key) temp = temp->next;
    if (temp == NULL) return;
    if (temp->prev) temp->prev->next = temp->next;
    else *head = temp->next;
    if (temp->next) temp->next->prev = temp->prev;
    free(temp);
    printf("Deleted %d from Doubly Linked List\n", key);
}

void traverseDLL(struct DNode* head) {
    printf("DLL: ");
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

// Circular Linked List
struct CNode {
    int data;
    struct CNode* next;
};

void insertCLL(struct CNode** head, int data) {
    struct CNode* newNode = (struct CNode*)malloc(sizeof(struct CNode));
    newNode->data = data;
    if (*head == NULL) {
        newNode->next = newNode;
        *head = newNode;
    } else {
        struct CNode* temp = *head;
        while (temp->next != *head) temp = temp->next;
        temp->next = newNode;
        newNode->next = *head;
    }
    printf("Inserted %d in Circular Linked List\n", data);
}

void deleteCLL(struct CNode** head, int key) {
    if (*head == NULL) return;
    struct CNode *curr = *head, *prev = NULL;
    while (curr->data != key) {
        if (curr->next == *head) return;
        prev = curr;
        curr = curr->next;
    }
    if (curr == *head && curr->next == *head) *head = NULL;
    else if (curr == *head) {
        prev = *head;
        while (prev->next != *head) prev = prev->next;
        *head = curr->next;
        prev->next = *head;
    } else prev->next = curr->next;
    free(curr);
    printf("Deleted %d from Circular Linked List\n", key);
}

void traverseCLL(struct CNode* head) {
    struct CNode* temp = head;
    if (!head) return;
    printf("CLL: ");
    do {
        printf("%d ", temp->data);
        temp = temp->next;
    } while (temp != head);
    printf("\n");
}

int main() {
    // Singly Linked List
    struct Node* sll = NULL;
    insertSLL(&sll, 10);
    insertSLL(&sll, 20);
    traverseSLL(sll);
    deleteSLL(&sll, 10);
    traverseSLL(sll);

    // Doubly Linked List
    struct DNode* dll = NULL;
    insertDLL(&dll, 30);
    insertDLL(&dll, 40);
    traverseDLL(dll);
    deleteDLL(&dll, 30);
    traverseDLL(dll);

    // Circular Linked List
    struct CNode* cll = NULL;
    insertCLL(&cll, 50);
    insertCLL(&cll, 60);
    traverseCLL(cll);
    deleteCLL(&cll, 50);
    traverseCLL(cll);
    return 0;
}