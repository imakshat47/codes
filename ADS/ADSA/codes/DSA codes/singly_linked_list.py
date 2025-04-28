class Node:
    """A class representing a node in a singly linked list."""
    def __init__(self, data):
        self.data = data
        self.next = None

class SinglyLinkedList:
    """A class representing a singly linked list."""
    def __init__(self):
        self.head = None

    def insert(self, data):
        """Insert a new node at the end of the list."""
        new_node = Node(data)
        if not self.head:
            print("First Node!")
            self.head = new_node
            return
        current = self.head
        while current.next:
            current = current.next
        current.next = new_node
        print("Node Added!")

    def insert_at_start(self, data):
        new_node = Node(data)
        if not self.head:
            self.head = new_node
            return
        current = self.head
        new_node.next = current
        self.head = new_node

    def delete(self, key):
        """Delete the first occurrence of a node with the given key."""
        current = self.head
        if current and current.data == key:
            print("First node to be deleted!")
            self.head = current.next
            return
        prev = None
        while current and current.data != key:
            prev = current
            current = current.next
        if current:
            print("Node found!")
            prev.next = current.next

    def traverse(self):
        """Traverse the list and print the elements."""
        current = self.head
        while current:
            print(current.data, end=" -> ")
            current = current.next
        print("None")

# Example usage
if __name__ == "__main__":
    print("Singly Linked List Example")
    print("Creating a new singly linked list...")
    print("Inserting nodes...")
    # Create a new singly linked list and insert some nodes
    linked_list = SinglyLinkedList()
    linked_list.insert(1)
    linked_list.insert(2)
    linked_list.insert(3)
    # Traverse the list
    print("Traversing the list:")
    linked_list.traverse()  # Output: 1 -> 2 -> 3 -> None
    # Insert at the start
    print("Inserting at the start:")
    linked_list.insert_at_start(0)
    linked_list.traverse()  # Output: 0 -> 1 -> 2 -> 3 -> None
    # Delete a node
    print("Deleting a node:")
    linked_list.delete(3)
    linked_list.traverse()  # Output: 1 -> 3 -> None
