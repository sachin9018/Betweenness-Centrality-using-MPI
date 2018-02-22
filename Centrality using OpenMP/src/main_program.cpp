//============================================================================================
// Name        : test.cpp
// Author      : Kartik
//				 Snehal Gandham
// Version     : 1.0
// Course      : High Performance Computing
// Description : Betweenness Centrality calculation using OpenMP.
//				 Brandes Algorith has been used for calculating Betweenness Centrality
//============================================================================================

#include <iostream>
#include <list>
#include <iostream>
#include <iterator>
#include <vector>
#include <stack>
using namespace std;

//Method for forming the adjacency list
void edge_add(int src, int dest, vector<int> adj[]) {
	cout << src << ":" << dest << "\n";
	adj[src].push_back(dest);
	adj[dest].push_back(src);
}

//Method for printing the graph
void graph_print(vector<int> adj[], int V) {

	for (int u = 0; u < V; u++) {
		cout << "Node " << u << " makes an edge with \n";
		for (auto it = adj[u].begin(); it != adj[u].end(); ++it) {
			cout << *it << ":";
		}
		cout << "\n";
	}

}

//Method for initializing the arrays and variables for each source vertex
void initialize(int V, int source_vertex, vector<int> predecessor[], int *sigma, int *distance, float *delta){
	predecessor = new vector<int>[V];
	sigma       = new int[V];
	distance    = new int[V];
	delta       = new int[V];

	for(int i=0;i<V;i++){
		predecessor[i] = new vector<int>[V];
		sigma[i]       = 0;
		distance[i]    = -1;
		delta[i]       = 0;
	}

	distance[source_vertex] = 0;
	sigma[source_vertex]    = 1;
}

//Method for calculating the Betweenness Centrality
void calculate_centrality(int V,vector<int> adj[],vector<int> predecessor[],int *sigma, int *distance,float *delta, stack<int> st) {

//	iterating through each vertex
	for(int i=1;i<=V;i++){
		int source_vertex = i;
		initialize(V,source_vertex,predecessor, sigma,distance,delta);
	}
}

int main() {

//	Declarations
	int V = 5;
	vector<int> adj[V];
	vector<int> predecessor[V];
	int *sigma = new int[V];
	int *distance = new int[V];
	float *delta = new float[V];
	stack<int> st;


//	Adding to the adjacency list
	edge_add(1, 2, adj);
	edge_add(2, 3, adj);
	edge_add(3, 4, adj);
	edge_add(4, 1, adj);

//	calculating the centrality
	calculate_centrality(5, adj,predecessor, sigma,distance,delta,st);

//	Printing the graph
	graph_print(adj, V);

	return 0;
}
