#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <map>
#include <algorithm>
#include <set>
#include <iomanip>
#include <string>
#include <limits>
#include <tuple>
using namespace std;

const int SIZE = 9;
const int INF = numeric_limits<int>::max();

struct BusRoute {
    int src, dest, travelTime;
};

typedef pair<int, int> RouteLink;  // Creates alias 'Pair' for the pair<int,int> data type

class BusNetwork {
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<RouteLink>> adjList;
    vector<string> stopNames;

    // Graph Constructor
    BusNetwork(vector<BusRoute> const &routes, int numStops, vector<string> names) : stopNames(names) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);

        // add edges to the directed graph
        for (auto &route: routes) {
            int src = route.src;
            int dest = route.dest;
            int travelTime = route.travelTime;

            // insert at the end
            adjList[src].push_back(make_pair(dest, travelTime));
            // for an undirected graph, add an edge from dest to src also
            adjList[dest].push_back(make_pair(src, travelTime));
        }
    }

    // Print the graph's adjacency list
    void printGraph() {
       cout << "Bus Stop Network Topology:" << endl;
        cout << "================================" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << stopNames[i] << " connects to:" << endl;
            for (RouteLink link : adjList[i]) {
                cout << "  -> " << stopNames[link.first] << " (" << link.second << " mins away)" << endl;
            }
        }
    }

    void DFS(int startVertex) {
        cout << "Inspection Route Planner (DFS) starting at " << stopNames[startVertex] << ":" << endl;
        cout << "Purpose: Ensuring all stops are visited efficiently" << endl;
        cout << "=======================================" << endl;
        set<int> visited;
        dfsUtil(startVertex, visited);
        cout << endl;
    }

    void dfsUtil(int v, set<int>& visited) {
        visited.insert(v);
        cout << "Inspecting " << stopNames[v] << endl;

        for (RouteLink neighbor : adjList[v]) {
            int nextStop = neighbor.first;
            if (visited.find(nextStop) == visited.end()) {
                 cout << "  -> Next potential stop: " << stopNames[nextStop] 
                      << " (" << neighbor.second << " mins away)" << endl;
                dfsUtil(nextStop, visited);
            }
        }
    }

    void bfs(int startVertex) {
        cout << "Service Area Analysis (BFS) from " << stopNames[startVertex] << ":" << endl;
        cout << "Purpose: Analyzing stops reachable in layers of distance" << endl;
        cout << "=================================================" << endl;
        set<int> visited;
        deque<int> q;

        q.push_back(startVertex);
        visited.insert(startVertex);

        while (!q.empty()) {
            int current = q.front();
            q.pop_front();
            cout << "Checking service for " << stopNames[current] << endl;

            for (RouteLink neighbor : adjList[current]) {
                int nextStop = neighbor.first;
                if (visited.find(nextStop) == visited.end()) {
                    visited.insert(nextStop);
                    q.push_back(nextStop);
                    cout << "  -> Next layer stop: " << stopNames[nextStop] 
                         << " (" << neighbor.second << " mins away)" << endl;
                }
            }
        }
        cout << endl;
    }

    void shortestPath(int startVertex) {
        cout << "Shortest Path Calculation from " << stopNames[startVertex] << ":" << endl;
        cout << "=================================================" << endl;
        // Use a set of (weight, vertex) pairs to simulate a priority queue.
        // The set automatically keeps elements sorted by the first element (weight).
        set<RouteLink> setds;
        vector<int> dist(SIZE, INF);

        setds.insert(make_pair(0, startVertex));
        dist[startVertex] = 0;

        while (!setds.empty()) {
            // Extract the minimum distance vertex from the set
            RouteLink tmp = *setds.begin();
            setds.erase(setds.begin());

            int u = tmp.second;

            // Iterate over all neighbors of u
            for (auto& link : adjList[u]) {
                int v = link.first;
                int weight = link.second;

                // If a shorter path to v is found through u
                if (dist[v] > dist[u] + weight) {
                    // If v is already in the set, we need to remove the old entry
                    // before inserting the new one with the smaller distance.
                    if (dist[v] != INF)
                        setds.erase(setds.find(make_pair(dist[v], v)));

                    // Update the distance and insert the new pair into the set
                    dist[v] = dist[u] + weight;
                    setds.insert(make_pair(dist[v], v));
                }
            }
        }

        // Print the shortest distances
        for (int i = 0; i < SIZE; ++i) {
            cout << startVertex << " -> " << i << " (" << stopNames[i] << ") : " 
                 << dist[i] << " mins" << endl;
        }
        cout << endl;
    }

    void minimumSpanningTree() {
        cout << "Minimum Spanning Tree Edges:" << endl;
        cout << "Purpose: Identifying essential infrastructure links to connect all stops with minimum total distance/cost" << endl;
        cout << "=======================================================" << endl;
        
        // Priority structure: (weight, source_vertex, destination_vertex)
        // We use a set of tuples to automatically sort by weight (first element)
        set<tuple<int, int, int>> edgeSet;

        vector<int> min_weight(SIZE, INF);
        vector<bool> inMST(SIZE, false);
        vector<int> parent(SIZE, -1);
        int startVertex = 0; // Start Prim's from vertex 0

        edgeSet.insert(make_tuple(0, -1, startVertex));
        min_weight[startVertex] = 0;

        while (!edgeSet.empty()) {
            // Extract the minimum weight edge from the set
            auto it = edgeSet.begin();
            int u = get<2>(*it);
            int p = get<1>(*it);
            edgeSet.erase(it);

            // If already included in MST, skip
            if (inMST[u]) continue;

            inMST[u] = true;
            parent[u] = p;

            // Add the selected edge to the output if it's not the start node itself
            if (parent[u] != -1) {
                 cout << "Edge from " << parent[u] << " (" << stopNames[parent[u]] << ") to " 
                      << u << " (" << stopNames[u] << ") with time: " 
                      << min_weight[u] << " mins" << endl;
            }

            // Iterate over all neighbors of u
            for (auto& link : adjList[u]) {
                int v = link.first;
                int weight = link.second;

                // If v is not in MST and weight of edge (u, v) is smaller than current min_weight[v]
                if (!inMST[v] && weight < min_weight[v]) {
                    // Update the minimum weight for v and insert (or update) in the set
                    min_weight[v] = weight;
                    // Note: In a set, we cannot directly update a tuple's value in place.
                    // We must erase the old entry and insert a new one if it existed.
                    // A simple insertion is fine since we check `inMST` above.
                    edgeSet.insert(make_tuple(weight, u, v));
                }
            }
        }
    }
};

int main() {
      vector<string> names = {
        "Stop A (Downtown Terminal)", "Stop B (Central Station)", "Stop C (Uptown Hub)",
        "Stop D (Market St)", "Stop E (City Hall)", "Stop F (Airport Terminal)",
        "Stop G (Industrial Park)", "Stop H (Suburban Plaza)", "Stop I (Metro Center)"
    };
    
    // Creates a vector of graph edges/weights
    vector<BusRoute> busRoute = {
        // (x, y, w) —> edge from x to y having weight w
        {0, 1, 8}, {0, 2, 21},{1, 2, 6}, {1, 3, 5}, {1, 4, 4},{2, 7, 11}, {2, 8, 8},{3, 4, 9},{5, 6, 10}, {5, 7, 15}, {5, 8, 5},{6, 7, 3}, {6, 8, 7}
    };

    // Creates graph
    BusNetwork graph(busRoute, SIZE, names);

    // Prints adjacency list representation of graph
    graph.printGraph();

    cout << "DFS starting from vertex 0:\n";
    graph.DFS(0);
    
    cout << "BFS starting from vertex 0:\n";
    graph.bfs(0);

    cout << "Calculating the shortest paths:\n";
    graph.shortestPath(0);

    cout << "Calculating the minimum spanning tree:\n";
    graph.minimumSpanningTree();

    return 0;
}