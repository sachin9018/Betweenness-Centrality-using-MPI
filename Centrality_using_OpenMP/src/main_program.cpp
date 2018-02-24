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
////		cout << "Node " << u << " makes an edge with \n";
//		for (vector<int>::iterator it = adj[u].begin(); it != adj[u].end(); ++it) {
////			cout << *it << ":";
//		}
//		cout << "\n";
	}

}

//Method for initializing the arrays and variables for each source vertex
void initialize(int V, int source_vertex, vector<int> predecessor[], vector<int> vector_sigma,
		vector<int> vector_distance, vector<float> vector_delta) {
	predecessor = new vector<int> [V];
	vector_sigma.clear();
	vector_distance.clear();
	vector_delta.clear();

	for (int i = 0; i < V; i++) {
//		predecessor[i] = new vector<int>[];
		vector_sigma[i] = 0;
		vector_distance[i] = -1;
		vector_delta[i] = 0;
	}

	vector_distance[source_vertex] = 0;
	vector_sigma[source_vertex] = 1;
}

void print_BC(vector<float> BC, int V, string file_name) {
	cout << "\nBetweenness Centrality \n";
	ofstream myfile(file_name);
	if (myfile.is_open()) {
		for (int i = 1; i <= V; i++) {
//			cout << "Vertex  : " << i << " : " << BC[i] << "\n";
			myfile << i << " : ";
			myfile << BC[i] << "\n";
		}
		 myfile.close();
	} else
		cout << "Unable to open BC file"<<endl;
}

//Method for calculating the Betweenness Centrality
vector<float> calculate_centrality(int V, vector<int> adj[], vector<int> predecessor[],
		vector<int> sigma, vector<int> distance, vector<float> delta, stack<int> st,
		map<int, vector<int> > map, queue<int> q, vector<float> CB,
		int** shortest_path_dist) {

//	cout << "went in calculate centrality";

//	iterating through each vertex
//	try{
	for (int i = 1; i < V; i++) {
//		cout<<"Vertex : "<<i<<endl;
		int source_vertex = 1;

//		Begin of Initialization
		predecessor = new vector<int> [V];
//		sigma = new int[V];
//		distance = new int[V];
//		delta = new float[V];
		sigma.clear();
		delta.clear();
		distance.clear();


		for (int i = 0; i < V; i++) {
			sigma.push_back(0);
			distance.push_back(-1);
			delta.push_back(0);
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
//	} catch (const std::overflow_error& e) {
//		    // this executes if f() throws std::overflow_error (same type rule)
//			cout<<"Overflow Error"<<endl;
//			cout<<e.what();
//		} catch (const std::runtime_error& e) {
//		    // this executes if f() throws std::underflow_error (base class rule)
//			cout<<"Runtime Error"<<endl;
//			cout<<e.what();
//		} catch (const std::exception& e) {
//		    // this executes if f() throws std::logic_error (base class rule)
//			cout<<"Exception Error"<<endl;
//			cout<<e.what();
//		} catch (...) {
//		    // this executes if f() throws std::string or int or any other unrelated type
//			cout<<"Different Error"<<endl;
//	//		cout<<e.what();
//		}
	return CB;
}

//	  Reference - split functionality
//	  http://ysonggit.github.io/coding/2014/12/16/split-a-string-using-c.html
void split(const string &s, char delim, vector<int> adj[]) {
	stringstream ss(s);
	string item;
	string main_tmp = "";
	int count = 0;
	while (getline(ss, item, delim)) {
		if (item.find(",") != string::npos) {
			stringstream ss(item);
			while (getline(ss, item, ',')) {
//				cout << main_tmp << " : " << item << endl;

				edge_add(stoi(main_tmp), stoi(item), adj);
			}
			count = 0;
		} else if (count == 1) {
			count = 1;
//			cout << main_tmp << " : " << item << endl;
		}
		main_tmp = item;
		count++;

	}
}

// Method read file
void read_file(string path, vector<int> adj[]) {
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
			split(line, ':', adj);
		}
		myfile.close();
	} else
		cout << "Unable to open File_100 file"<<endl;
}

int main(int argc, char* argv[]) {

//	Declarations
	int V = 100;
	vector<int> adj[V];
	vector<int> predecessor[V];
	vector<int> vector_sigma(V);
	vector<int> vector_distance(V);
	vector<float> vector_delta(V);
	vector<float> vector_BC(V);
//	int *sigma = new int[V];
//	int *distance = new int[V];
//	float *delta = new float[V];
//	float * BC = new float[V];
	stack<int> st;
	map<int, vector<int> > map;
	queue<int> q;
	int **shortest_path_dist = new int*[V];
	ofstream out;
	string input_filename = argv[1];
	string output_filename = argv[2];
	string runtime_file = argv[3];
//	string input_filename =
//			"sgandham@hpc.oit.uci.edu:/data/users/sgandham/Betweenness-Centrality-using-OpenMp/Centrality_using_OpenMP/src/file_100.txt";
//	string output_filename = "sgandham@hpc.oit.uci.edu:/data/users/sgandham/Betweenness-Centrality-using-OpenMp/Centrality_using_OpenMP/src/BC_100.txt";
//	string runtime_file = "sgandham@hpc.oit.uci.edu:/data/users/sgandham/Betweenness-Centrality-using-OpenMp/Centrality_using_OpenMP/src/code_run_time.txt";
	clock_t t1,t2;
	for (int i = 0; i < V; ++i) {
		shortest_path_dist[i] = new int[V];
	}

	for (int i = 0; i < V; ++i) {
		vector_BC.push_back(0);
	}

//	cout << "before going in edge add\n";
//	Reading from the file
//	try{
	read_file(input_filename, adj);

	cout<<"Calculation of Betweenness Centrality Started"<<endl;
	 t1=clock();
//	calculating the centrality
	vector_BC = calculate_centrality(V, adj, predecessor, vector_sigma, vector_distance, vector_delta, st, map,
			q, vector_BC, shortest_path_dist);
	 t2=clock();
	cout<<"Calculation of Betweenness Centrality ended"<<endl;
//	float run_time = ((float)t2-(float)t1);
	double run_time = double(t2 - t1) / CLOCKS_PER_SEC;
	cout<<"Centrality calculated in : %f"<<run_time<<endl;
	out.open(runtime_file, std::ios::app);
	out<<"\nRun Time for : "<<V<<" : vertices is : ";
	out<<run_time;
	out.close();
//	print_BC(BC, V);

//	Printing the graph
//	graph_print(adj, V);

//	Print Betweenness Centrality
	print_BC(vector_BC, V, output_filename);
//	} catch (const std::overflow_error& e) {
//	    // this executes if f() throws std::overflow_error (same type rule)
//		cout<<"Overflow Error"<<endl;
//		cout<<e.what();
//	} catch (const std::runtime_error& e) {
//	    // this executes if f() throws std::underflow_error (base class rule)
//		cout<<"Runtime Error"<<endl;
//		cout<<e.what();
//	} catch (const std::exception& e) {
//	    // this executes if f() throws std::logic_error (base class rule)
//		cout<<"Exception Error"<<endl;
//		cout<<e.what();
//	} catch (...) {
//	    // this executes if f() throws std::string or int or any other unrelated type
//		cout<<"Different Error"<<endl;
////		cout<<e.what();
//	}
	return 0;
}
