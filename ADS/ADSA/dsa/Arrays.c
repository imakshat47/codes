// C implementation of Array operations
#include <stdio.h>
#define SIZE 5

// Insertion
void insert(int arr[], int *length, int index, int value) {
    if (*length >= SIZE) {
        printf("Array is full\n");
        return;
    }
    if (index < 0 || index > *length) {
        printf("Invalid index\n");
        return;
    }
    for (int i = *length; i > index; i--) {
        arr[i] = arr[i - 1];
    }
    arr[index] = value;
    (*length)++;
    printf("Inserted %d at index %d\n", value, index);
}

// Deletion
void delete(int arr[], int *length, int index) {
    if (index < 0 || index >= *length) {
        printf("Invalid index\n");
        return;
    }
    printf("Deleted %d from index %d\n", arr[index], index);
    for (int i = index; i < *length - 1; i++) {
        arr[i] = arr[i + 1];
    }
    (*length)--;
}

// Searching
int search(int arr[], int length, int value) {
    for (int i = 0; i < length; i++) {
        if (arr[i] == value) {
            printf("Element %d found at index %d\n", value, i);
            return i;
        }
    }
    printf("Element not found\n");
    return -1;
}

// Traversing
void traverse(int arr[], int length) {
    printf("Array elements: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Updation
void update(int arr[], int length, int index, int value) {
    if (index < 0 || index >= length) {
        printf("Invalid index\n");
        return;
    }
    printf("Updated index %d from %d to %d\n", index, arr[index], value);
    arr[index] = value;
}

int main() {
    int arr[SIZE] = {0};
    int length = 0;

    insert(arr, &length, 0, 10);
    insert(arr, &length, 1, 20);
    insert(arr, &length, 2, 30);
    traverse(arr, length);
    search(arr, length, 20);
    update(arr, length, 1, 25);
    traverse(arr, length);
    delete(arr, &length, 1);
    traverse(arr, length);

    return 0;
}