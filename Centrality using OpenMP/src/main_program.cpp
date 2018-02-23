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
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


//Method for forming the adjacency list
void edge_add(int src, int dest, vector<int> adj[]) {
//	cout << "Gone " << src << ":" << dest << "\n";
	adj[src].push_back(dest);
	adj[dest].push_back(src);
}

//Method for printing the graph
void graph_print(vector<int> adj[], int V) {

	for (int u = 0; u < V; u++) {
//		cout << "Node " << u << " makes an edge with \n";
		for (auto it = adj[u].begin(); it != adj[u].end(); ++it) {
//			cout << *it << ":";
		}
//		cout << "\n";
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

void print_BC(float* BC, int V) {
	cout << "\nBetweenness Centrality \n";
	for (int i = 1; i <= V; i++)
		cout << "Vertex  : " << i << " : " << BC[i] << "\n";
}

//Method for calculating the Betweenness Centrality
void calculate_centrality(int V, vector<int> adj[], vector<int> predecessor[],
		int *sigma, int *distance, float *delta, stack<int> st,
		map<int, vector<int> > map, queue<int> q, float* CB,
		int** shortest_path_dist) {

//	cout << "went in calculate centrality";

//	iterating through each vertex
	for (int i = 1; i <= V; i++) {
		int source_vertex = 1;

//		Begin of Initialization
		predecessor = new vector<int> [V];
		sigma = new int[V];
		distance = new int[V];
		delta = new float[V];

		for (int i = 0; i < V; i++) {
			sigma[i] = 0;
			distance[i] = -1;
			delta[i] = 0;
		}

		distance[source_vertex] = 0;
		sigma[source_vertex] = 1;

//		Finish of Initialization

//		for(int i=0;i<V;i++) cout<<distance[i]<<endl;
		st = stack<int>();
		q.push(source_vertex);
		while (!q.empty()) {

			int vertex = 0;
			vertex = q.front();
//			cout<<"q vertex : "<<source_vertex<<"\n";
			q.pop();
			st.push(vertex);
			for (auto &i : adj[vertex]) {
//				cout << "Vertex : " << vertex << " neighbor :" << i << endl;
				if (distance[i] < 0) {
					q.push(i);
					distance[i] = distance[vertex] + 1;
				}
//				cout << "distance[" << i << "]" << "=" << distance[i] << "\n";
				if (distance[i] == distance[vertex] + 1) {
					sigma[i] = sigma[i] + sigma[vertex];
//					cout << "sigma[" << i << "] : " << sigma[i] << endl;

//					https://stackoverflow.com/questions/24139428/check-if-element-is-in-the-list-contains
					bool found = (std::find(predecessor[i].begin(),
							predecessor[i].end(), vertex)
							!= predecessor[i].end());

					if (!found) {
						predecessor[i].push_back(vertex);
//						cout << "predessor for " << i << " is " << vertex
//								<< endl;
					}
				}
			}

		}

		while (!st.empty()) {
			int st_neigh = st.top();
			st.pop();
			for (auto &vertex_pred : predecessor[st_neigh]) {
//				cout << "neigh : " << st_neigh << " pred : " << vertex_pred
//						<< endl;

//				cout << "before:\n" << "delta[" << vertex_pred << "] : "
//						<< delta[vertex_pred];
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
//		for (int i = 0; i < V; i++)
//			shortest_path_dist[source_vertex][i] = distance[i];
//        shortest_path_dist[src] = distance;

	}
}

//	  Reference - split functionality
//	  http://ysonggit.github.io/coding/2014/12/16/split-a-string-using-c.html
void split(const string &s, char delim) {
	stringstream ss(s);
	string item;
	string main_tmp="";
	int count=0;
	while (getline(ss, item, delim)) {
		if(item.find(",")!=string::npos){
			stringstream ss(item);
			while (getline(ss, item, ',')){
				cout<<main_tmp<<" : "<<item<<endl;
			}
			count=0;
		}else if(count==1){
			count=1;
			cout<<main_tmp<<" : "<<item<<endl;
		}
		main_tmp = item;
		count++;

	}
}

// Method read file
void read_file(string path) {
//	Reference - reading the file
//	http://www.cplusplus.com/doc/tutorial/files/

	string line;
	ifstream myfile(path);
	stringstream ss(line);
	string item;
	vector<string> tokens;
//	  Reference - split functionality
//	  http://ysonggit.github.io/coding/2014/12/16/split-a-string-using-c.html
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			if (line.find("notfound") != string::npos)
				continue;
			split(line, ':');
		}
		myfile.close();
	} else
		cout << "Unable to open file";
}

int main() {

//	Declarations
	int V = 50;
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

	for (int i = 0; i < V; ++i) {
		BC[i] = 0.0;
	}

	cout << "before going in edge add\n";
//Readign from the file
	read_file("/Users/balaji/Documents/Github_New/HPC/file.txt");
//	Adding to the adjacency list
	edge_add(1, 2, adj);
	//edge_add(1, 2, adj);
	edge_add(1, 3, adj);
	edge_add(1, 4, adj);
	edge_add(2, 3, adj);
	edge_add(2, 5, adj);
	edge_add(3, 4, adj);
	edge_add(3, 5, adj);
	edge_add(4, 5, adj);
	edge_add(5, 6, adj);
	edge_add(5, 7, adj);
	edge_add(6, 7, adj);
	//edge_add(5, 1, adj);
	//edge_add(5, 3, adj);

//	check_method(V, adj, predecessor, sigma, distance, delta, st, map, q, BC,
//			shortest_path_dist);
//	cout << "before going in calculate centrality" << "\n";
//	print_BC(BC, V);

//	calculating the centrality
	calculate_centrality(V, adj, predecessor, sigma, distance, delta, st, map,
			q, BC, shortest_path_dist);

	cout << "After going in calculate centrality" << "\n";
//	print_BC(BC, V);

//	Printing the graph
//	graph_print(adj, V);

//	Print Betweenness Centrality
//	print_BC(BC,V);

	return 0;
}
