#include <iostream>
#include <cstdlib>
using namespace std;

#define MAX 100

// Array-based Priority Queue
struct PriorityQueueArray {
    int data[MAX];
    int priority[MAX];
    int size;
};

// Linked List Node
struct Node {
    int data;
    int priority;
    Node* next;
};

// Heap-based Priority Queue
struct PriorityQueueHeap {
    int data[MAX];
    int priority[MAX];
    int size;
};

int counter = 0; // For analysis

// ---------------- Array Implementation ----------------
void insertArray(PriorityQueueArray* pq, int val, int pri) {
    cout << "Inserting " << val << " with priority " << pri << " in Array\n";
    int i = pq->size;
    while (i > 0 && pq->priority[i - 1] > pri) {
        pq->data[i] = pq->data[i - 1];
        pq->priority[i] = pq->priority[i - 1];
        i--;
        counter++;
    }
    pq->data[i] = val;
    pq->priority[i] = pri;
    pq->size++;
}

void deleteArray(PriorityQueueArray* pq) {
    if (pq->size == 0) {
        cout << "Array is empty\n";
        return;
    }
    cout << "Deleting " << pq->data[0] << " from Array\n";
    for (int i = 0; i < pq->size - 1; i++) {
        pq->data[i] = pq->data[i + 1];
        pq->priority[i] = pq->priority[i + 1];
        counter++;
    }
    pq->size--;
}

// ---------------- Linked List Implementation ----------------
void insertLinkedList(Node** head, int val, int pri) {
    cout << "Inserting " << val << " with priority " << pri << " in Linked List\n";
    Node* newNode = new Node{val, pri, nullptr};
    if (*head == nullptr || (*head)->priority > pri) {
        newNode->next = *head;
        *head = newNode;
        return;
    }
    Node* temp = *head;
    while (temp->next != nullptr && temp->next->priority <= pri) {
        temp = temp->next;
        counter++;
    }
    newNode->next = temp->next;
    temp->next = newNode;
}

void deleteLinkedList(Node** head) {
    if (*head == nullptr) {
        cout << "Linked List is empty\n";
        return;
    }
    cout << "Deleting " << (*head)->data << " from Linked List\n";
    Node* temp = *head;
    *head = (*head)->next;
    delete temp;
    counter++;
}

// ---------------- Heap Implementation ----------------
void heapify(PriorityQueueHeap* pq, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < pq->size && pq->priority[left] < pq->priority[smallest])
        smallest = left;
    if (right < pq->size && pq->priority[right] < pq->priority[smallest])
        smallest = right;

    if (smallest != i) {
        swap(pq->data[i], pq->data[smallest]);
        swap(pq->priority[i], pq->priority[smallest]);
        counter++;
        heapify(pq, smallest);
    }
}

void insertHeap(PriorityQueueHeap* pq, int val, int pri) {
    cout << "Inserting " << val << " with priority " << pri << " in Heap\n";
    int i = pq->size++;
    pq->data[i] = val;
    pq->priority[i] = pri;
    while (i > 0 && pq->priority[(i - 1) / 2] > pq->priority[i]) {
        swap(pq->data[i], pq->data[(i - 1) / 2]);
        swap(pq->priority[i], pq->priority[(i - 1) / 2]);
        i = (i - 1) / 2;
        counter++;
    }
}

void deleteHeap(PriorityQueueHeap* pq) {
    if (pq->size == 0) {
        cout << "Heap is empty\n";
        return;
    }
    cout << "Deleting " << pq->data[0] << " from Heap\n";
    pq->data[0] = pq->data[--pq->size];
    pq->priority[0] = pq->priority[pq->size];
    heapify(pq, 0);
}

// ---------------- Main Function ----------------
int main() {
    PriorityQueueArray arrPQ = {.size = 0};
    Node* llPQ = nullptr;
    PriorityQueueHeap heapPQ = {.size = 0};

    // Array Implementation
    insertArray(&arrPQ, 15, 2);
    insertArray(&arrPQ, 25, 1);
    insertArray(&arrPQ, 35, 3);
    deleteArray(&arrPQ);
    cout << "Array Priority Queue iterations: " << counter << "\n";
    counter = 0;

    // Linked List Implementation
    insertLinkedList(&llPQ, 15, 2);
    insertLinkedList(&llPQ, 25, 1);
    insertLinkedList(&llPQ, 35, 3);
    deleteLinkedList(&llPQ);
    cout << "Linked List Priority Queue iterations: " << counter << "\n";
    counter = 0;

    // Heap Implementation
    insertHeap(&heapPQ, 15, 2);
    insertHeap(&heapPQ, 25, 1);
    insertHeap(&heapPQ, 35, 3);
    deleteHeap(&heapPQ);
    cout << "Heap Priority Queue iterations: " << counter << "\n";

    return 0;
}
