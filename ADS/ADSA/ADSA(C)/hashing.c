

#include <stdio.h>
#include <stdlib.h>


#define TABLE_SIZE 10


// Node structure for chaining
typedef struct Node {
    int key;
    struct Node* next;
} Node;


// Hash table
Node* hashTable[TABLE_SIZE];


// Hash function
int hashFunction(int key) {
    return key % TABLE_SIZE;
}


// Insert key
void insert(int key) {
    int index = hashFunction(key);
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
    printf("Inserted %d at index %d\n", key, index);
}


// Search key
int search(int key) {
    int index = hashFunction(key);
    Node* temp = hashTable[index];
    while (temp) {
        if (temp->key == key)
            return 1;
        temp = temp->next;
    }
    return 0;
}


// Delete key
void delete(int key) {
    int index = hashFunction(key);
    Node* temp = hashTable[index];
    Node* prev = NULL;


    while (temp) {
        if (temp->key == key) {
            if (prev)
                prev->next = temp->next;
            else
                hashTable[index] = temp->next;
            free(temp);
            printf("Deleted %d from index %d\n", key, index);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Key %d not found\n", key);
}


// Display table
void display() {
    printf("Hash Table:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("%d: ", i);
        Node* temp = hashTable[i];
        while (temp) {
            printf("%d -> ", temp->key);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}


// Driver
int main() {
    insert(10);
    insert(20);
    insert(30);
    insert(25);
    insert(35);


    display();


    if (search(25))
        printf("Found 25\n");
    else
        printf("25 not found\n");


    delete(25);
    display();


    return 0;
}
