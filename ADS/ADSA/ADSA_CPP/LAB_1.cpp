#include <iostream>
using namespace std;

#define MAX_SIZE 100

// Function declarations
void insert(int arr[], int &size, int element, int position);
void remove(int arr[], int &size, int position);
int search(int arr[], int size, int element);
void traverse(int arr[], int size);
void update(int arr[], int size, int position, int new_value);

int main() {
    int arr[MAX_SIZE], size, element, position, new_value;

    cout << "Enter the number of elements in array: ";
    cin >> size;

    cout << "Enter " << size << " elements: ";
    for (int i = 0; i < size; i++) {
        cin >> arr[i];
    }

    cout << "\nInitial Array:\n";
    traverse(arr, size);

    // Insertion
    cout << "\nInsertion:\n";
    cout << "Enter element to insert: ";
    cin >> element;
    cout << "Enter position (1-based index): ";
    cin >> position;
    insert(arr, size, element, position);
    traverse(arr, size);

    // Deletion
    cout << "\nDeletion:\n";
    cout << "Enter position to delete (1-based index): ";
    cin >> position;
    remove(arr, size, position);
    traverse(arr, size);

    // Searching
    cout << "\nSearching:\n";
    cout << "Enter element to search: ";
    cin >> element;
    position = search(arr, size, element);
    if (position != -1)
        cout << "Element found at position " << position + 1 << endl;
    else
        cout << "Element not found\n";

    // Updation
    cout << "\nUpdation:\n";
    cout << "Enter position to update (1-based index): ";
    cin >> position;
    cout << "Enter new value: ";
    cin >> new_value;
    update(arr, size, position, new_value);
    traverse(arr, size);

    cout << "\nAll operations completed successfully!\n";

    return 0;
}

void insert(int arr[], int &size, int element, int position) {
    if (size >= MAX_SIZE) {
        cout << "Array is full! Cannot insert.\n";
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
    cout << "Element inserted successfully!\n";
}

void remove(int arr[], int &size, int position) {
    if (position < 1 || position > size) {
        cout << "Invalid position!\n";
        return;
    }
    for (int i = position - 1; i < size - 1; i++) {
        arr[i] = arr[i + 1];
    }
    size--;
    cout << "Element deleted successfully!\n";
}

int search(int arr[], int size, int element) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == element) {
            return i;
        }
    }
    return -1;
}

void traverse(int arr[], int size) {
    cout << "Current Array Elements: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void update(int arr[], int size, int position, int new_value) {
    if (position < 1 || position > size) {
        cout << "Invalid position!\n";
        return;
    }
    arr[position - 1] = new_value;
    cout << "Element updated successfully!\n";
}
