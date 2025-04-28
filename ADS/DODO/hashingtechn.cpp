#include <iostream>
#include <list>
using namespace std;

const int TABLE_SIZE = 10;

class HashTable {
private:
    list<pair<int, string>> table[TABLE_SIZE];

    int hashFunction(int key) {
        return key % TABLE_SIZE;
    }

public:
    void insert(int key, const string& value) {
        cout << "Insert " << key << endl;
        int index = hashFunction(key);
        table[index].push_back({key, value});
    }

    void remove(int key) {
        cout << "Delete operation\n";
        int index = hashFunction(key);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->first == key) {
                table[index].erase(it);
                return;
            }
        }
    }

    void search(int key) {
        cout << "Search operation\n";
        int index = hashFunction(key);
        for (auto& element : table[index]) {
            if (element.first == key) {
                cout << "Found " << key << ": " << element.second << endl;
                return;
            }
        }
        cout << "Key not found.\n";
    }

    void display() {
        cout << "Display operation\n";
        bool hasElements = false;
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (!table[i].empty()) {
                hasElements = true;
                cout << "Index " << i << ": ";
                for (auto& pair : table[i]) {
                    cout << pair.first << "->" << pair.second << " ";
                }
                cout << endl;
            }
        }
        if (!hasElements) {
            cout << "Hash Table is empty.\n";
        }
    }
};

int main() {
    HashTable ht;

    ht.insert(1, "Apple");
    ht.insert(11, "Banana");
    ht.insert(21, "Mango");

    ht.display();

    ht.search(11);
    ht.remove(11);
    ht.search(11);

    ht.display();

    return 0;
}
