#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <map>
#include <algorithm>
#include <set>
#include <iomanip>
#include <string>
using namespace std;

const int SIZE = 9;

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

    return 0;
}