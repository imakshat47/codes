

#include <stdio.h>
#include <stdlib.h>


// Structure to define a dynamic array
typedef struct {
    int *data;      // Pointer to the actual array
    int size;       // Current number of elements
    int capacity;   // Total capacity of the array
} DynamicArray;


// Function to initialize the dynamic array
void initArray(DynamicArray *arr, int initialCapacity) {
    arr->capacity = initialCapacity;
    arr->size = 0;
    arr->data = (int *)malloc(sizeof(int) * arr->capacity);


    if (arr->data == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    printf("Array initialized with capacity %d.\n", arr->capacity);
}


// Function to double the capacity when the array is full
void resizeArray(DynamicArray *arr) {
    arr->capacity *= 2;
    int *newData = (int *)realloc(arr->data, sizeof(int) * arr->capacity);


    if (newData == NULL) {
        printf("Memory reallocation failed\n");
        exit(1);
    }


    arr->data = newData;
    printf("Array resized to new capacity %d.\n", arr->capacity);
}


// Function to insert an element at the end of the array
void insertElement(DynamicArray *arr, int value) {
    if (arr->size == arr->capacity) {
        resizeArray(arr);
    }
    arr->data[arr->size++] = value;
    printf("Inserted %d at index %d.\n", value, arr->size - 1);
}


// Function to delete the first occurrence of a value from the array
void deleteElement(DynamicArray *arr, int value) {
    int found = 0;
    for (int i = 0; i < arr->size; i++) {
        if (arr->data[i] == value) {
            found = 1;
            // Shift elements to the left
            for (int j = i; j < arr->size - 1; j++) {
                arr->data[j] = arr->data[j + 1];
            }
            arr->size--;
            printf("Deleted %d from index %d.\n", value, i);
            break;
        }
    }
    if (!found) {
        printf("Value %d not found in array.\n", value);
    }
}


// Function to search for a value in the array
int searchElement(DynamicArray *arr, int value) {
    for (int i = 0; i < arr->size; i++) {
        if (arr->data[i] == value) {
            return i;
        }
    }
    return -1; // Not found
}


// Function to update the element at a given index
void updateElement(DynamicArray *arr, int index, int newValue) {
    if (index >= 0 && index < arr->size) {
        printf("Updated index %d from %d to %d.\n", index, arr->data[index], newValue);
        arr->data[index] = newValue;
    } else {
        printf("Invalid index: %d\n", index);
    }
}


// Function to display all elements in the array
void traverseArray(DynamicArray *arr) {
    printf("Current Array: ");
    for (int i = 0; i < arr->size; i++) {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
}


// Function to free allocated memory
void freeArray(DynamicArray *arr) {
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
    printf("Array memory deallocated.\n");
}


// Main function to demonstrate dynamic array operations
int main() {
    DynamicArray myArray;
    initArray(&myArray, 2);  // Start with capacity 2


    insertElement(&myArray, 10);
    insertElement(&myArray, 20);
    insertElement(&myArray, 30);  // Will trigger resize


    traverseArray(&myArray);


    deleteElement(&myArray, 20);
    traverseArray(&myArray);


    int idx = searchElement(&myArray, 30);
    if (idx != -1) {
        printf("Element 30 found at index %d.\n", idx);
    } else {
        printf("Element 30 not found.\n");
    }


    updateElement(&myArray, 1, 50);
    traverseArray(&myArray);


    freeArray(&myArray);
    return 0;
}
