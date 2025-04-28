# Bellman-Ford Algorithm
def bellman_ford(graph, V, source):
    # Step 1: Initialize distances from source
    dist = [float('inf')] * V
    dist[source] = 0

    # Step 2: Relax all edges V-1 times
    for _ in range(V - 1):
        for u in range(V):
            for v, weight in graph[u]:
                if dist[u] != float('inf') and dist[u] + weight < dist[v]:
                    dist[v] = dist[u] + weight

    # Step 3: Check for negative weight cycles
    for u in range(V):
        for v, weight in graph[u]:
            if dist[u] != float('inf') and dist[u] + weight < dist[v]:
                print("Graph contains negative weight cycle")
                return None

    # Return the shortest distances from the source
    return dist


# Example Usage:
# Representing the graph as an adjacency list where each edge is (destination, weight)
graph = [
    [(1, -1), (2, 4)],          # 0 -> 1 (-1), 0 -> 2 (4)
    [(2, 3), (3, 2), (4, 2)],   # 1 -> 2 (3), 1 -> 3 (2), 1 -> 4 (2)
    [(3, 5)],                   # 2 -> 3 (5)
    [(4, -3)],                  # 3 -> 4 (-3)
    []                          # No outgoing edges from 4
]

V = 5  # Number of vertices
source = 0  # Source vertex

# Running Bellman-Ford algorithm
distances = bellman_ford(graph, V, source)

if distances:
    print("Shortest distances from source vertex (Bellman-Ford)", source)
    for i, dist in enumerate(distances):
        print(f"Distance from {source} to {i}: {dist}")
