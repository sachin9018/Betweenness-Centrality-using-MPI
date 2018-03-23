//Inspired from A Faster Algorithm for Betweenness Centrality paper.

//Takes O(nm) and O(nm + n^2log n) time on unweighted and weighted networks.

#include <iostream>
#include <cstdio>
#include <fstream>
#include <sys/time.h>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <list>
#include <mpi.h>

#include <cstring>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

double clkbegin, clkend, t;

double rtclock(void)
{
  struct timezone Tzp;
  struct timeval Tp;
  int stat;
  stat = gettimeofday (&Tp, &Tzp);
  if (stat != 0) printf("Error return from gettimeofday: %d",stat);
  return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}

// A simple neighbor struct, consisting of the target neighbor and the edge weight.
struct neighbor {
    int target;
    int weight;
    neighbor(int mTarget, int mWeight) : target(mTarget), weight(mWeight) {
    }
};

// The new adjacency list type.
typedef vector<vector<neighbor> > adjacency_list;

// BFS algorithm is used to calculate all the single source shortest paths in a non weighted graph and the source's closeness.
float bfs_SSSP(int src, int n, stack<int> &visitStack, vector<int> &sigma, list<int> *pred, adjacency_list &adjList) {
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
        for (vector<neighbor>::iterator it = adjList[v].begin(); it != adjList[v].end(); it++) {
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
    if (closeness!=0) {
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

// Prints input file statistics just after input has finished.
void printInputStats(bool isWeigthed, int n, int e) {
    ofstream out;
    out.open ("out_graph_stats.txt");
    cout << "\n==================="
            << "\nINPUT GRAPH STATS"
            << "\n>Weighted: " << boolalpha << bool(isWeigthed)
            << "\n>#ofNodes: " << n
            << "\n>#ofEdges: " << e
            << "\n===================\n\n";
    out << "Weighted: " << boolalpha << bool(isWeigthed)
            << "\n>#ofNodes: " << n
            << "\n>#ofEdges: " << e;
    out.close();
}

bool split(const string &s, int V) {
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
//		        adjList[dest].push_back(neighbor(start, 1));
	if (src > V)
		return false;
//	else return false;

	return true;
}

void read_file(string path, long V) {

	string line;
	ifstream myfile(path);
	stringstream ss(line);

	int count = 0;
	int read_count = 1;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			if (line.find("#") != string::npos)
				continue;
			if (line.find(":") != string::npos)
				continue;

			bool var = split(line, V);
			read_count++;
			if (read_count % 100000 == 0)
				cout << read_count << " read . ";
			if (!var)
				count++;
			if (count == 20)
				myfile.close();

		}
		myfile.close();
	} else
		cout << "Unable to open File_100 file" << endl;
}

// Reads an input file and fills up the adjacency list as well as the edges.
void readGraph(int &n,int &e, adjacency_list &adjList) {

//    e = 0; // Total number of edges (for statistics).
//
//
//    char * line = NULL;
//    size_t len = 0;
//    FILE * fp = argv[1];
//
//    // Find n, the total number of nodes.
//    if (getline(&line, &len, fp) != -1) {
//                strtok(line, " ");
//            n = atoi(strtok(NULL, " "));
//
//    }
//
//    // Reserve n space for adjacency list. If it fails, n was not parsed.
//    if (n) {
//        adjList.reserve(n);
//    } else {
//        cout << "Malformed input. Number of nodes undefined.";
//        exit(EXIT_FAILURE);
//    }
//    cout<<"debug 1\n";
    
//    // Read the nodes and the edges, one by one, and fill up adjList and edgeBetweenness.
//    int start, end, weight;
//    while (getline(&line, &len, fp) != -1) {
//        e += 1;
//        start = atoi(strtok(line, " "));
//        end = atoi(strtok(NULL, " "));
//        weight = 1;//atoi(strtok(NULL, " "));
//        // Check if the graph is weighted. If w<=0, the input is malformed
//        if (weight > 1) {
//            isWeigthed = true;
//        } else if(weight<=0) {
//            cout << "Malformed input. Edge w weight=0.";
//            exit(EXIT_FAILURE);
//        }

        adjList[start].push_back(neighbor(end, weight));
        adjList[end].push_back(neighbor(start, weight));
    }

    if (line) {
        free(line);
    }
    
    // Print statistics after reading.
    
}

// Clears the variables or re-initializes to 0, so that they are ready for the next loop.
void resetVariables(int src, int n, list<int> *pred, vector<int> &sigma, vector<float> &delta) {
    for (int i = 0; i < n; i++) {
        pred[i].clear();
    }

    sigma.clear();
    sigma.resize(n, 0);
    sigma[src] = 1;

    delta.clear();
    delta.resize(n, 0);
}

// Prints Closeness Centrality.
void printCloseness( int n, vector<float> closeness, bool normalize) {
    float nrml = 1;
    if (normalize) {
        nrml = 1.0/(n - 1);
    } 
    ofstream out;
    out.open ("out_closeness.txt");
    //cout << "> Closeness Centrality" << endl;    
    for (int i = 0; i < n; i++) {
        //cout << "Node " << i << ": " << closeness[i] / nrml << endl;
        out << "Node " << i << ": " << closeness[i] / nrml << endl;
    }
    out.close();
}

// Prints Node Betweenness Centrality.
void printNodeBetweenness( int n, vector<float> nodeBetweenness, bool normalize) {
    float nrml = 1;
    if (normalize) {
        nrml = (n - 1)*(n - 2);
    }
    ofstream out;
    out.open ("out_node_betweenness.txt");
    cout << endl << "> Node Betweenness Centrality" << endl;
    for (int i = 0; i < n; i++) {
        //cout << "Node " << i << ": " << nodeBetweenness[i] / nrml << endl;
        out << "Node " << i << ": " << nodeBetweenness[i] / nrml << endl;
    }
    out.close();
}

int main(void) {

    MPI_Init(NULL,NULL);

    int var_size_mpi;
    MPI_Comm_size(MPI_COMM_WORLD,&var_size_mpi);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

    int number_node,number_edge;



    // Centrality measures.
  //  map<string, float> edgeBetweenness;
    vector<float> nodeBetweenness;
    vector<float> nodeBetweenness_g;
    vector<float> closeness;
    // Input is read, and values are set to all the arguments.
    readGraph(number_node,number_edge, adjList);
    if(world_rank==0) printInputStats(false, number_node, number_edge);
    nodeBetweenness.resize(number_node, 0);
    nodeBetweenness_g.resize(number_node,0);
    closeness.resize(number_node, 0);

    list<int> pred[number_node]; // List of predecessors of node v.
    vector<int> sigma;
    vector<float> delta;
    stack<int> visitStack; // Stack that holds the inverse order of visited nodes.
    MPI_Barrier(MPI_COMM_WORLD);
    clkbegin = rtclock();
    // For each node of the graph.
    int begin_vertex = number_node/var_size_mpi * world_rank;
    int end_vertex = number_node/var_size_mpi * (world_rank + 1);
    if(world_rank == var_size_mpi-1){
        end_vertex = number_node;
    }
    for (int src = begin_vertex; src < end_vertex; src++) { 
        // Prepare the variables for the next loop.
        if(src < number_node){
            resetVariables(src, number_node, pred, sigma, delta);
            //if(world_rank==1)cout<<"my rank : "<<world_rank<<" debug 1: "<<n<<" "<<src<<endl;
            closeness[src] = bfs_SSSP(src, number_node, visitStack, sigma, pred, adjList);

            // Get the inverse order of visited nodes.
            while (!visitStack.empty()) {
                int w = visitStack.top();
                visitStack.pop();
                
                // For each predecessors of node w, do the math!
                for (list<int>::iterator it = pred[w].begin(); it != pred[w].end(); it++) {
                    int v = *it;
                    float c = ((float) sigma[v] / (float) sigma[w]) * (1.0 + delta[w]);

                    delta[v] += c;
                }
                // Node betweenness aggregation part.
                if (w != src) {
                    nodeBetweenness[w] += delta[w];
                }
            }
        }
        //cout<<"my rank "<<world_rank<<endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce( &nodeBetweenness.front(), &nodeBetweenness_g.front(), number_node, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    clkend = rtclock();
    MPI_Barrier(MPI_COMM_WORLD);
    if(world_rank == 0){
        t = clkend-clkbegin;
        
        // Printing output.
        printCloseness(number_node, closeness, true);
        printNodeBetweenness(number_node, nodeBetweenness_g, true);
        //
        cout << "\n" ;
        cout << "Time Taken : " << t;
        cout << "\n";
    }
    MPI_Barrier(MPI_COMM_WORLD);
    return 0;
}
