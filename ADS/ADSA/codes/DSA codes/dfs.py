def dfs_util(graph, u, visited):
    visited[u] = True
    print("Visited node:",u)
    for v in graph[u]:
        if not visited[v]:
            dfs_util(graph, v, visited)

def dfs(graph, start):
    visited = [False] * len(graph)
    print("Starting DFS from node",start)
    dfs_util(graph, start, visited)

graph = [
    [1, 2],
    [0, 3, 4],
    [0, 5],
    [1],
    [1, 5],
    [2, 4]
]
dfs(graph, 0)

# sudo update-alternatives --install /usr/bin/python python /usr/bin/python3 10