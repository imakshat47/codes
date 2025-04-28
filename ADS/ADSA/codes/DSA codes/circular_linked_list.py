class Node:
    """A class representing a node in a circular linked list."""
    def __init__(self, data):
        self.data = data
        self.next = None

class CircularLinkedList:
    """A class representing a circular linked list."""
    def __init__(self):
        self.head = None

    def insert(self, data):
        """Insert a new node at the end of the list."""
        new_node = Node(data)
        if not self.head:
            self.head = new_node
            new_node.next = self.head
            return
        current = self.head
        while current.next != self.head:
            current = current.next
        current.next = new_node
        new_node.next = self.head

    def insert_at_start(self, data):
        """Insert a new node at the start of the list."""
        new_node = Node(data)
        if not self.head:
            self.head = new_node
            new_node.next = self.head
            return
        current = self.head
        while current.next != self.head:
            current = current.next
        new_node.next = self.head
        current.next = new_node
        self.head = new_node

    def delete(self, key):
        """Delete the first occurrence of a node with the given key."""
        current = self.head
        if current and current.data == key:
            print("First node to be deleted!")
            if current.next == self.head:  # Only one node
                self.head = None
                return
            else:
                while current.next != self.head:
                    current = current.next
                current.next = self.head.next
                self.head = self.head.next
                return
        prev = None
        while current and current.data != key:
            prev = current
            current = current.next
            if current == self.head:
                return
        if current:
            prev.next = current.next

    def traverse(self):
        """Traverse the list and print the elements."""
        if not self.head:
            print("List is empty")
            return
        current = self.head
        while True:
            print(current.data, end=" -> ")
            current = current.next
            if current == self.head:
                break
        print("(back to head)")

# Example usage
if __name__ == "__main__":
    cll = CircularLinkedList()
    print("Circular Linked List Example")
    print("Creating a new circular linked list...")
    print("Inserting elements...")
    cll.insert(1)
    cll.insert(2)
    cll.insert(3)
    # Traverse the list
    print("Traversing the list:")
    cll.traverse()  # Output: 1 -> 2 -> 3 -> (back to head)
    print("Inserting at start...")
    cll.insert_at_start(0)
    cll.traverse()  # Output: 0 -> 1 -> 2 -> 3 -> (back to head)
    print("Deleting a node...")
    cll.delete(2)
    cll.traverse()  # Output: 0 -> 1 -> 3 -> (back to head)
