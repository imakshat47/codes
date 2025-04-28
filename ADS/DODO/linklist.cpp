#include <iostream>
using namespace std;

// ------------------------ Singly Linked List ------------------------
struct SinglyNode {
    int data;
    SinglyNode* next;
};

void insertSingly(SinglyNode*& head, int value) {
    SinglyNode* newNode = new SinglyNode{value, nullptr};
    if (!head) {
        head = newNode;
    } else {
        SinglyNode* temp = head;
        while (temp->next)
            temp = temp->next;
        temp->next = newNode;
    }
    cout << "Inserted " << value << " into Singly Linked List." << endl;
}

void deleteSingly(SinglyNode*& head, int value) {
    SinglyNode* temp = head;
    SinglyNode* prev = nullptr;

    while (temp && temp->data != value) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        cout << "Value " << value << " not found in Singly Linked List." << endl;
        return;
    }

    if (!prev) head = head->next;
    else prev->next = temp->next;

    delete temp;
    cout << "Deleted " << value << " from Singly Linked List." << endl;
}

void traverseSingly(SinglyNode* head) {
    cout << "Singly Linked List: ";
    while (head) {
        cout << head->data;
        if (head->next) cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

// ------------------------ Doubly Linked List ------------------------
struct DoublyNode {
    int data;
    DoublyNode* prev;
    DoublyNode* next;
};

void insertDoubly(DoublyNode*& head, int value) {
    DoublyNode* newNode = new DoublyNode{value, nullptr, nullptr};
    if (!head) {
        head = newNode;
    } else {
        DoublyNode* temp = head;
        while (temp->next)
            temp = temp->next;
        temp->next = newNode;
        newNode->prev = temp;
    }
    cout << "Inserted " << value << " into Doubly Linked List." << endl;
}

void deleteDoubly(DoublyNode*& head, int value) {
    DoublyNode* temp = head;

    while (temp && temp->data != value)
        temp = temp->next;

    if (!temp) {
        cout << "Value " << value << " not found in Doubly Linked List." << endl;
        return;
    }

    if (temp->prev) temp->prev->next = temp->next;
    else head = temp->next;

    if (temp->next) temp->next->prev = temp->prev;

    delete temp;
    cout << "Deleted " << value << " from Doubly Linked List." << endl;
}

void traverseDoubly(DoublyNode* head) {
    cout << "Doubly Linked List: ";
    while (head) {
        cout << head->data;
        if (head->next) cout << " <-> ";
        head = head->next;
    }
    cout << endl;
}

// ------------------------ Circular Linked List ------------------------
struct CircularNode {
    int data;
    CircularNode* next;
};

void insertCircular(CircularNode*& tail, int value) {
    CircularNode* newNode = new CircularNode{value, nullptr};
    if (!tail) {
        newNode->next = newNode;
        tail = newNode;
    } else {
        newNode->next = tail->next;
        tail->next = newNode;
        tail = newNode;
    }
    cout << "Inserted " << value << " into Circular Linked List." << endl;
}

void deleteCircular(CircularNode*& tail, int value) {
    if (!tail) {
        cout << "Circular Linked List is empty." << endl;
        return;
    }

    CircularNode* curr = tail->next;
    CircularNode* prev = tail;
    bool found = false;

    do {
        if (curr->data == value) {
            found = true;
            break;
        }
        prev = curr;
        curr = curr->next;
    } while (curr != tail->next);

    if (!found) {
        cout << "Value " << value << " not found in Circular Linked List." << endl;
        return;
    }

    if (curr == prev) {
        delete curr;
        tail = nullptr;
    } else {
        prev->next = curr->next;
        if (curr == tail) tail = prev;
        delete curr;
    }

    cout << "Deleted " << value << " from Circular Linked List." << endl;
}

void traverseCircular(CircularNode* tail) {
    cout << "Circular Linked List: ";
    if (!tail) {
        cout << "List is empty." << endl;
        return;
    }

    CircularNode* curr = tail->next;
    do {
        cout << curr->data;
        curr = curr->next;
        if (curr != tail->next) cout << " -> ";
    } while (curr != tail->next);
    cout << endl;
}

// ------------------------ Main Function ------------------------
int main() {
    SinglyNode* singlyHead = nullptr;
    DoublyNode* doublyHead = nullptr;
    CircularNode* circularTail = nullptr;

    // --- Singly Linked List Demo ---
    cout << "\n- Singly Linked List Operations -----" << endl;
    insertSingly(singlyHead, 10);
    insertSingly(singlyHead, 20);
    insertSingly(singlyHead, 30);
    traverseSingly(singlyHead);
    deleteSingly(singlyHead, 20);
    traverseSingly(singlyHead);

    // --- Doubly Linked List Demo ---
    cout << "\n- Doubly Linked List Operations -----" << endl;
    insertDoubly(doublyHead, 5);
    insertDoubly(doublyHead, 15);
    insertDoubly(doublyHead, 25);
    traverseDoubly(doublyHead);
    deleteDoubly(doublyHead, 15);
    traverseDoubly(doublyHead);

    // --- Circular Linked List Demo ---
    cout << "\n- Circular Linked List Operations -----" << endl;
    insertCircular(circularTail, 100);
    insertCircular(circularTail, 200);
    insertCircular(circularTail, 300);
    traverseCircular(circularTail);
    deleteCircular(circularTail, 200);
    traverseCircular(circularTail);

    return 0;
}
