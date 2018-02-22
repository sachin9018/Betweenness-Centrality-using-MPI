//============================================================================
// Name        : test.cpp
// Author      : Kartik
//				 Snehal Gandham
// Version     : 1.0
// Course      : High Performance Computing
// Description : Betweenness Centrality calculation using OpenMP.
//				 Brandes Algorith has been used for calculating Betweenness Centrality
//============================================================================

#include <iostream>
#import <list>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;

void edge_add(int src, int dest, vector<int> adj[]) {
	cout << src << ":" << dest << "\n";
	adj[src].push_back(dest);
	adj[dest].push_back(src);
}

void graph_print(vector<int> adj[], int V) {

	for (int u = 0; u < V; u++) {
		cout << "Node " << u << " makes an edge with \n";
		for (auto it = adj[u].begin(); it != adj[u].end(); ++it) {
			cout << *it << ":";
		}
		cout << "\n";
	}

}

void initialize(int source_vertex, vector<int> predecessor[], int *sigma, int *distance, float *delta){

}
void calculate_centrality(vector<int> adj[], int V,vector<int> predecessor[],int *sigma, int *distance,float *delta) {

	//	itirating through each vertex
	for(int i=1;i<=V;i++){
		int source_vertex = i;
		initialize(source_vertex,predecessor, sigma,distance,delta);
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


//	Adding to the adjacency list
	edge_add(1, 2, adj);
	edge_add(2, 3, adj);
	edge_add(3, 4, adj);
	edge_add(4, 1, adj);

//	calculating the centrality
	calculate_centrality(adj,5,predecessor, sigma,distance,delta);

//	Printing the graph
	graph_print(adj, V);

	return 0;
}
