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
void edge_add(long src, long dest, vector<long> adj[]) {
	cout << src << ":" << dest << endl;
	adj[src].push_back(dest);
	adj[dest].push_back(src);
}

//Method for prlonging the graph
void graph_prlong(vector<long> adj[], long V) {

	for (long u = 0; u < V; u++) {
////		cout << "Node " << u << " makes an edge with \n";
//		for (vector<long>::iterator it = adj[u].begin(); it != adj[u].end(); ++it) {
////			cout << *it << ":";
//		}
//		cout << "\n";
	}

}

//Method for initializing the arrays and variables for each source vertex
void initialize(long V, long source_vertex, vector<long> predecessor[],
		vector<long> vector_sigma, vector<long> vector_distance,
		vector<float> vector_delta) {
	predecessor = new vector<long> [V];
	vector_sigma.clear();
	vector_distance.clear();
	vector_delta.clear();

	for (long i = 0; i < V; i++) {
//		predecessor[i] = new vector<long>[];
		vector_sigma[i] = 0;
		vector_distance[i] = -1;
		vector_delta[i] = 0;
	}

	vector_distance[source_vertex] = 0;
	vector_sigma[source_vertex] = 1;
}

void prlong_BC(vector<float> BC, long V, string file_name) {
	cout << "\nBetweenness Centrality \n";
	ofstream myfile(file_name);
	if (myfile.is_open()) {
		for (long i = 1; i <= V; i++) {
//			cout << "Vertex  : " << i << " : " << BC[i] << "\n";
			myfile << i << " : ";
			myfile << BC[i] << "\n";
		}
		myfile.close();
	} else
		cout << "Unable to open BC file" << endl;
}

//Method for calculating the Betweenness Centrality
vector<float> calculate_centrality(long V, vector<long> adj[],
		vector<long> predecessor[], stack<long> st,
		queue<long> q, vector<float> CB) {

//	cout << "went in calculate centrality";

//	iterating through each vertex
//	try{
	for (long i = 1; i < V; i++) {
		cout << "Vertex : " << i << endl;
		long source_vertex = 1;

//		Begin of Initialization
		predecessor = new vector<long> [V];
		vector<long> sigma(V,0);
		vector<long> distance(V,-1);
		vector<long> delta(V,0);
//		sigma.clear();
//		delta.clear();
//		distance.clear();
//
//		for (long i = 0; i < V; i++) {
//			sigma.push_back(0);
//			distance.push_back(-1);
//			delta.push_back(0);
//		}

		distance[source_vertex] = 0;
		sigma[source_vertex] = 1;

//		Finish of Initialization

//		for(long i=0;i<V;i++) cout<<distance[i]<<endl;
		st = stack<long>();
		q.push(source_vertex);
		while (!q.empty()) {

			long vertex = 0;
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
			long st_neigh = st.top();
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
		//            System.out.prlongln("i :" + i);
//		for (long i = 0; i < V; i++)
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
//		    // this executes if f() throws std::string or long or any other unrelated type
//			cout<<"Different Error"<<endl;
//	//		cout<<e.what();
//		}
	return CB;
}

//	  Reference - split functionality
//	  http://ysonggit.github.io/coding/2014/12/16/split-a-string-using-c.html
void split(const string &s, vector<long> adj[], long V) {
	stringstream ss(s);
	vector<string> tokens;
	string buf;
	vector<long> vect(2);
	bool var_bool = true;
	long src = 0, dest = 0;
	while (ss >> buf) {
//		vect.push_back(stol(buf));
		if (var_bool) {
			src = stol(buf);
			var_bool = false;
		} else {
			dest = stol(buf);
			var_bool = true;
		}

//		cout<<buf;
//		cout<<" : ";
	}
//	cout<<endl;
	if (src < V && dest < V)
		edge_add(src, dest, adj);
//	string item;
//	string main_tmp = "";
//	long count = 0;
//	while (getline(ss, item, delim)) {
//		if (item.find(",") != string::npos) {
//			stringstream ss(item);
//			while (getline(ss, item, ',')) {
////				cout << main_tmp << " : " << item << endl;
//
//				edge_add(stol(main_tmp), stol(item), adj);
//			}
//			count = 0;
//		} else if (count == 1) {
//			count = 1;
////			cout << main_tmp << " : " << item << endl;
//		}
//		main_tmp = item;
//		count++;
//
//	}
}

// Method read file
void read_file(string path, vector<long> adj[], long V) {
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
			cout << line << endl;
			if (line.find("#") != string::npos)
				continue;
			if (line.find(":") != string::npos)
				continue;
			split(line, adj, V);
//			if(++count>100) break;
		}
		myfile.close();
	} else
		cout << "Unable to open File_100 file" << endl;
}

int main(int argc, char* argv[]) {

//	Declarations
	long V = stol(argv[4]);
	cout<<"Vertex declared";
	vector<long> adj[V];
	cout<<"adj declared";
	vector<long> predecessor[V];
	cout<<"Predecessor declared";
	vector<float> vector_BC(V,0);
	cout<<"vector_BC declared";
	stack<long> st;
	cout<<"vector_BC declared";
//	map<long, vector<long> > map;
	queue<long> q;
	string input_filename = argv[1];
	string output_filename = argv[2];
	string runtime_file = argv[3];
	ofstream out;
	clock_t t1, t2;
//	vector<long> vector_sigma(V);
//	vector<long> vector_distance(V);
//	vector<float> vector_delta(V);

//	long *sigma = new long[V];
//	long *distance = new long[V];
//	float *delta = new float[V];
//	float * BC = new float[V];

//	long **shortest_path_dist = new long*[V];


//	string input_filename =
//			"sgandham@hpc.oit.uci.edu:/data/users/sgandham/Betweenness-Centrality-using-OpenMp/Centrality_using_OpenMP/src/file_100.txt";
//	string output_filename = "sgandham@hpc.oit.uci.edu:/data/users/sgandham/Betweenness-Centrality-using-OpenMp/Centrality_using_OpenMP/src/BC_100.txt";
//	string runtime_file = "sgandham@hpc.oit.uci.edu:/data/users/sgandham/Betweenness-Centrality-using-OpenMp/Centrality_using_OpenMP/src/code_run_time.txt";

//	for (long i = 0; i < V; ++i) {
//		shortest_path_dist[i] = new long[V];
//	}

//	for (long i = 0; i < V; ++i) {
//		vector_BC.push_back(0);
//	}

//	cout << "before going in edge add\n";
//	Reading from the file
//	try{
	cout<<"Started Reading the file";
	read_file(input_filename, adj, V);

	cout << "Calculation of Betweenness Centrality Started" << endl;
	t1 = clock();
//	calculating the centrality
	vector_BC = calculate_centrality(V, adj, predecessor,st,q, vector_BC);
	t2 = clock();
	cout << "Calculation of Betweenness Centrality ended" << endl;
//	float run_time = ((float)t2-(float)t1);
	double run_time = double(t2 - t1) / CLOCKS_PER_SEC;
	cout << "Centrality calculated in : %f" << run_time << endl;
	out.open(runtime_file, std::ios::app);
	out << "\nRun Time for : " << V << " : vertices is : ";
	out << run_time;
	out.close();
//	prlong_BC(BC, V);

//	Prlonging the graph
//	graph_prlong(adj, V);

//	Prlong Betweenness Centrality
	prlong_BC(vector_BC, V, output_filename);
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
//	    // this executes if f() throws std::string or long or any other unrelated type
//		cout<<"Different Error"<<endl;
////		cout<<e.what();
//	}
	return 0;
}
