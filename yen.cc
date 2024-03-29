// yen's algorithm
// in which Dijkstra algorithm is adapted from:
// https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/?ref=lbp
#include "stdc++.h"
#include <iostream>
#include "yen.h"
using namespace YEN;

using namespace std;

// iPair ==>  Integer Pair
typedef pair<int, int> iPair;
typedef pair<int, vector<int> > vPair;
int inf = 1000000;

// Global definition of a graph
//Graph aGraph(15);

// This class represents a directed graph using
// adjacency list representation

Graph::Graph(int V) {
    this->V = V;
    adj = new list<iPair> [V];
} // Constructor
Graph::Graph(const Graph &rhs) { /* copy construction from rhs*/
    V = rhs.V;
    adj = rhs.adj;
    dist = rhs.dist;
}

/*
void Graph::delGraph() {
    for(int i = 0; i < V; i++) {
        while(!adj[i].empty()) {
            adj[i].pop_front();
        }
    }
}*/

// function to add an edge to graph
void Graph::addEdge(int u, int v, int w) {
    iPair possible_edge = make_pair(v, w);
bool found = (find(adj[u].begin(), adj[u].end(), possible_edge) != adj[u].end());
    if (!found){
        adj[u].push_back(possible_edge);
        adj[v].push_back(make_pair(u, w));
    }
}

void Graph::delEdge(int u, int v){
    // Traversing through the first vector list
    // and removing the second element from it
    for ( auto it = adj[u].begin(); it != adj[u].end(); it++) {
        if (it->first == v) {
            it = adj[u].erase(it);
            break;
        }
    }
    // Traversing through the second vector list
    // and removing the first element from it
    for ( auto it = adj[v].begin(); it != adj[v].end(); it++) {
        if (it->first == u) {
            it = adj[v].erase(it);
            break;
        }
    }
}


vector<int> Graph::findPath(int parent[], int s, int d){
    vector<int> path;
    path.push_back(d);
    int u = parent[d];
    if(u == -1) {
        return path;
    }
    path.insert(path.begin(), u);
    while (u != s){
        int temp = parent[u];
        u = temp;
        path.insert(path.begin(), temp);
    }
    return path;
}
// prints shortest path from s
vector<int> Graph::dijkstra(int src, int des){
        // Create a priority queue to store vertices that
    // are being preprocessed. This is weird syntax in C++.
    // Refer below link for details of this syntax
    // https://www.geeksforgeeks.org/implement-min-heap-using-stl/
    priority_queue< iPair, vector <iPair> , greater<iPair> > pq;

    // Create a vector for distances and initialize all
    // distances as infinite (INF)
    //vector<int> dist(V, INT_MAX);
    
    dist.reserve(V);
    for (int i=0; i<V; i++){
        dist[i] = inf;
    }
    // Insert source itself in priority queue and initialize
    // its distance as 0.
    pq.push(make_pair(0, src));
    dist[src] = 0;

    // Parent array to store shortest path
    int parent[V];
    for (int i=0; i<V; i++){
        parent[i] = -1;
    }
    //parent[0] = -1;

    /* Looping till priority queue becomes empty (or all
    distances are not finalized) */
    while (!pq.empty())
    {
        // The first vertex in pair is the minimum distance
        // vertex, extract it from priority queue.
        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = pq.top().second;
        pq.pop();

        // 'i' is used to get all adjacent vertices of a vertex
        list< pair<int, int> >::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = (*i).first;
            int weight = (*i).second;

            //  If there is shorted path to v through u.
            if (dist[v] > dist[u] + weight)
            {
                // Update parent of v. Update distance of v
                parent[v] = u;
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    vector<int> path = findPath(parent, src, des);
    return path;
}

// helper function to slice vector from X to Y, inclusive
vector<int> slicing(vector<int>& arr, int X, int Y) {
    // Starting and Ending iterators
    auto start = arr.begin() + X;
    auto end = arr.begin() + Y + 1;

    // To store the sliced vector
    vector<int> result(Y - X + 1);

    // Copy vector using copy function()
    copy(start, end, result.begin());

    // Return the final sliced vector
    return result;
}

// psudocode: https://en.wikipedia.org/wiki/Yen%27s_algorithm
vector<vector<int> > yen(Graph g, int s, int d, int K) {
    // Determine the shortest path from the s to the d
    vector<vector<int> > A;
    //Graph g_copy = g;
    // apply dijkstra
    vector<int> path = g.dijkstra(s, d);

    A.push_back(path);
    // Initialize the set to store the potential kth shortest path.
    priority_queue<vPair, vector<vPair>, greater<vPair> > B;

    for (int k=1; k<K; k++){
        // The spur node ranges from the first node to the next to last node in the previous k-shortest path
        for (int i=0; i<=A[k-1].size()-2; i++){
            Graph g_copy = g;
            // Spur node is retrieved from the previous k-shortest path, k − 1
            int spurNode = A[k-1][i];
            int root_dis = g.dist[spurNode];
            // The sequence of nodes from the source to the spur node of the previous k-shortest path
            vector<int> rootPath = slicing(A[k-1], 0, i);
            // for each path p in A
            for (int j = 0; j < A.size(); j++) {
                vector<int> p = A[j];
                if(A[j].size() <= i) {
                    // does not make sense to slice so skip
                    continue;
                }
                vector<int> slice = slicing(p, 0, i);

                if (equal(rootPath.begin(), rootPath.end(), slice.begin())) {
                //if (rootPath == slicing(p, 0, i)) {
                    // Remove the links that are part of the previous shortest paths which share the same root path
                    g_copy.delEdge(p[i], p[i+1]);
                }
            }

            // for each node rootPathNode in rootPath except spurNode:
            // remove rootPathNode from Graph;
            //for (int i=0; i<rootPath.size()-1; i++){
            //}
            // Calculate the spur path from the spur node to the sink.

            vector<int> spurPath = g_copy.dijkstra(spurNode, d);
            int spur_dis = g_copy.dist[d];

            // Entire path is made up of the root path and spur path
            vector<int> totalPath = rootPath;
            totalPath.insert(totalPath.end(), spurPath.begin()+1, spurPath.end());
            // Add the potential k-shortest path to the heap
            //if (totalPath not in B):
            //   B.append(totalPath);
            int total_dis = root_dis+spur_dis;
            if (B.empty() && total_dis < inf){
                B.push(make_pair(total_dis, totalPath));
            }
            priority_queue<vPair, vector<vPair>, greater<vPair> > temp;
            bool found = false;
            while (!B.empty()) {
                vPair dis_path = B.top();
                temp.push(dis_path);
                B.pop();
                if (dis_path.second == totalPath && !found){
                    found = true;             
                }
            }
            if (!found && total_dis < inf) {
                temp.push(make_pair(total_dis, totalPath));
            }
            B = temp;
            // Add back the edges and nodes that were removed from the graph
            //g_copy = g; // already happens in the begiging of this loop
        }
        if (B.empty()){
            // This handles the case of there being no spur paths, or no spur paths left.
            // This could happen if the spur paths have already been exhausted (added to A),
            // or there are no spur paths at all - such as when both the source and sink vertices
            // lie along a "dead end".
            break;
        }
        // Add the lowest cost path becomes the k-shortest path
        A.push_back(B.top().second);
        // In fact we should rather use shift since we are removing the first element
        B.pop();
    }
    cout << "size of A: " << A.size() << endl;
    return A;
}
