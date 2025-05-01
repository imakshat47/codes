

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>


#define MAX_LEVEL 16


typedef struct Node {
    int key;
    int value;
    struct Node **forward; // Array of forward pointers
} Node;


typedef struct SkipList {
    int level; // Current maximum level
    Node *header;
} SkipList;


// Create a new node
Node* create_node(int level, int key, int value) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->forward = (Node**)malloc(sizeof(Node*) * (level + 1));
    for (int i = 0; i <= level; i++) {
        node->forward[i] = NULL;
    }
    return node;
}


// Initialize skip list
SkipList* init_skip_list() {
    SkipList *list = (SkipList*)malloc(sizeof(SkipList));
    list->level = 0;
    list->header = create_node(MAX_LEVEL, INT_MIN, 0);
    
    for (int i = 0; i <= MAX_LEVEL; i++) {
        list->header->forward[i] = NULL;
    }
    
    return list;
}


// Random level generator
int random_level() {
    int level = 0;
    while (rand() < RAND_MAX / 2 && level < MAX_LEVEL) {
        level++;
    }
    return level;
}


// Insert a key-value pair
void insert(SkipList *list, int key, int value) {
    Node *update[MAX_LEVEL + 1];
    Node *current = list->header;
    
    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    
    current = current->forward[0];
    
    if (current == NULL || current->key != key) {
        int new_level = random_level();
        
        if (new_level > list->level) {
            for (int i = list->level + 1; i <= new_level; i++) {
                update[i] = list->header;
            }
            list->level = new_level;
        }
        
        Node *new_node = create_node(new_level, key, value);
        
        for (int i = 0; i <= new_level; i++) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
    } else {
        current->value = value; // Update existing key
    }
}


// Search for a key
Node* search(SkipList *list, int key) {
    Node *current = list->header;
    
    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }
    
    current = current->forward[0];
    
    if (current != NULL && current->key == key) {
        return current;
    } else {
        return NULL;
    }
}


// Delete a key
void delete(SkipList *list, int key) {
    Node *update[MAX_LEVEL + 1];
    Node *current = list->header;
    
    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    
    current = current->forward[0];
    
    if (current != NULL && current->key == key) {
        for (int i = 0; i <= list->level; i++) {
            if (update[i]->forward[i] != current) {
                break;
            }
            update[i]->forward[i] = current->forward[i];
        }
        
        free(current->forward);
        free(current);
        
        while (list->level > 0 && list->header->forward[list->level] == NULL) {
            list->level--;
        }
    }
}


// Print the skip list
void print_skip_list(SkipList *list) {
    printf("Skip List (level %d):\n", list->level);
    for (int i = 0; i <= list->level; i++) {
        Node *node = list->header->forward[i];
        printf("Level %d: ", i);
        while (node != NULL) {
            printf("%d -> ", node->key);
            node = node->forward[i];
        }
        printf("NULL\n");
    }
}


// Free memory
void free_skip_list(SkipList *list) {
    Node *current = list->header->forward[0];
    while (current != NULL) {
        Node *next = current->forward[0];
        free(current->forward);
        free(current);
        current = next;
    }
    free(list->header->forward);
    free(list->header);
    free(list);
}


int main() {
    srand(time(NULL));
    
    SkipList *list = init_skip_list();
    
    insert(list, 3, 30);
    insert(list, 6, 60);
    insert(list, 7, 70);
    insert(list, 9, 90);
    insert(list, 12, 120);
    insert(list, 19, 190);
    insert(list, 17, 170);
    insert(list, 26, 260);
    insert(list, 21, 210);
    insert(list, 25, 250);
    
    print_skip_list(list);
    
    // Search test
    int key = 17;
    Node *result = search(list, key);
    if (result) {
        printf("Found key %d with value %d\n", key, result->value);
    } else {
        printf("Key %d not found\n", key);
    }
    
    // Delete test
    printf("Deleting key 21\n");
    delete(list, 21);
    print_skip_list(list);
    
    free_skip_list(list);
    
    return 0;
}
