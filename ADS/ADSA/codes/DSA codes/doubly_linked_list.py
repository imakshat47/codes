class Node:
    """A class representing a node in a doubly linked list."""
    def __init__(self, data):
        self.data = data
        self.next = None
        self.prev = None

class DoublyLinkedList:
    """A class representing a doubly linked list."""
    def __init__(self):
        self.head = None

    def insert(self, data):
        """Insert a new node at the end of the list."""
        new_node = Node(data)
        if not self.head:
            self.head = new_node
            return
        current = self.head
        while current.next:
            current = current.next
        current.next = new_node
        new_node.prev = current

    def insert_at_start(self, data):
        new_node = Node(data)
        if not self.head:
            self.head = new_node
            return
        current = self.head
        self.head = new_node
        current.prev = new_node
        new_node.next = current

    def delete(self, key):
        """Delete the first occurrence of a node with the given key."""
        current = self.head
        if current and current.data == key:
            print("First Node to be deleted!")
            self.head = current.next
            if self.head:
                self.head.prev = None
            return
        while current and current.data != key:
            current = current.next
        if current:
            if current.next:
                current.next.prev = current.prev
            if current.prev:
                current.prev.next = current.next

    def traverse(self):
        """Traverse the list and print the elements."""
        current = self.head
        while current:
            print(current.data, end=" <-> ")
            current = current.next
        print("None")

# Example usage
if __name__ == "__main__":
    print("Doubly Linked List Example")
    print("Creating a new doubly linked list...")
    print("Inserting elements...")
    dll = DoublyLinkedList()
    dll.insert(1)
    dll.insert(2)
    dll.insert(3)
    # Traverse the list
    print("Traversing the list:")
    dll.traverse()  # Output: 1 <-> 2 <-> 3 <-> None
    # Insert at start
    print("Inserting at start...")
    dll.insert_at_start(0)
    dll.traverse()  # Output: 0 <-> 1 <-> 2 <-> 3 <-> None
    # Delete a node
    print("Deleting a node...")
    dll.delete(2)
    dll.traverse()  # Output: 1 <-> 3 <-> None
