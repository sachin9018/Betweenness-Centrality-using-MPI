//============================================================================================
// Name        : test.cpp
// Author      : Kartik
//				 Snehal Gandham
// Version     : 1.0
// Course      : High Performance Computing
// Description : Betweenness Centrality calculation using OpenMP.
//				 Brandes Algorithm has been used for calculating Betweenness Centrality
//============================================================================================

#include <iostream>
#include <list>
#include <iostream>
#include <iterator>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;

//Method for checking if the neighbor is already present
//bool check_neigh_present(map<int, list<int> > map, int source){
//		vector<int> tmp_list = map.get(source);
//		list <int> :: iterator it;
//	    for(it = g.begin(); it != g.end(); ++it)
//	        cout << '\t' << *it;
//	    cout << '\n';
//}

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
void initialize(int V, int source_vertex, vector<int> predecessor[], int *sigma,
		int *distance, float *delta) {
	predecessor = new vector<int> [V];
	sigma = new int[V];
	distance = new int[V];
	delta = new float[V];

	for (int i = 0; i < V; i++) {
//		predecessor[i] = new vector<int>[];
		sigma[i] = 0;
		distance[i] = -1;
		delta[i] = 0;
	}

	distance[source_vertex] = 0;
	sigma[source_vertex] = 1;
}

void print_BC(float* BC, int V){
	for(int i=1;i<=V;i++)
		cout<<"Vertex  : " << i << BC[i];
}

//Method for calculating the Betweenness Centrality
void calculate_centrality(int V, vector<int> adj[], vector<int> predecessor[],
		int *sigma, int *distance, float *delta, stack<int> st,
		map<int, vector<int> > map, queue<int> q, float* CB,
		int** shortest_path_dist) {

//	iterating through each vertex
	for (int i = 1; i <= V; i++) {
		int source_vertex = i;
		initialize(V, source_vertex, predecessor, sigma, distance, delta);
		st = stack<int>();
		q.push(source_vertex);
		while (!q.empty()) {
			int vertex = 0;
			vertex = q.front();
			q.pop();
			st.push(vertex);
			for (auto &i : adj[vertex]) {

				if (distance[i] < 0) {
					q.push(i);
					distance[i] = distance[vertex] + 1;
				}

				if (distance[i] == distance[vertex] + 1) {
					sigma[i] = sigma[i] + sigma[vertex];

//					https://stackoverflow.com/questions/24139428/check-if-element-is-in-the-list-contains
					bool found = (std::find(predecessor[i].begin(),
							predecessor[i].end(), vertex)
							!= predecessor[i].end());

					if (!found)
						predecessor[i].push_back(vertex);
				}
			}

		}

		while (!st.empty()) {
			int st_neigh = st.top();
			st.pop();
			for (auto &vertex_pred : predecessor[st_neigh]) {
				float tmp_delta = delta[vertex_pred]
						+ (((float) ((float) sigma[vertex_pred]
								/ (float) sigma[st_neigh]))
								* (1 + delta[st_neigh]));
				delta[vertex_pred] += tmp_delta;
				if (source_vertex != st_neigh)
					CB[st_neigh] += delta[st_neigh];
			}
		}
		//            System.out.println("i :" + i);
		for (int i = 0; i < V; i++)
			shortest_path_dist[source_vertex][i] = distance[i];
//        shortest_path_dist[src] = distance;

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
	float * BC = new float[V];
	stack<int> st;
	map<int, vector<int> > map;
	queue<int> q;
	int **shortest_path_dist = new int*[V];
	for (int i = 0; i < V; ++i) {
		shortest_path_dist[i] = new int[V];
	}


//	Adding to the adjacency list
	edge_add(1, 2, adj);
	edge_add(2, 3, adj);
	edge_add(3, 4, adj);
	edge_add(4, 1, adj);

//	calculating the centrality
	calculate_centrality(5, adj, predecessor, sigma, distance, delta, st, map,
			q, BC, shortest_path_dist);

//	Printing the graph
	graph_print(adj, V);

//	Print Betweenness Centrality
	print_BC(BC,V);

	return 0;
}
