class Node:
    def __init__(self, key):
        self.key = key
        self.left = None
        self.right = None

def right_rotate(x):
    print(f"Right rotate on node with key: {x.key}")
    y = x.left
    x.left = y.right
    y.right = x
    return y

def left_rotate(x):
    print(f"Left rotate on node with key: {x.key}")
    y = x.right
    x.right = y.left
    y.left = x
    return y

def splay(root, key):
    if root is None:
        print(f"Splay: Key {key} not found, root is None")
        return root
    if root.key == key:
        print(f"Splay: Key {key} found at root")
        return root
    if key < root.key:
        if root.left is None:
            print(f"Splay: Key {key} not found in left subtree")
            return root
        if key < root.left.key:
            print(f"Splay: Zig-Zig (Right-Right) on key {key}")
            root.left.left = splay(root.left.left, key)
            root = right_rotate(root)
        elif key > root.left.key:
            print(f"Splay: Zig-Zag (Right-Left) on key {key}")
            root.left.right = splay(root.left.right, key)
            if root.left.right is not None:
                root.left = left_rotate(root.left)
        return root.left is None and root or right_rotate(root)
    else:
        if root.right is None:
            print(f"Splay: Key {key} not found in right subtree")
            return root
        if key > root.right.key:
            print(f"Splay: Zag-Zag (Left-Left) on key {key}")
            root.right.right = splay(root.right.right, key)
            root = left_rotate(root)
        elif key < root.right.key:
            print(f"Splay: Zag-Zig (Left-Right) on key {key}")
            root.right.left = splay(root.right.left, key)
            if root.right.left is not None:
                root.right = right_rotate(root.right)
        return root.right is None and root or left_rotate(root)

def insert(root, key):
    print(f"Inserting key: {key}")
    if root is None:
        print(f"Inserted key {key} as root")
        return Node(key)
    root = splay(root, key)
    if root.key == key:
        print(f"Key {key} already exists in the tree")
        return root
    new_node = Node(key)
    if key < root.key:
        print(f"Key {key} inserted to the left of root {root.key}")
        new_node.right = root
        new_node.left = root.left
        root.left = None
    else:
        print(f"Key {key} inserted to the right of root {root.key}")
        new_node.left = root
        new_node.right = root.right
        root.right = None
    return new_node

def delete_node(root, key):
    print(f"Deleting key: {key}")
    if root is None:
        print("Tree is empty")
        return None
    root = splay(root, key)
    if root.key != key:
        print(f"Key {key} not found in the tree")
        return root
    temp = root
    if root.left is None:
        print(f"Deleted key {key}, no left subtree")
        root = root.right
    else:
        print(f"Deleted key {key}, splaying left subtree")
        root = splay(root.left, key)
        root.right = temp.right
    del temp
    return root

def search(root, key):
    print(f"Searching for key: {key}")
    root = splay(root, key)
    if root and root.key == key:
        print(f"Key {key} found")
        return True
    print(f"Key {key} not found")
    return False

def inorder(root):
    if root is None:
        return
    inorder(root.left)
    print(root.key, end=" ")
    inorder(root.right)

if __name__ == "__main__":
    root = None
    root = insert(root, 10)
    root = insert(root, 20)
    root = insert(root, 30)
    root = insert(root, 40)
    root = insert(root, 50)
    root = insert(root, 25)
    
    print("Inorder traversal after insertions:")
    inorder(root)
    print()
    
    if search(root, 25):
        print("Found 25")
    else:
        print("25 not found")
    
    root = delete_node(root, 30)
    print("Inorder traversal after deletion:")
    inorder(root)
    print()