import heapq

def dijkstra(graph, start):
    dist = [float('inf')] * len(graph)
    dist[start] = 0
    pq = [(0, start)]

    print(f"Starting Dijkstra's Algorithm from node {start}:")
    while pq:
        d, u = heapq.heappop(pq)
        if d > dist[u]:
            continue
        for v, w in graph[u]:
            if dist[u] + w < dist[v]:
                dist[v] = dist[u] + w
                heapq.heappush(pq, (dist[v], v))
    
    print("\nShortest distances from source node:")
    for i, d in enumerate(dist):
        print(f"Node {i}: {d}")

wgraph = [
    [(1, 4), (2, 1)],
    [(3, 1)],
    [(1, 2), (3, 5)],
    []
]
dijkstra(wgraph, 0)
