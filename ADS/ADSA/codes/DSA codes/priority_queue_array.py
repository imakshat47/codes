class PriorityQueue:
    def __init__(self):
        self.queue = []

    def is_empty(self):
        return len(self.queue) == 0

    def insert(self, item, priority):
        self.queue.append((item, priority))
        self.queue.sort(key=lambda x: x[1], reverse=True)  # Higher priority value means higher priority

    def delete(self):
        if not self.is_empty():
            return self.queue.pop(0)[0]
        else:
            raise IndexError("delete from empty priority queue")

    def peek(self):
        if not self.is_empty():
            return self.queue[0][0]
        else:
            raise IndexError("peek from empty priority queue")

# Example usage:
pq = PriorityQueue()
pq.insert('task1', 1)
pq.insert('task2', 3)
pq.insert('task3', 2)
print(pq.delete())  # Output: 'task2'
print(pq.peek())    # Output: 'task3'
