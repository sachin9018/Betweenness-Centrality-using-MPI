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
#include <sys/time.h>
#include <mpi.h>

using namespace std;

double clkbegin, clkend, t;

double rtclock(void) {
	struct timezone Tzp;
	struct timeval Tp;
	int stat;
	stat = gettimeofday(&Tp, &Tzp);
	if (stat != 0)
		printf("Error return from gettimeofday: %d", stat);
	return (Tp.tv_sec + Tp.tv_usec * 1.0e-6);
}

// A simple neighbor struct, consisting of the target neighbor and the edge weight.
struct neighbor {
	int target;
	int weight;
	neighbor(int mTarget, int mWeight) :
			target(mTarget), weight(mWeight) {
	}
};

// The new adjacency list type.
typedef vector<vector<neighbor> > adjacency_list;

// BFS algorithm is used to calculate all the single source shortest paths in a non weighted graph and the source's closeness.
float bfs_SSSP(int src, int n, stack<int> &visitStack, vector<int> &sigma,
		list<int> *pred, adjacency_list &adjList) {
	// Closeness counter.
	float closeness = 0;

	// Vector that holds the distances from the source.
	vector<int> dist;
	dist.resize(n, -1);
	dist[src] = 0;

	// Queue used for the Bfs algorithm.
	queue<int> visitQueue;
	visitQueue.push(src);

	// While there are still elements in the queue.
	while (!visitQueue.empty()) {
		// Pop the first.
		int v = visitQueue.front();
		visitQueue.pop();
		visitStack.push(v);

		// Closeness part aggregation.
		closeness += dist[v];

		// Check the neighbors w of v.
		for (vector<neighbor>::iterator it = adjList[v].begin();
				it != adjList[v].end(); it++) {
			int w = it->target;
			// Node w found for the first time?
			if (dist[w] < 0) {
				visitQueue.push(w);
				dist[w] = dist[v] + 1;
			}
			// Is the shortest path to w via u?
			if (dist[w] == dist[v] + 1) {
				pred[w].push_back(v);
				sigma[w] += sigma[v];
			}
		}

	}
	// Closeness part inversion.
	if (closeness != 0) {
		return 1.0 / closeness;
	} else {
		return 0;
	}
}

// Given two node indices, this function returns a string representation with
// the smallest index first, a dash, and the second index after.
string getEdgeTag(int n1, int n2) {
	ostringstream os;
	if (n1 <= n2) {
		os << n1 << "-" << n2;
	} else {
		os << n2 << "-" << n1;
	}
	return os.str();
}

void time_print(int n, int e, double t) {
	ofstream out;
	out.open("parallel_code_run.txt");

	cout << "\nParallel Run Time for "<< n <<" Nodes and " << e<< " Edges  is : "<< t;
	out << "\nParallel Run Time for "<< n <<" Nodes and " << e<< " Edges  is : "<< t;

	out.close();
}

bool split(string s, int V, adjacency_list &adjList) {
	stringstream ss(s);
	string buf;

	bool var_bool = true;
	long src = 0, dest = 0;
	while (ss >> buf) {

		if (var_bool) {
			src = stol(buf);
			var_bool = false;
		} else {
			dest = stol(buf);
			var_bool = true;
		}

	}

	if (src <= V && dest <= V)
		adjList[src].push_back(neighbor(dest, 1));
	if (src > V)
		return false;
//	else return false;

	return true;
}
// Reads an input file and fills up the adjacency list as well as the edges.
void readGraph(int &n, int &e, bool &isWeigthed, adjacency_list &adjList,
		char* input) {

	e = 0; // Total number of edges (for statistics).
	isWeigthed = false;

	char * line = NULL;
	size_t len = 0;
	FILE * fp = fopen(input, "r");
	n = 200;
	adjList.reserve(200);

	// Read the nodes and the edges, one by one, and fill up adjList and edgeBetweenness.
	int start, end, weight;

	while (getline(&line, &len, fp) != -1) {
		e += 1;
		start = atoi(strtok(line, " "));
		end = atoi(strtok(NULL, " "));

		adjList[start].push_back(neighbor(end, 1));
//        adjList[end].push_back(neighbor(start, weight));
	}

	if (line) {
		free(line);
	}

	// Print statistics after reading.

}

// Clears the variables or re-initializes to 0, so that they are ready for the next loop.
void resetVariables(int src, int n, list<int> *pred, vector<int> &sigma,
		vector<float> &delta) {
	for (int i = 0; i < n; i++) {
		pred[i].clear();
	}

	sigma.clear();
	sigma.resize(n, 0);
	sigma[src] = 1;

	delta.clear();
	delta.resize(n, 0);
}

// Prints Node Betweenness Centrality.
void betweenness_centrality_print(int n, vector<float> nodeBetweenness) {

	ofstream out;
	out.open("parallel_betweenness_centrality.txt");
	cout << endl << "> Parallel Betweenness Centrality" << endl;
	for (int i = 0; i < n; i++) {
		//cout << "Node " << i << ": " << nodeBetweenness[i] / nrml << endl;
		out << "Vertex " << i << ": "
				<< nodeBetweenness[i] / ((n - 1) * (n - 2)) << endl;
	}
	out.close();
}

int main(int argc, char* argv[]) {
	//====================
	MPI_Init(NULL, NULL);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	//====================
	int n, e; // Number of nodes
	bool isWeigthed; // Weighted graph check.
	adjacency_list adjList; // Adjacency list.

	// Centrality measures.
	//  map<string, float> edgeBetweenness;
	vector<float> nodeBetweenness;
	vector<float> nodeBetweenness_g;
	vector<float> closeness;
	// Input is read, and values are set to all the arguments.
	readGraph(n, e, isWeigthed, adjList, argv[1]);
//    if(world_rank==0) printInputStats(false, n, e);
	nodeBetweenness.resize(n, 0);
	nodeBetweenness_g.resize(n, 0);
	closeness.resize(n, 0);

	list<int> pred[n]; // List of predecessors of node v.
	vector<int> sigma;
	vector<float> delta;
	stack<int> visitStack; // Stack that holds the inverse order of visited nodes.
	MPI_Barrier (MPI_COMM_WORLD);
	double t1, t2;
	t1 = rtclock();
//    clkbegin = rtclock();
	// For each node of the graph.
	int begin_vertex = n / world_size * world_rank;
	int end_vertex = n / world_size * (world_rank + 1);
	if (world_rank == world_size - 1) {
		end_vertex = n;
	}
	for (int src = begin_vertex; src < end_vertex; src++) {
		// Prepare the variables for the next loop.
		if (src < n) {
			resetVariables(src, n, pred, sigma, delta);
			//if(world_rank==1)cout<<"my rank : "<<world_rank<<" debug 1: "<<n<<" "<<src<<endl;
			closeness[src] = bfs_SSSP(src, n, visitStack, sigma, pred, adjList);

			// Get the inverse order of visited nodes.
			while (!visitStack.empty()) {
				int w = visitStack.top();
				visitStack.pop();

				// For each predecessors of node w, do the math!
				for (list<int>::iterator it = pred[w].begin();
						it != pred[w].end(); it++) {
					int v = *it;
					float c = ((float) sigma[v] / (float) sigma[w])
							* (1.0 + delta[w]);

					delta[v] += c;
				}
				// Node betweenness aggregation part.
				if (w != src) {
					nodeBetweenness[w] += delta[w];
				}
			}
		}

	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Reduce(&nodeBetweenness.front(), &nodeBetweenness_g.front(), n,
			MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	t2 = rtclock();

	MPI_Barrier(MPI_COMM_WORLD);
	if (world_rank == 0) {
		double t = double(t2 - t1) / CLOCKS_PER_SEC;

		time_print(n, e, t);

		betweenness_centrality_print(n, nodeBetweenness_g);
		//
		cout << "\n";
		cout << "Time Taken : " << t;
		cout << "\n";
	}
	MPI_Barrier(MPI_COMM_WORLD);
	return 0;
}
