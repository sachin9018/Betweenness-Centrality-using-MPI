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


void edge_add(int src, int dest, list<int> adj[]) {
	cout << src << ":" << dest<<"\n";
	adj[src].push_back(dest);
	adj[dest].push_back(src);
}

void graph_print(list<int> adj[], int V){

	    for (int u = 0; u < V; u++)
	    {
	        cout << "Node " << u << " makes an edge with \n";
	        for (auto it = adj[u].begin(); it!=adj[u].end(); ++it)
	        {
	        	cout<<*it <<":";
	        }
	        cout<<"\n";
	        }

}

//};

int main() {

	int V = 5;
	list<int> adj[V];
	edge_add(1, 2, adj);
	edge_add(2, 3, adj);
	edge_add(3, 4, adj);
	edge_add(4, 1, adj);
	graph_print(adj, V);

	return 0;
}
