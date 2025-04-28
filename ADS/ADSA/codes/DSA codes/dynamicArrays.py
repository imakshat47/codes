class DynamicArray:
    def __init__(self):
        self._capacity = 1
        self._size = 0
        self._array = self._make_array(self._capacity)

    def _make_array(self, new_cap):
        return [None] * new_cap

    def append(self, item):
        if self._size == self._capacity:
            self._resize(2 * self._capacity)
        self._array[self._size] = item
        self._size += 1

    def _resize(self, new_cap):
        new_array = self._make_array(new_cap)
        for i in range(self._size):
            new_array[i] = self._array[i]
        self._array = new_array
        self._capacity = new_cap

    def delete(self):
        if self._size == 0:
            raise IndexError("Array is empty")
        self._size -= 1
        self._array[self._size] = None
        if self._size <= self._capacity // 4 and self._capacity > 1:
            self._resize(self._capacity // 2)

    def get(self, index):
        if not 0 <= index < self._size:
            raise IndexError("Index out of bounds")
        return self._array[index]

    def __len__(self):
        return self._size

    def __str__(self):
        return str([self._array[i] for i in range(self._size)])

# Example usage
if __name__ == "__main__":
    da = DynamicArray()
    for i in range(10):
        da.append(i)
    print("Array after appending:", da)
    da.delete()
    da.delete()
    print("Array after deletions:", da)
    print("Element at index 2:", da.get(2))