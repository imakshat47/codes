#include <iostream>
using namespace std;

#define MAX_SIZE 100

// -------------------- Array Operations --------------------
void arrayInsertion(int arr[], int &size, int element, int position) {
    if (size >= MAX_SIZE) {
        cout << "Array is full. Cannot insert.\n";
        return;
    }
    if (position < 1 || position > size + 1) {
        cout << "Invalid position!\n";
        return;
    }
    for (int i = size; i >= position; i--) {
        arr[i] = arr[i - 1];
    }
    arr[position - 1] = element;
    size++;
}

void arrayDeletion(int arr[], int &size, int position) {
    if (position < 1 || position > size) {
        cout << "Invalid position!\n";
        return;
    }
    for (int i = position - 1; i < size - 1; i++) {
        arr[i] = arr[i + 1];
    }
    size--;
}

void arrayTraversal(int arr[], int size) {
    cout << "Array Elements: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// -------------------- Singly Linked List --------------------
struct Node {
    int data;
    Node* next;
};

Node* head = NULL;

void linkedListInsertion(int element, int position) {
    Node* newNode = new Node();
    newNode->data = element;
    newNode->next = NULL;

    if (position < 1) {
        cout << "Invalid position!\n";
        return;
    }

    if (position == 1) {
        newNode->next = head;
        head = newNode;
        return;
    }

    Node* temp = head;
    for (int i = 1; temp != NULL && i < position - 1; i++) {
        temp = temp->next;
    }

    if (temp == NULL) {
        cout << "Position out of range!\n";
        return;
    }

    newNode->next = temp->next;
    temp->next = newNode;
}

void linkedListDeletion(int position) {
    if (head == NULL || position < 1) {
        cout << "Invalid position or empty list!\n";
        return;
    }

    Node* temp = head;

    if (position == 1) {
        head = head->next;
        delete temp;
        return;
    }

    for (int i = 1; temp != NULL && i < position - 1; i++) {
        temp = temp->next;
    }

    if (temp == NULL || temp->next == NULL) {
        cout << "Position out of range!\n";
        return;
    }

    Node* del = temp->next;
    temp->next = del->next;
    delete del;
}

void linkedListTraversal() {
    Node* temp = head;
    cout << "Linked List Elements: ";
    while (temp != NULL) {
        cout << temp->data;
        if (temp->next != NULL) cout << " -> ";
        temp = temp->next;
    }
    cout << endl;
}

// -------------------- Main --------------------
int main() {
    int arr[MAX_SIZE], arrSize = 0;
    int choice, element, position;

    cout << "Demonstrating Array Operations:\n";
    arrInsertionDemo:
    arrayInsertion(arr, arrSize, 10, 1);
    arrayInsertion(arr, arrSize, 20, 2);
    arrayInsertion(arr, arrSize, 30, 3);
    arrayTraversal(arr, arrSize);
    arrayDeletion(arr, arrSize, 2);
    arrayTraversal(arr, arrSize);

    cout << "\nDemonstrating Linked List Operations:\n";
    linkedListInsertion(10, 1);
    linkedListInsertion(20, 2);
    linkedListInsertion(30, 3);
    linkedListTraversal();
    linkedListDeletion(2);
    linkedListTraversal();


    return 0;
}
