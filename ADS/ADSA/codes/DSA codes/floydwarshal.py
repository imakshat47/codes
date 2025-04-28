def floyd_warshall(matrix):
    n = len(matrix)
    dist = [row[:] for row in matrix]  # Deep copy

    print("Running Floyd–Warshall Algorithm...")
    for k in range(n):
        print(f"Using node {k} as an intermediate:")
        for i in range(n):
            for j in range(n):
                if dist[i][k] != float('inf') and dist[k][j] != float('inf'):
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])

    print("\nAll-pairs shortest path matrix:")
    for row in dist:
        print(" ".join(['INF' if x == float('inf') else str(x) for x in row]))

matrix = [
    [0, 3, 1, float('inf')],
    [float('inf'), 0, float('inf'), 2],
    [float('inf'), 1, 0, 4],
    [float('inf'), float('inf'), float('inf'), 0]
]
floyd_warshall(matrix)
