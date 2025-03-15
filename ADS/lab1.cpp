#include <iostream>
using namespace std;

// Function to traverse and display array elements
void traverse(int arr[], int n) {
    cout << "Array elements: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Function to insert an element at a specific position
void insert(int arr[], int& n, int element, int position, int capacity) {
    if (n >= capacity) {
        cout << "Insertion failed: Array is full." << endl;
        return;
    }
    if (position < 0 || position > n) {
        cout << "Insertion failed: Invalid position." << endl;
        return;
    }
    for (int i = n; i > position; i--) {
        arr[i] = arr[i - 1];
    }
    arr[position] = element;
    n++;
    cout << "Inserted " << element << " at position " << position << "." << endl;
}

// Function to delete an element from a specific position
void remove(int arr[], int& n, int position) {
    if (position < 0 || position >= n) {
        cout << "Deletion failed: Invalid position." << endl;
        return;
    }
    cout << "Deleted element " << arr[position] << " from position " << position << "." << endl;
    for (int i = position; i < n - 1; i++) {
        arr[i] = arr[i + 1];
    }
    n--;
}

// Function to search for an element and return its position
int search(int arr[], int n, int element) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == element) {
            return i;
        }
    }
    return -1; // Element not found
}

// Function to update an element at a specific position
void update(int arr[], int n, int position, int newElement) {
    if (position < 0 || position >= n) {
        cout << "Update failed: Invalid position." << endl;
        return;
    }
    cout << "Updated position " << position << " from " << arr[position] << " to " << newElement << "." << endl;
    arr[position] = newElement;
}

int main() {
    const int capacity = 100; // Maximum capacity of the array
    int arr[capacity];
    int n = 0; // Current number of elements in the array

    // Inserting elements
    insert(arr, n, 10, 0, capacity);
    insert(arr, n, 20, 1, capacity);
    insert(arr, n, 30, 2, capacity);
    insert(arr, n, 40, 3, capacity);
    insert(arr, n, 50, 4, capacity);

    // Traversing array
    traverse(arr, n);

    // Deleting an element
    remove(arr, n, 2);
    traverse(arr, n);

    // Searching for an element
    int pos = search(arr, n, 40);
    if (pos != -1) {
        cout << "Element 40 found at position " << pos << "." << endl;
    } else {
        cout << "Element 40 not found." << endl;
    }

    // Updating an element
    update(arr, n, 1, 25);
    traverse(arr, n);

    return 0;
}
