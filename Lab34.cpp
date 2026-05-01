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

        for (auto &edge: edges) {
            adjList[edge.src].push_back(make_pair(edge.dest, edge.weight));
            adjList[edge.dest].push_back(make_pair(edge.src, edge.weight));
        }
    }

    // -------- Pretty Print --------
    void printGraph() {
        cout << "Computer Network Topology:\n";
        cout << "================================\n";

        for (int i = 0; i < adjList.size(); i++) {
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

    // -------- DFS --------
    void DFSUtil(int v, vector<bool> &visited) {
        visited[v] = true;

        if (labels[v] == "REMOVED") return;

        cout << "Inspecting Node " << v << " (" << labels[v] << ")\n";

        for (Pair neighbor : adjList[v]) {
            if (!visited[neighbor.first] && labels[neighbor.first] != "REMOVED") {
                cout << "  -> Possible path to Node " << neighbor.first
                     << " (" << labels[neighbor.first] << ")"
                     << " [Latency: " << neighbor.second << " ms]\n";

                DFSUtil(neighbor.first, visited);
            }
        }
    }

    void DFS(int start) {
        vector<bool> visited(SIZE, false);

        cout << "\nNetwork Trace (DFS) from Node " << start
             << " (" << labels[start] << "):\n";
        cout << "Purpose: Tracing packet routes / failures\n";
        cout << "=======================================\n";

        DFSUtil(start, visited);
    }

    // -------- BFS --------
    void BFS(int start) {
        vector<bool> visited(SIZE, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "\nLayer-by-Layer Network Scan (BFS) from Node "
             << start << " (" << labels[start] << "):\n";
        cout << "Purpose: Finding closest reachable systems\n";
        cout << "=========================================\n";

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (labels[v] == "REMOVED") continue;

            cout << "Checking Node " << v << " (" << labels[v] << ")\n";

            for (Pair neighbor : adjList[v]) {
                if (!visited[neighbor.first] && labels[neighbor.first] != "REMOVED") {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);

                    cout << "  -> Next reachable: Node " << neighbor.first
                         << " (" << labels[neighbor.first] << ")"
                         << " [Latency: " << neighbor.second << " ms]\n";
                }
            }
        }
    }

    // -------- Dijkstra (Shortest Path) --------
    void shortestPath(int start) {
        vector<int> dist(SIZE, numeric_limits<int>::max());

        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        dist[start] = 0;
        pq.push(make_pair(0, start));

        while (!pq.empty()) {
            int current = pq.top().second;
            int currentDist = pq.top().first;
            pq.pop();

            for (Pair neighbor : adjList[current]) {
                int next = neighbor.first;
                int weight = neighbor.second;

                if (dist[current] + weight < dist[next]) {
                    dist[next] = dist[current] + weight;
                    pq.push(make_pair(dist[next], next));
                }
            }
        }

        cout << "\nShortest path from node " << start << ":\n";

        for (int i = 0; i < SIZE; i++) {
            if (labels[i] == "REMOVED") continue;

            if (dist[i] == numeric_limits<int>::max())
                cout << start << " -> " << i << " : INF\n";
            else
                cout << start << " -> " << i << " : " << dist[i] << endl;
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

    graph.printGraph();

    graph.DFS(0);
    graph.BFS(0);

    graph.shortestPath(0);

    return 0;
}