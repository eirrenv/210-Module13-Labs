#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

const int SIZE = 13;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;

class Graph {
public:
    vector<vector<Pair>> adjList;

    vector<string> labels = {
        "Main Server", "REMOVED", "Core Router", "REMOVED",
        "Data Center A", "Data Center B", "Firewall",
        "Edge Router 1", "Edge Router 2", "Client Node A",
        "Client Node B", "Backup Server", "Cloud Gateway"
    };

    Graph(vector<Edge> const &edges) {
        adjList.resize(SIZE);

        for (auto &edge : edges) {
            adjList[edge.src].push_back(make_pair(edge.dest, edge.weight));
            adjList[edge.dest].push_back(make_pair(edge.src, edge.weight));
        }
    }

    // ---------------- PRINT GRAPH ----------------
    void printGraph() {
        cout << "\nComputer Network Topology:\n";
        cout << "================================\n";

        for (int i = 0; i < SIZE; i++) {
            if (labels[i] == "REMOVED") continue;

            cout << "Node " << i << " (" << labels[i] << ") connects to:\n";

            for (Pair v : adjList[i]) {
                if (labels[v.first] == "REMOVED") continue;

                cout << "  -> Node " << v.first
                     << " (" << labels[v.first] << ")"
                     << " [Latency: " << v.second << " ms]\n";
            }
        }
    }

    // ---------------- DFS ----------------
    void DFSUtil(int v, vector<bool> &visited) {
        visited[v] = true;

        if (labels[v] == "REMOVED") return;

        cout << "Inspecting Node " << v << " (" << labels[v] << ")\n";

        for (Pair neighbor : adjList[v]) {
            if (!visited[neighbor.first] && labels[neighbor.first] != "REMOVED") {
                DFSUtil(neighbor.first, visited);
            }
        }
    }

    void DFS(int start) {
        vector<bool> visited(SIZE, false);
        cout << "\nDFS Traversal:\n";
        DFSUtil(start, visited);
    }

    // ---------------- BFS ----------------
    void BFS(int start) {
        vector<bool> visited(SIZE, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "\nBFS Traversal:\n";

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (labels[v] == "REMOVED") continue;

            cout << "Visiting " << v << " (" << labels[v] << ")\n";

            for (Pair neighbor : adjList[v]) {
                if (!visited[neighbor.first] && labels[neighbor.first] != "REMOVED") {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);
                }
            }
        }
    }

    // ---------------- DIJKSTRA ----------------
    void shortestPath(int start) {
        vector<int> dist(SIZE, numeric_limits<int>::max());
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        dist[start] = 0;
        pq.push(make_pair(0, start));

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (Pair neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        cout << "\nShortest paths from node " << start << ":\n";

        for (int i = 0; i < SIZE; i++) {
            if (labels[i] == "REMOVED") continue;

            if (dist[i] == numeric_limits<int>::max())
                cout << start << " -> " << i << " : INF\n";
            else
                cout << start << " -> " << i << " : " << dist[i] << "\n";
        }
    }

    // ---------------- MST (PRIM) ----------------
    void minimumSpanningTree() {
        vector<int> key(SIZE, numeric_limits<int>::max());
        vector<int> parent(SIZE, -1);
        vector<bool> inMST(SIZE, false);

        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        key[0] = 0;
        pq.push(make_pair(0, 0));

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (inMST[u] || labels[u] == "REMOVED") continue;

            inMST[u] = true;

            for (Pair neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (!inMST[v] && weight < key[v] && labels[v] != "REMOVED") {
                    key[v] = weight;
                    parent[v] = u;
                    pq.push(make_pair(key[v], v));
                }
            }
        }

        cout << "\nMinimum Spanning Tree edges:\n";

        for (int i = 1; i < SIZE; i++) {
            if (labels[i] == "REMOVED" || parent[i] == -1) continue;

            cout << "Edge from " << parent[i]
                 << " to " << i
                 << " with cost: " << key[i] << "\n";
        }
    }
};

int main() {
    vector<Edge> edges = {
        {0,2,15},{2,6,3},{2,4,7},{2,5,9},
        {6,7,4},{7,8,6},{8,9,2},
        {9,10,5},{10,11,8},{11,12,1},
        {4,12,10},{5,8,11},{0,7,13}
    };

    Graph graph(edges);

    int choice;

    do {
        cout << "\nComputer Network Menu:\n";
        cout << "[1] Display network\n";
        cout << "[2] BFS traversal\n";
        cout << "[3] DFS traversal\n";
        cout << "[4] Shortest paths (Dijkstra)\n";
        cout << "[5] Minimum Spanning Tree\n";
        cout << "[0] Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: graph.printGraph(); break;
            case 2: graph.BFS(0); break;
            case 3: graph.DFS(0); break;
            case 4: graph.shortestPath(0); break;
            case 5: graph.minimumSpanningTree(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice\n";
        }

    } while (choice != 0);

    return 0;
}