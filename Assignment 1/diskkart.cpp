#include <iostream>
#include <climits>
using namespace std;

#define V 5   // Number of vertices

// Function to find vertex with minimum distance value
int minDistance(int dist[], bool visited[]) {
    int minVal = INT_MAX, minIndex = -1;

    for (int i = 0; i < V; i++)
        if (!visited[i] && dist[i] <= minVal)
            minVal = dist[i], minIndex = i;

    return minIndex;
}

// Dijkstra Algorithm using adjacency matrix
void dijkstra(int graph[V][V], int src) {
    int dist[V];      
    bool visited[V];  

    // Initialize distances
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
    }

    dist[src] = 0; // distance of source is 0

    // Find shortest path to all vertices
    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, visited);
        visited[u] = true;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] != 0 &&
                dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) 
            {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // Print shortest distances
    cout << "Shortest distances from source " << src << ":\n";
    for (int i = 0; i < V; i++)
        cout << dist[i] << " ";
}

int main() {
    // Adjacency Matrix (5×5)
    int graph[V][V] = {
        {0, 4, 8, 0, 0},
        {0, 0, 0, 0, 6},
        {0, 0, 0, 2, 0},
        {0, 0, 0, 0,10},
        {0, 0, 0, 0, 0}
    };

    int src = 0;

    dijkstra(graph, src);

    return 0;
}
