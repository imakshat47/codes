#include <stdio.h>
#include <stdlib.h>
#define MAX 100

// Priority Queue using Array
typedef struct {
    int data;
    int priority;
} PQElement;

PQElement pq_array[MAX];
int size_array = 0;

void insertArray(int data, int priority) {
    pq_array[size_array].data = data;
    pq_array[size_array].priority = priority;
    size_array++;
    printf("Inserted %d with priority %d in Array PQ\n", data, priority);
}

void deleteArray() {
    if (size_array == 0) return;
    int max = 0;
    for (int i = 1; i < size_array; i++)
        if (pq_array[i].priority > pq_array[max].priority)
            max = i;
    printf("Deleted %d with priority %d from Array PQ\n", pq_array[max].data, pq_array[max].priority);
    for (int i = max; i < size_array - 1; i++)
        pq_array[i] = pq_array[i + 1];
    size_array--;
}

// Priority Queue using Linked List
typedef struct node {
    int data, priority;
    struct node* next;
} Node;

Node* pq_ll = NULL;

void insertLL(int data, int priority) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->priority = priority;
    newNode->next = NULL;

    if (!pq_ll || priority > pq_ll->priority) {
        newNode->next = pq_ll;
        pq_ll = newNode;
    } else {
        Node* temp = pq_ll;
        while (temp->next && temp->next->priority >= priority)
            temp = temp->next;
        newNode->next = temp->next;
        temp->next = newNode;
    }
    printf("Inserted %d with priority %d in Linked List PQ\n", data, priority);
}

void deleteLL() {
    if (!pq_ll) return;
    Node* temp = pq_ll;
    printf("Deleted %d with priority %d from Linked List PQ\n", temp->data, temp->priority);
    pq_ll = pq_ll->next;
    free(temp);
}

// Priority Queue using Heap (Min Heap)
int heap[MAX], heap_size = 0;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void insertHeap(int val) {
    int i = heap_size++;
    heap[i] = val;
    while (i != 0 && heap[(i - 1) / 2] > heap[i]) {
        swap(&heap[i], &heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
    printf("Inserted %d in Heap PQ\n", val);
}

void deleteHeap() {
    if (heap_size == 0) return;
    printf("Deleted %d from Heap PQ\n", heap[0]);
    heap[0] = heap[--heap_size];
    int i = 0;
    while (2 * i + 1 < heap_size) {
        int smallest = i, left = 2 * i + 1, right = 2 * i + 2;
        if (left < heap_size && heap[left] < heap[smallest]) smallest = left;
        if (right < heap_size && heap[right] < heap[smallest]) smallest = right;
        if (smallest == i) break;
        swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
}

int main() {
    // Array PQ
    insertArray(10, 2);
    insertArray(30, 4);
    insertArray(20, 3);
    deleteArray();

    // Linked List PQ
    insertLL(10, 2);
    insertLL(30, 4);
    insertLL(20, 3);
    deleteLL();

    // Heap PQ
    insertHeap(30);
    insertHeap(10);
    insertHeap(20);
    deleteHeap();

    return 0;
}