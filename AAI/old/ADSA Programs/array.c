#include <stdio.h>
#define MAX_SIZE 100


// Function declaration
void insert(int arr[], int *size, int element, int position);
void delete(int arr[], int *size, int position);
int search(int arr[], int size, int element);
void traverse(int arr[], int size);
void update(int arr[], int size, int position, int new_value);


int main() {
    int arr[MAX_SIZE], size, element, position, new_value;
    printf("Enter the number of elements in the array: ");
    scanf("%d", &size);
    printf("Enter %d elements: ", size);
    for (int i = 0; i < size; i++) {
        scanf("%d", &arr[i]);
    }
    printf("\nInitial Array:\n");
    traverse(arr, size);
    
    // Demonstrate insertion
    printf("\nDemonstrating Insertion:\n");
    printf("Enter element to insert: ");
    scanf("%d", &element);
    printf("Enter position (1-based index): ");
    scanf("%d", &position);
    insert(arr, &size, element, position);
    traverse(arr, size);
    
    // Demonstrate deletion
    printf("\nDemonstrating Deletion:\n");
    printf("Enter position to delete (1-based index): ");
    scanf("%d", &position);
    delete(arr, &size, position);
    traverse(arr, size);
    
    // Demonstrate searching
    printf("\nDemonstrating Searching:\n");
    printf("Enter element to search: ");
    scanf("%d", &element);
    position = search(arr, size, element);
    if (position != -1)
        printf("Element found at position %d\n", position + 1);
    else
        printf("Element not found\n");
    
    // Demonstrate updating
    printf("\nDemonstrating Updation:\n");
    printf("Enter position to update (1-based index): ");
    scanf("%d", &position);
    printf("Enter new value: ");
    scanf("%d", &new_value);
    update(arr, size, position, new_value);
    traverse(arr, size);
    
    printf("\nAll operations completed successfully!\n");
    return 0;
}


void insert(int arr[], int *size, int element, int position) {
    if (*size >= MAX_SIZE) {
        printf("Array is full! Cannot insert.\n");
        return;
    }
    if (position < 1 || position > *size + 1) {
        printf("Invalid position!\n");
        return;
    }
    for (int i = *size; i >= position; i--) {
        arr[i] = arr[i - 1];
    }
    arr[position - 1] = element;
    (*size)++;
    printf("Element inserted successfully!\n");
}


void delete(int arr[], int *size, int position) {
    if (position < 1 || position > *size) {
        printf("Invalid position!\n");
        return;
    }
    for (int i = position - 1; i < *size - 1; i++) {
        arr[i] = arr[i + 1];
    }
    (*size)--;
    printf("Element deleted successfully!\n");
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
    printf("Current Array Elements: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


void update(int arr[], int size, int position, int new_value) {
    if (position < 1 || position > size) {
        printf("Invalid position!\n");
        return;
    }
    arr[position - 1] = new_value;
    printf("Element updated successfully!\n");
}
