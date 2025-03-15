#include <iostream>
using namespace std;

// Node structure for Singly Linked List
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Singly Linked List Class
class SinglyLinkedList {
public:
    Node* head;
    SinglyLinkedList() : head(nullptr) {}

    void insert(int val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = newNode;
            return;
        }
        Node* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }

    void remove(int val) {
        if (!head) return;
        if (head->data == val) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        Node* temp = head;
        while (temp->next && temp->next->data != val) temp = temp->next;
        if (temp->next) {
            Node* delNode = temp->next;
            temp->next = temp->next->next;
            delete delNode;
        }
    }

    void traverse() {
        Node* temp = head;
        while (temp) {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "NULL\n";
    }
};

// Node structure for Doubly Linked List
struct DNode {
    int data;
    DNode* next;
    DNode* prev;
    DNode(int val) : data(val), next(nullptr), prev(nullptr) {}
};

// Doubly Linked List Class
class DoublyLinkedList {
public:
    DNode* head;
    DoublyLinkedList() : head(nullptr) {}

    void insert(int val) {
        DNode* newNode = new DNode(val);
        if (!head) {
            head = newNode;
            return;
        }
        DNode* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
        newNode->prev = temp;
    }

    void remove(int val) {
        if (!head) return;
        DNode* temp = head;
        while (temp && temp->data != val) temp = temp->next;
        if (!temp) return;
        if (temp->prev) temp->prev->next = temp->next;
        if (temp->next) temp->next->prev = temp->prev;
        if (temp == head) head = temp->next;
        delete temp;
    }

    void traverse() {
        DNode* temp = head;
        while (temp) {
            cout << temp->data << " <-> ";
            temp = temp->next;
        }
        cout << "NULL\n";
    }
};

// Node structure for Circular Linked List
struct CNode {
    int data;
    CNode* next;
    CNode(int val) : data(val), next(nullptr) {}
};

// Circular Linked List Class
class CircularLinkedList {
public:
    CNode* head;
    CircularLinkedList() : head(nullptr) {}

    void insert(int val) {
        CNode* newNode = new CNode(val);
        if (!head) {
            head = newNode;
            head->next = head;
            return;
        }
        CNode* temp = head;
        while (temp->next != head) temp = temp->next;
        temp->next = newNode;
        newNode->next = head;
    }

    void remove(int val) {
        if (!head) return;
        CNode* temp = head, *prev = nullptr;
        if (head->data == val && head->next == head) {
            delete head;
            head = nullptr;
            return;
        }
        while (temp->next != head && temp->data != val) {
            prev = temp;
            temp = temp->next;
        }
        if (temp->data == val) {
            if (temp == head) {
                prev = head;
                while (prev->next != head) prev = prev->next;
                head = head->next;
                prev->next = head;
            } else {
                prev->next = temp->next;
            }
            delete temp;
        }
    }

    void traverse() {
        if (!head) return;
        CNode* temp = head;
        do {
            cout << temp->data << " -> ";
            temp = temp->next;
        } while (temp != head);
        cout << "(HEAD)\n";
    }
};

// Main function for testing
int main() {
    cout << "Singly Linked List:\n";
    SinglyLinkedList sll;
    sll.insert(1); sll.insert(2); sll.insert(3);
    sll.traverse();
    sll.remove(2);
    sll.traverse();

    cout << "\nDoubly Linked List:\n";
    DoublyLinkedList dll;
    dll.insert(10); dll.insert(20); dll.insert(30);
    dll.traverse();
    dll.remove(20);
    dll.traverse();

    cout << "\nCircular Linked List:\n";
    CircularLinkedList cll;
    cll.insert(100); cll.insert(200); cll.insert(300);
    cll.traverse();
    cll.remove(200);
    cll.traverse();

    return 0;
}
