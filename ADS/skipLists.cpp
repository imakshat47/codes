#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>

#define MAX_LEVEL 6  // Maximum level for the skip list
using namespace std;

// Node structure
class Node {
public:
    int key;
    Node** forward;

    Node(int key, int level) {
        this->key = key;
        forward = new Node*[level + 1];
        for (int i = 0; i <= level; ++i)
            forward[i] = nullptr;
    }

    ~Node() {
        delete[] forward;
    }
};

// Skip List class
class SkipList {
private:
    int level;
    Node* header;

public:
    SkipList() {
        level = 0;
        header = new Node(INT_MIN, MAX_LEVEL);
        std::srand(std::time(nullptr));  // Seed for random level
    }

    // Random level generator
    int randomLevel() {
        int lvl = 0;
        while (std::rand() % 2 && lvl < MAX_LEVEL)
            ++lvl;
        return lvl;
    }

    // Insert key
    void insert(int key) {
        Node* current = header;
        Node* update[MAX_LEVEL + 1];

        // Start from the highest level of skip list
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
            update[i] = current;
        }

        current = current->forward[0];

        // Insert only if not already present
        if (!current || current->key != key) {
            int newLevel = randomLevel();

            if (newLevel > level) {
                for (int i = level + 1; i <= newLevel; i++)
                    update[i] = header;
                level = newLevel;
            }

            Node* newNode = new Node(key, newLevel);
            for (int i = 0; i <= newLevel; i++) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
            std::cout << "Inserted key: " << key << "\n";
        }
    }

    // Search key
    bool search(int key) {
        Node* current = header;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
        }

        current = current->forward[0];
        return current && current->key == key;
    }

    // Delete key
    void remove(int key) {
        Node* current = header;
        Node* update[MAX_LEVEL + 1];

        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
            update[i] = current;
        }

        current = current->forward[0];

        if (current && current->key == key) {
            for (int i = 0; i <= level; i++) {
                if (update[i]->forward[i] != current)
                    break;
                update[i]->forward[i] = current->forward[i];
            }

            delete current;

            while (level > 0 && header->forward[level] == nullptr)
                level--;

            std::cout << "Deleted key: " << key << "\n";
        }
    }

    // Display skip list
    void display() {
        std::cout << "\n***** Skip List *****\n";
        for (int i = 0; i <= level; i++) {
            Node* node = header->forward[i];
            std::cout << "Level " << i << ": ";
            while (node) {
                std::cout << node->key << " ";
                node = node->forward[i];
            }
            std::cout << "\n";
        }
    }

    ~SkipList() {
        Node* current = header;
        while (current) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
    }
};
