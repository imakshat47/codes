#include <iostream>
using namespace std;

class DynamicArray {
private:
    int* arr;
    int size;
    int capacity;

    void resize(int newCapacity) {
        int* temp = new int[newCapacity];
        for (int i = 0; i < size; ++i)
            temp[i] = arr[i];
        delete[] arr;
        arr = temp;
        capacity = newCapacity;
    }

public:
    DynamicArray() {
        size = 0;
        capacity = 2;
        arr = new int[capacity];
    }

    void insert(int value) {
        if (size == capacity)
            resize(capacity * 2);
        arr[size++] = value;
        cout << "Inserted: " << value << "\n";
    }

    void deleteAt(int index) {
        if (index < 0 || index >= size) {
            cout << "Invalid index\n";
            return;
        }
        cout << "Deleted: " << arr[index] << "\n";
        for (int i = index; i < size - 1; ++i)
            arr[i] = arr[i + 1];
        size--;
        if (size < capacity / 2 && capacity > 2)
            resize(capacity / 2);
    }

    int search(int value) {
        for (int i = 0; i < size; ++i) {
            if (arr[i] == value)
                return i;
        }
        return -1;
    }

    void update(int index, int value) {
        if (index < 0 || index >= size) {
            cout << "Invalid index\n";
            return;
        }
        cout << "Updated index " << index << " from " << arr[index] << " to " << value << "\n";
        arr[index] = value;
    }

    void traverse() {
        cout << "Array Elements: ";
        for (int i = 0; i < size; ++i)
            cout << arr[i] << " ";
        cout << "\n";
    }

    ~DynamicArray() {
        delete[] arr;
    }
};

int main() {
    DynamicArray da;

    da.insert(10);
    da.insert(20);
    da.insert(30);
    da.traverse();

    da.deleteAt(1);
    da.traverse();

    int index = da.search(30);
    if (index != -1)
        cout << "Element 30 found at index " << index << "\n";
    else
        cout << "Element 30 not found\n";

    da.update(0, 99);
    da.traverse();

    return 0;
}
