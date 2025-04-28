MAX_SIZE = 100

def insert_element(arr, n, pos, value):
    if n >= MAX_SIZE:
        print("Array is full!")
        return n
    if pos < 0 or pos > n:
        print("Invalid position!")
        return n
    arr.insert(pos, value)
    n += 1
    print(f"Inserted {value} at position {pos}")
    return n

def delete_element(arr, n, pos):
    if pos < 0 or pos >= n:
        print("Invalid position!")
        return n
    deleted_value = arr.pop(pos)
    n -= 1
    print(f"Deleted element {deleted_value} from position {pos}")
    return n

def search_element(arr, n, value):
    if value in arr:
        return arr.index(value)
    return -1

def traverse_array(arr, n):
    print("Array elements:", *arr[:n])

def update_element(arr, n, pos, new_value):
    if pos < 0 or pos >= n:
        print("Invalid position!")
        return
    arr[pos] = new_value
    print(f"Updated position {pos} to new value {new_value}")

def main():
    arr = []
    n = 0

    n = insert_element(arr, n, 0, 10)
    n = insert_element(arr, n, 1, 20)
    n = insert_element(arr, n, 1, 15)
    traverse_array(arr, n)

    pos = search_element(arr, n, 15)
    if pos != -1:
        print(f"Element 15 found at index {pos}")
    else:
        print("Element 15 not found.")

    update_element(arr, n, 1, 17)
    traverse_array(arr, n)

    n = delete_element(arr, n, 0)
    traverse_array(arr, n)

if __name__ == "__main__":
    main()
