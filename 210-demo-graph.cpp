#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <map>
#include <algorithm>
#include <set>
using namespace std;

const int SIZE = 7;

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
    BusNetwork(vector<BusRoute> const &routes, int numStops, vector<string> names) {
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
        cout << "Bus Route Connections:" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << " connects to: ";
            for (RouteLink v : adjList[i])
                cout << " -> " << v.first << ") , (Number of Buses that Stop Here: " << v.second << ") ";
            cout << endl;
        }
    }

    void DFS(int startVertex) {
        set<int> visited;
        stack<int> s;

        s.push(startVertex);
        visited.insert(startVertex);
        cout << startVertex << " ";

        while (!s.empty()) {
            int current = s.top();
            s.pop();}
    }

    void dfsUtil(int v, set<int>& visited) {
        visited.insert(v);
        cout << v << " ";

        for (RouteLink neighbor : adjList[v]) {
            int nextVertex = neighbor.first;
            if (visited.find(nextVertex) == visited.end()) {
                dfsUtil(nextVertex, visited);
            }
        }
    }

    void startDFS(int startVertex) {
        set<int> visited;
        dfsUtil(startVertex, visited);
        cout << endl;
    }

    void bfs(int startVertex) {
        set<int> visited;
        deque<int> q;

        q.push_back(startVertex);
        visited.insert(startVertex);

        while (!q.empty()) {
            int current = q.front();
            q.pop_front();
            cout << current << " ";

            for (RouteLink neighbor : adjList[current]) {
                int nextVertex = neighbor.first;
                if (visited.find(nextVertex) == visited.end()) {
                    visited.insert(nextVertex);
                    q.push_back(nextVertex);
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
    graph.startDFS(0);
    
    cout << "BFS starting from vertex 0:\n";
    graph.bfs(0);

    return 0;
}