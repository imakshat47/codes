#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Array Implementation
class ArrayOperations {
public:
    int arr[100], n;
    ArrayOperations(int size) : n(size) {
        for (int i = 0; i < n; i++) arr[i] = i + 1;
    }
    void insertAtBeginning(int val) {
        for (int i = n; i > 0; i--) arr[i] = arr[i - 1];
        arr[0] = val;
        n++;
    }
    void deleteFromMiddle(int pos) {
        for (int i = pos; i < n - 1; i++) arr[i] = arr[i + 1];
        n--;
    }
    void traverse() {
        for (int i = 0; i < n; i++) cout << arr[i] << " ";
        cout << endl;
    }
};

// Linked List Implementation
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

class LinkedList {
public:
    Node* head;
    LinkedList() : head(nullptr) {}
    void insertAtBeginning(int val) {
        Node* newNode = new Node(val);
        newNode->next = head;
        head = newNode;
    }
    void deleteFromMiddle(int val) {
        Node *temp = head, *prev = nullptr;
        while (temp && temp->data != val) {
            prev = temp;
            temp = temp->next;
        }
        if (temp) {
            if (prev) prev->next = temp->next;
            else head = temp->next;
            delete temp;
        }
    }
    void traverse() {
        Node* temp = head;
        while (temp) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};

// Performance Analysis
void analyzePerformance() {
    const int SIZE = 10000;
    ArrayOperations arr(SIZE);
    LinkedList list;
    for (int i = 0; i < SIZE; i++) list.insertAtBeginning(i);

    // Array insertion time
    auto start = high_resolution_clock::now();
    arr.insertAtBeginning(0);
    auto stop = high_resolution_clock::now();
    auto durationArray = duration_cast<microseconds>(stop - start);

    // Linked List insertion time
    start = high_resolution_clock::now();
    list.insertAtBeginning(0);
    stop = high_resolution_clock::now();
    auto durationLinkedList = duration_cast<microseconds>(stop - start);

    cout << "Array Insertion Time: " << durationArray.count() << " microseconds\n";
    cout << "Linked List Insertion Time: " << durationLinkedList.count() << " microseconds\n";
}

int main() {
    cout << "Array Operations:\n";
    ArrayOperations arr(5);
    arr.insertAtBeginning(0);
    arr.deleteFromMiddle(3);
    arr.traverse();

    cout << "\nLinked List Operations:\n";
    LinkedList list;
    list.insertAtBeginning(3);
    list.insertAtBeginning(2);
    list.insertAtBeginning(1);
    list.deleteFromMiddle(2);
    list.traverse();

    cout << "\nPerformance Analysis:\n";
    analyzePerformance();

    return 0;
}
