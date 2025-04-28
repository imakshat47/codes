MAX = 100

# Array Insertion: Worst-case O(n) (if not at end)
def insert_arr(arr, pos, val):
    arr.insert(pos, val)  # Python handles shifting automatically

# Array Deletion: Worst-case O(n) (if not at end)
def delete_arr(arr, pos):
    if 0 <= pos < len(arr):
        arr.pop(pos)

# Array Traversal: O(n)
def traverse_arr(arr):
    print(" ".join(map(str, arr)))

# Node class for Linked List
class Node:
    def __init__(self, data):
        self.data = data
        self.next = None

# Linked List Insertion at head: O(1)
def insert_ll(head, val):
    new_node = Node(val)
    new_node.next = head
    return new_node

# Linked List Deletion: Worst-case O(n) (if traversal needed)
def delete_ll(head, val):
    if not head:
        return head
    if head.data == val:
        return head.next
    prev = head
    curr = head.next
    while curr:
        if curr.data == val:
            prev.next = curr.next
            break
        prev = curr
        curr = curr.next
    return head

# Linked List Traversal: O(n)
def traverse_ll(head):
    curr = head
    while curr:
        print(curr.data, end=" ")
        curr = curr.next
    print()

# MAIN execution
if __name__ == "__main__":
    # Array operations
    print("Array Operations:")
    arr = []
    print("Initial array:", arr)
    insert_arr(arr, 0, 10)   # O(n)
    insert_arr(arr, 1, 20)   # O(1) at end
    print("After insertions:", arr)
    traverse_arr(arr)       # O(n)
    print("Array length:", len(arr))
    delete_arr(arr, 0)      # O(n)
    print("After deletion:", arr)
    traverse_arr(arr)       # O(n)

    # Linked List operations
    head = None
    print("\nLinked List Operations:")
    print("Initial linked list: None")
    head = insert_ll(head, 30)  # O(1)
    head = insert_ll(head, 40)  # O(1)
    print("After insertions:")
    traverse_ll(head)           # O(n)
    head = delete_ll(head, 30)  # O(n)
    print("After deletion:")
    traverse_ll(head)           # O(n)
