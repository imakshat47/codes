def topological_sort(graph):
    visited = [False] * len(graph)
    stack = []

    def dfs(u):
        visited[u] = True
        for v in graph[u]:
            if not visited[v]:
                dfs(v)
        stack.append(u)

    for i in range(len(graph)):
        if not visited[i]:
            dfs(i)
    
    print("Topological Order (for a DAG):")
    print(" -> ".join(map(str, reversed(stack))))

dag = [
    [1, 2],
    [3],
    [3],
    []
]
topological_sort(dag)
