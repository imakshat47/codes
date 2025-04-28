from skiplistcollections import SkipListSet

# Create SkipListSet instance
sl = SkipListSet()
print("Step 1: Initialized an empty Skip List (Set variant).\n")

# Insert elements into the skip list
elements_to_insert = [3, 6, 7, 9, 12, 19]
print(f"Step 2: Inserting elements: {elements_to_insert}")
for key in elements_to_insert:
    sl.add(key)
    print(f"  Inserted key: {key}")

# Display the skip list contents
print("\nStep 3: Displaying all elements in Skip List (in sorted order):")
print("  Skip List Keys:", list(sl))

# Search operation
search_key = 9
print(f"\nStep 4: Searching for key {search_key}...")
print("  Result:", "Found ✅" if search_key in sl else "Not Found ❌")

# Delete operation
print(f"\nStep 5: Removing key {search_key} from the Skip List...")
sl.remove(search_key)
print(f"  Key {search_key} removed.")

# Search again after deletion
print(f"\nStep 6: Searching for key {search_key} after deletion...")
print("  Result:", "Found ✅" if search_key in sl else "Not Found ❌")

# Display updated skip list
print("\nStep 7: Updated Skip List contents:")
print("  Skip List Keys:", list(sl))

# Simulated level-based diagram (not actual skip list levels)
print("\nStep 8: Diagrammatic Representation (flat view):")
print("  ┌───" + "┬───" * (len(sl) - 1) + "┐")
print("  │ " + " │ ".join(f"{k:2}" for k in sl) + " │")
print("  └───" + "┴───" * (len(sl) - 1) + "┘")
print("  (Note: This is a flat representation. Actual skip list has multiple levels.)")
