from collections import deque

def bfs(graph, start):
    visited = [False] * len(graph)
    distance = [-1] * len(graph)
    queue = deque([start])
    visited[start] = True
    distance[start] = 0

    print(f"Starting BFS from node {start}:")
    while queue:
        u = queue.popleft()
        print(f"Visiting node {u}, Distance from source: {distance[u]}")
        for v in graph[u]:
            if not visited[v]:
                visited[v] = True
                distance[v] = distance[u] + 1
                queue.append(v)
    
    print("\nFinal distances from source node:")
    for i, d in enumerate(distance):
        print(f"Node {i}: {d}")

graph = [
    [1, 2],
    [0, 3, 4],
    [0, 5],
    [1],
    [1, 5],
    [2, 4]
]
bfs(graph, 0)
