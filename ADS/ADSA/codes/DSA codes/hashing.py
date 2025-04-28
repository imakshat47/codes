class HashTable:
    def __init__(self, size):
        # Initialize the hash table with a given size
        # Each index in the table contains an empty list to handle collisions using chaining
        self.size = size
        self.table = [[] for _ in range(size)]

    def _hash_function(self, key):
        # Compute the hash value for a given key
        # The hash value is the remainder of the hash of the key divided by the table size
        return hash(key) % self.size

    def insert(self, key, value):
        # Insert a key-value pair into the hash table
        index = self._hash_function(key)  # Get the index using the hash function
        for pair in self.table[index]:  # Check if the key already exists in the bucket
            if pair[0] == key:
                pair[1] = value  # Update the value if the key exists
                return
        self.table[index].append([key, value])  # Add the new key-value pair to the bucket

    def search(self, key):
        # Search for a value associated with a given key
        index = self._hash_function(key)  # Get the index using the hash function
        for pair in self.table[index]:  # Iterate through the bucket at the index
            if pair[0] == key:
                return pair[1]  # Return the value if the key is found
        return None  # Return None if the key is not found

    def delete(self, key):
        # Delete a key-value pair from the hash table
        index = self._hash_function(key)  # Get the index using the hash function
        for i, pair in enumerate(self.table[index]):  # Iterate through the bucket at the index
            if pair[0] == key:
                del self.table[index][i]  # Remove the key-value pair if the key is found
                return True  # Return True to indicate successful deletion
        return False  # Return False if the key is not found

    def display(self):
        # Display the contents of the hash table
        for i, bucket in enumerate(self.table):
            print(f"Bucket {i}: {bucket}")  # Print each bucket and its contents

# Example usage
ht = HashTable(10)  # Create a hash table with 10 buckets
ht.insert("apple", 100)  # Insert key-value pair ("apple", 100)
ht.insert("banana", 200)  # Insert key-value pair ("banana", 200)
ht.insert("orange", 300)  # Insert key-value pair ("orange", 300)
ht.display()  # Display the hash table
print("Search 'banana':", ht.search("banana"))  # Search for the value associated with "banana"
ht.delete("banana")  # Delete the key-value pair with key "banana"
ht.display()  # Display the hash table after deletion