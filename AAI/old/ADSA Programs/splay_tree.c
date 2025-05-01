

#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
    int key;
    struct Node *left, *right;
} Node;


Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    return node;
}


// Right rotation
Node* rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}


// Left rotation
Node* leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}


// Splaying operation
Node* splay(Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;
    
    // Key in left subtree
    if (root->key > key) {
        if (root->left == NULL) return root;
        
        // Zig-Zig (left-left)
        if (root->left->key > key) {
            root->left->left = splay(root->left->left, key);
            root = rightRotate(root);
        }
        // Zig-Zag (left-right)
        else if (root->left->key < key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }
        
        return (root->left == NULL) ? root : rightRotate(root);
    }
    // Key in right subtree
    else {
        if (root->right == NULL) return root;
        
        // Zag-Zig (right-left)
        if (root->right->key > key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        // Zag-Zag (right-right)
        else if (root->right->key < key) {
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }
        
        return (root->right == NULL) ? root : leftRotate(root);
    }
}


// Search operation
Node* search(Node* root, int key) {
    return splay(root, key);
}


// Insert operation
Node* insert(Node* root, int key) {
    if (root == NULL) return newNode(key);
    
    // Bring closest leaf node to root
    root = splay(root, key);
    
    // If key already exists
    if (root->key == key) return root;
    
    Node* new = newNode(key);
    
    if (root->key > key) {
        new->right = root;
        new->left = root->left;
        root->left = NULL;
    }
    else {
        new->left = root;
        new->right = root->right;
        root->right = NULL;
    }
    
    return new;
}


// Delete operation
Node* delete(Node* root, int key) {
    if (root == NULL) return NULL;
    
    root = splay(root, key);
    
    if (root->key != key) return root;
    
    Node* temp;
    if (root->left == NULL) {
        temp = root;
        root = root->right;
    }
    else {
        temp = root;
        root = splay(root->left, key);
        root->right = temp->right;
    }
    
    free(temp);
    return root;
}


// Inorder traversal
void inorder(Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}


int main() {
    Node* root = NULL;
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);
    
    printf("Inorder traversal:\n");
    inorder(root);
    printf("\n");
    
    root = search(root, 30);
    printf("After searching 30:\n");
    inorder(root);
    printf("\n");
    
    root = delete(root, 40);
    printf("After deleting 40:\n");
    inorder(root);
    printf("\n");
    
    return 0;
}
