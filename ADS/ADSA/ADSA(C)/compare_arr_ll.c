

#include <stdio.h>
#include <stdlib.h>


// Array Operations
void insertArray(int arr[], int *size, int value, int pos) {
    for (int i = *size; i > pos; i--) {
        arr[i] = arr[i - 1];
    }
    arr[pos] = value;
    (*size)++;
}


void deleteArray(int arr[], int *size, int pos) {
    for (int i = pos; i < *size - 1; i++) {
        arr[i] = arr[i + 1];
    }
    (*size)--;
}


void displayArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


// Linked List Operations
struct Node {
    int data;
    struct Node* next;
};


void insertLinkedList(struct Node** head, int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = *head;
    *head = newNode;
}


void deleteLinkedList(struct Node** head, int key) {
    struct Node *temp = *head, *prev = NULL;
    if (temp != NULL && temp->data == key) {
        *head = temp->next;
        free(temp);
        return;
    }
    while (temp != NULL && temp->data != key) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    prev->next = temp->next;
    free(temp);
}


void displayLinkedList(struct Node* head) {
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}


int main() {
    // Array Implementation
    int arr[10] = {1, 2, 3, 4, 5};
    int size = 5;
    printf("Array Before Operations:\n");
    displayArray(arr, size);
    insertArray(arr, &size, 10, 2);
    printf("Array After Insertion:\n");
    displayArray(arr, size);
    deleteArray(arr, &size, 3);
    printf("Array After Deletion:\n");
    displayArray(arr, size);
    
    // Linked List Implementation
    struct Node* head = NULL;
    insertLinkedList(&head, 10);
    insertLinkedList(&head, 20);
    insertLinkedList(&head, 30);
    printf("\nLinked List Before Operations:\n");
    displayLinkedList(head);
    deleteLinkedList(&head, 20);
    printf("Linked List After Deletion:\n");
    displayLinkedList(head);
    return 0;
}
