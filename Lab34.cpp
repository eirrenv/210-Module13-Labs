#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int SIZE = 7;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;

class Graph {
public:
    vector<vector<Pair>> adjList;

    Graph(vector<Edge> const &edges) {
        adjList.resize(SIZE);

        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            adjList[src].push_back(make_pair(dest, weight));
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    void printGraph() {
        cout << "Graph's adjacency list:" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << " --> ";
            for (Pair v : adjList[i])
                cout << "(" << v.first << ", " << v.second << ") ";
            cout << endl;
        }
    }

    // -------- DFS --------
    void DFSUtil(int v, vector<bool> &visited) {
        visited[v] = true;
        cout << v << " ";

        for (Pair neighbor : adjList[v]) {
            if (!visited[neighbor.first])
                DFSUtil(neighbor.first, visited);
        }
    }

    void DFS(int start) {
        vector<bool> visited(SIZE, false);
        DFSUtil(start, visited);
        cout << endl;
    }

    // -------- BFS --------
    void BFS(int start) {
        vector<bool> visited(SIZE, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            cout << v << " ";

            for (Pair neighbor : adjList[v]) {
                if (!visited[neighbor.first]) {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);
                }
            }
        }
        cout << endl;
    }
};

int main() {
    vector<Edge> edges = {
        {0,1,12},{0,2,8},{0,3,21},
        {2,3,6},{2,6,2},{5,6,6},
        {4,5,9},{2,4,4},{2,5,5}
    };

    Graph graph(edges);

    graph.printGraph();

    cout << "DFS starting from vertex 0:" << endl;
    graph.DFS(0);

    cout << "BFS starting from vertex 0:" << endl;
    graph.BFS(0);

    return 0;
}