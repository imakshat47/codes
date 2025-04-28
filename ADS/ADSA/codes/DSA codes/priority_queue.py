import heapq

# Array-based Priority Queue (Unordered)
class ArrayPQ:
    def __init__(self):
        self.arr = []
        print("Initialized ArrayPQ")

    def insert(self, val):
        print(f"Inserting {val} into ArrayPQ")
        self.arr.append(val)

    def extract_min(self):
        if not self.arr:
            print("ArrayPQ is empty, cannot extract_min")
            return -1
        min_val = min(self.arr)
        print(f"\nExtracting min {min_val} from ArrayPQ")
        self.arr.remove(min_val)
        return min_val

    def get_size(self):
        size = len(self.arr)
        print(f"ArrayPQ size: {size}")
        return size


# Linked List-based Priority Queue (Sorted Insertion)
class ListNode:
    def __init__(self, data):
        self.data = data
        self.next = None

class LinkedListPQ:
    def __init__(self):
        self.head = None
        print("Initialized LinkedListPQ")

    def insert(self, val):
        print(f"Inserting {val} into LinkedListPQ")
        new_node = ListNode(val)
        if not self.head or val < self.head.data:
            new_node.next = self.head
            self.head = new_node
        else:
            curr = self.head
            while curr.next and curr.next.data <= val:
                curr = curr.next
            new_node.next = curr.next
            curr.next = new_node

    def extract_min(self):
        if not self.head:
            print("LinkedListPQ is empty, cannot extract_min")
            return -1
        min_val = self.head.data
        print(f"\nExtracting min {min_val} from LinkedListPQ")
        self.head = self.head.next
        return min_val


# Heap-based Priority Queue (Using heapq)
class HeapPQ:
    def __init__(self):
        self.heap = []
        print("Initialized HeapPQ")

    def insert(self, val):
        print(f"Inserting {val} into HeapPQ")
        heapq.heappush(self.heap, val)

    def extract_min(self):
        if not self.heap:
            print("HeapPQ is empty, cannot extract_min")
            return -1
        min_val = heapq.heappop(self.heap)
        print(f"\nExtracting min {min_val} from HeapPQ")
        return min_val


# --------- Demonstration ---------
if __name__ == "__main__":
    print("ArrayPQ:", end=" ")
    apq = ArrayPQ()
    for x in [5, 3, 8]:
        apq.insert(x)
    while apq.get_size() > 0:
        print(apq.extract_min(), end=" ")
    print()

    print("LinkedListPQ:", end=" ")
    lpq = LinkedListPQ()
    for x in [5, 3, 8]:
        lpq.insert(x)
    for _ in range(3):
        print(lpq.extract_min(), end=" ")
    print()

    print("HeapPQ:", end=" ")
    hpq = HeapPQ()
    for x in [5, 3, 8]:
        hpq.insert(x)
    for _ in range(3):
        print(hpq.extract_min(), end=" ")
    print()