#include <iostream>
using namespace std;

// Node structure
struct Node {
    int key;
    Node* left;
    Node* right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

// Right rotate
Node* rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

// Left rotate
Node* leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// Splay function
Node* splay(Node* root, int key) {
    if (!root || root->key == key)
        return root;

    // Left subtree
    if (key < root->key) {
        if (!root->left) return root;

        // Zig-Zig (Left Left)
        if (key < root->left->key) {
            root->left->left = splay(root->left->left, key);
            root = rightRotate(root);
        }
        // Zig-Zag (Left Right)
        else if (key > root->left->key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right)
                root->left = leftRotate(root->left);
        }

        return root->left ? rightRotate(root) : root;
    }
    // Right subtree
    else {
        if (!root->right) return root;

        // Zag-Zig (Right Left)
        if (key < root->right->key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left)
                root->right = rightRotate(root->right);
        }
        // Zag-Zag (Right Right)
        else if (key > root->right->key) {
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }

        return root->right ? leftRotate(root) : root;
    }
}

// Insert function
Node* insert(Node* root, int key) {
    if (!root) return new Node(key);

    root = splay(root, key);
    if (root->key == key) return root;  // Duplicate not inserted

    Node* newNode = new Node(key);
    if (key < root->key) {
        newNode->right = root;
        newNode->left = root->left;
        root->left = nullptr;
    } else {
        newNode->left = root;
        newNode->right = root->right;
        root->right = nullptr;
    }
    return newNode;
}

// Delete function
Node* deleteKey(Node* root, int key) {
    if (!root) return nullptr;

    root = splay(root, key);
    if (root->key != key) return root;  // Key not found

    Node* temp;
    if (!root->left) {
        temp = root->right;
    } else {
        temp = splay(root->left, key);
        temp->right = root->right;
    }
    delete root;
    return temp;
}

// Inorder traversal
void inorder(Node* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->key << " ";
    inorder(root->right);
}

// Structured test
int main() {
    Node* root = nullptr;

    cout << "\n--- Splay Tree Operations ---\n";
    cout << "\nInserting elements: 10, 20, 30, 40, 50, 25\n";
    int keys[] = {10, 20, 30, 40, 50, 25};
    for (int key : keys) {
        root = insert(root, key);
        cout << "Inserted: " << key << "\n";
    }

    cout << "\nInorder traversal after insertions:\n";
    inorder(root);
    cout << "\n";

    cout << "\nSearching for key 30:\n";
    root = splay(root, 30);
    cout << "Root after splay: " << root->key << "\n";

    cout << "\nDeleting key 20:\n";
    root = deleteKey(root, 20);
    inorder(root);
    cout << "\n";

    cout << "\nDeleting key 10:\n";
    root = deleteKey(root, 10);
    inorder(root);
    cout << "\n";

    return 0;
}
