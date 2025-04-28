#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <vector>

#define MAX_LEVEL 16

class Node {
public:
    int key, value;
    std::vector<Node*> forward;

    Node(int k, int v, int level) : key(k), value(v), forward(level + 1, nullptr) {}
};

class SkipList {
private:
    int level;
    Node* header;

    int randomLevel() {
        int lvl = 0;
        while (rand() < RAND_MAX / 2 && lvl < MAX_LEVEL) lvl++;
        return lvl;
    }

public:
    SkipList() {
        level = 0;
        header = new Node(INT_MIN, 0, MAX_LEVEL);
    }

    ~SkipList() {
        Node* curr = header->forward[0];
        while (curr) {
            Node* next = curr->forward[0];
            delete curr;
            curr = next;
        }
        delete header;
    }

    void insert(int key, int value) {
        std::vector<Node*> update(MAX_LEVEL + 1);
        Node* curr = header;

        for (int i = level; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->key < key) {
                curr = curr->forward[i];
            }
            update[i] = curr;
        }

        curr = curr->forward[0];

        if (!curr || curr->key != key) {
            int newLevel = randomLevel();

            if (newLevel > level) {
                for (int i = level + 1; i <= newLevel; i++) {
                    update[i] = header;
                }
                level = newLevel;
            }

            Node* newNode = new Node(key, value, newLevel);
            for (int i = 0; i <= newLevel; i++) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
        } else {
            curr->value = value;
        }
    }

    Node* search(int key) {
        Node* curr = header;

        for (int i = level; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->key < key) {
                curr = curr->forward[i];
            }
        }
        curr = curr->forward[0];

        if (curr && curr->key == key) return curr;
        return nullptr;
    }

    void remove(int key) {
        std::vector<Node*> update(MAX_LEVEL + 1);
        Node* curr = header;

        for (int i = level; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->key < key) {
                curr = curr->forward[i];
            }
            update[i] = curr;
        }

        curr = curr->forward[0];

        if (curr && curr->key == key) {
            for (int i = 0; i <= level; i++) {
                if (update[i]->forward[i] != curr) break;
                update[i]->forward[i] = curr->forward[i];
            }
            delete curr;

            while (level > 0 && !header->forward[level]) level--;
        }
    }

    void display() {
        std::cout << "Skip List (Level " << level << "):\n";
        for (int i = 0; i <= level; i++) {
            Node* node = header->forward[i];
            std::cout << "Level " << i << ": ";
            while (node) {
                std::cout << node->key << " -> ";
                node = node->forward[i];
            }
            std::cout << "NULL\n";
        }
    }
};

int main() {
    srand(time(0));

    SkipList list;
    list.insert(3, 30);
    list.insert(6, 60);
    list.insert(7, 70);
    list.insert(9, 90);
    list.insert(12, 120);
    list.insert(19, 190);
    list.insert(17, 170);
    list.insert(26, 260);
    list.insert(21, 210);
    list.insert(25, 250);

    list.display();

    int key = 12;
    Node* result = list.search(key);
    if (result) {
        std::cout << "Found key " << key << " with value " << result->value << "\n";
    } else {
        std::cout << "Key " << key << " not found\n";
    }

    std::cout << "Deleting key 21\n";
    list.remove(26);
    list.display();

    return 0;
}
