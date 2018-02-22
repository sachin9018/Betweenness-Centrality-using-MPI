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

//class Graph {
//public:
	void edge_add(int src, int dest,vector<int> adj[]) {
		cout<<src<<":"<<dest;
		adj[src].push_back(dest);
		adj[dest].push_back(src);
//	}
//};

class GraphDeclarations{
public:

//    https://stackoverflow.com/questions/17897943/array-of-stdlist-c
    typedef list<int> L;
    int size=5;
//    static vector<int> adj[5];
};

int main() {

//	Graph g;
	vector<int> adj[5];
	edge_add(1, 2,adj);
	edge_add(2, 3,adj);
	edge_add(3, 4,adj);
	edge_add(4, 1,adj);

	for(int i=1;i<6;i++) {
		for (auto it = adj[i].begin(); it!=adj[i].end(); it++)
			cout<<*it;
	}
	return 0;
}
