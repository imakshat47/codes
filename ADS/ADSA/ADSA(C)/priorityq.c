

#include <stdio.h>
#include <stdlib.h>
#define MAX 100


// Structure for Priority Queue using Array
typedef struct {
    int data[MAX];
    int priority[MAX];
    int size;
} PriorityQueueArray;


// Structure for Priority Queue using Linked List
typedef struct Node {
    int data;
    int priority;
    struct Node* next;
} Node;


// Structure for Priority Queue using Heap
typedef struct {
    int data[MAX];
    int priority[MAX];
    int size;
} PriorityQueueHeap;


int counter = 0; // Global iteration counter


// Array Implementation
void insertArray(PriorityQueueArray *pq, int value, int priority) {
    printf("Inserting %d with priority %d in Array\n", value, priority);
    int i = pq->size;
    while (i > 0 && pq->priority[i - 1] > priority) {
        pq->data[i] = pq->data[i - 1];
        pq->priority[i] = pq->priority[i - 1];
        i--;
        counter++;
    }
    pq->data[i] = value;
    pq->priority[i] = priority;
    pq->size++;
}


void deleteArray(PriorityQueueArray *pq) {
    if (pq->size == 0) return;
    printf("Deleting %d from Array\n", pq->data[0]);
    for (int i = 0; i < pq->size - 1; i++) {
        pq->data[i] = pq->data[i + 1];
        pq->priority[i] = pq->priority[i + 1];
        counter++;
    }
    pq->size--;
}


// Linked List Implementation
void insertLinkedList(Node **head, int value, int priority) {
    printf("Inserting %d with priority %d in Linked List\n", value, priority);
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = value;
    newNode->priority = priority;
    newNode->next = NULL;
    
    if (*head == NULL || (*head)->priority > priority) {
        newNode->next = *head;
        *head = newNode;
        return;
    }
    
    Node *temp = *head;
    while (temp->next != NULL && temp->next->priority <= priority) {
        temp = temp->next;
        counter++;
    }
    newNode->next = temp->next;
    temp->next = newNode;
}


void deleteLinkedList(Node **head) {
    if (*head == NULL) return;
    printf("Deleting %d from Linked List\n", (*head)->data);
    Node *temp = *head;
    *head = (*head)->next;
    free(temp);
    counter++;
}


// Heap Implementation
void heapify(PriorityQueueHeap *pq, int i) {
    int smallest = i, left = 2 * i + 1, right = 2 * i + 2;
    if (left < pq->size && pq->priority[left] < pq->priority[smallest])
        smallest = left;
    if (right < pq->size && pq->priority[right] < pq->priority[smallest])
        smallest = right;
    if (smallest != i) {
        int tempData = pq->data[i], tempPriority = pq->priority[i];
        pq->data[i] = pq->data[smallest];
        pq->priority[i] = pq->priority[smallest];
        pq->data[smallest] = tempData;
        pq->priority[smallest] = tempPriority;
        counter++;
        heapify(pq, smallest);
    }
}


void insertHeap(PriorityQueueHeap *pq, int value, int priority) {
    printf("Inserting %d with priority %d in Heap\n", value, priority);
    int i = pq->size++;
    pq->data[i] = value;
    pq->priority[i] = priority;
    while (i > 0 && pq->priority[(i - 1) / 2] > pq->priority[i]) {
        int tempData = pq->data[i], tempPriority = pq->priority[i];
        pq->data[i] = pq->data[(i - 1) / 2];
        pq->priority[i] = pq->priority[(i - 1) / 2];
        pq->data[(i - 1) / 2] = tempData;
        pq->priority[(i - 1) / 2] = tempPriority;
        i = (i - 1) / 2;
        counter++;
    }
}


void deleteHeap(PriorityQueueHeap *pq) {
    if (pq->size == 0) return;
    printf("Deleting %d from Heap\n", pq->data[0]);
    pq->data[0] = pq->data[--pq->size];
    pq->priority[0] = pq->priority[pq->size];
    heapify(pq, 0);
}


// Main function to test implementations
int main() {
    PriorityQueueArray pqArray = { .size = 0 };
    Node *pqLinkedList = NULL;
    PriorityQueueHeap pqHeap = { .size = 0 };
    
    insertArray(&pqArray, 10, 2);
    insertArray(&pqArray, 20, 1);
    insertArray(&pqArray, 30, 3);
    deleteArray(&pqArray);
    printf("Array-based PQ iterations: %d\n", counter);
    counter = 0;
    
    insertLinkedList(&pqLinkedList, 10, 2);
    insertLinkedList(&pqLinkedList, 20, 1);
    insertLinkedList(&pqLinkedList, 30, 3);
    deleteLinkedList(&pqLinkedList);
    printf("Linked List-based PQ iterations: %d\n", counter);
    counter = 0;
    
    insertHeap(&pqHeap, 10, 2);
    insertHeap(&pqHeap, 20, 1);
    insertHeap(&pqHeap, 30, 3);
    deleteHeap(&pqHeap);
    printf("Heap-based PQ iterations: %d\n", counter);
     return 0;
}
