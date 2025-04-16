#include <iostream>

class SplayTree {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;

        Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;

    // Right rotate
    Node* rotateRight(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        y->right = x;
        return y;
    }

    // Left rotate
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    // Splay operation
    Node* splay(Node* root, int key) {
        if (!root || root->key == key)
            return root;

        // Key lies in left subtree
        if (key < root->key) {
            if (!root->left) return root;

            // Zig-Zig (Left Left)
            if (key < root->left->key) {
                root->left->left = splay(root->left->left, key);
                root = rotateRight(root);
            }
            // Zig-Zag (Left Right)
            else if (key > root->left->key) {
                root->left->right = splay(root->left->right, key);
                if (root->left->right)
                    root->left = rotateLeft(root->left);
            }

            return root->left ? rotateRight(root) : root;
        }
        // Key lies in right subtree
        else {
            if (!root->right) return root;

            // Zag-Zig (Right Left)
            if (key < root->right->key) {
                root->right->left = splay(root->right->left, key);
                if (root->right->left)
                    root->right = rotateRight(root->right);
            }
            // Zag-Zag (Right Right)
            else if (key > root->right->key) {
                root->right->right = splay(root->right->right, key);
                root = rotateLeft(root);
            }

            return root->right ? rotateLeft(root) : root;
        }
    }

public:
    // Search and splay
    bool search(int key) {
        root = splay(root, key);
        return root && root->key == key;
    }

    // Insert and splay
    void insert(int key) {
        if (!root) {
            root = new Node(key);
            return;
        }

        root = splay(root, key);

        if (key == root->key) return; // Key already exists

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

        root = newNode;
    }

    // Delete
    void remove(int key) {
        if (!root) return;

        root = splay(root, key);

        if (root->key != key) return; // Not found

        Node* temp;
        if (!root->left) {
            temp = root->right;
        } else {
            temp = splay(root->left, key); // Move max of left subtree to root
            temp->right = root->right;
        }

        delete root;
        root = temp;
    }

    // In-order traversal
    void inorder(Node* node) {
        if (!node) return;
        inorder(node->left);
        std::cout << node->key << " ";
        inorder(node->right);
    }

    void printInorder() {
        std::cout << "In-order: ";
        inorder(root);
        std::cout << "\n";
    }
};

int main() {
    SplayTree st;

    st.insert(10);
    st.insert(20);
    st.insert(30);
    st.insert(40);
    st.insert(50);
    st.printInorder();  // Should print: 10 20 30 40 50

    std::cout << "Searching 30...\n";
    if (st.search(30))
        std::cout << "Found!\n";
    else
        std::cout << "Not found!\n";

    st.printInorder();  // Tree is now splayed, 30 is at root

    std::cout << "Deleting 20...\n";
    st.remove(20);
    st.printInorder();

    return 0;
}
