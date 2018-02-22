/*
 * Class for Graph
 */
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <list>
#include <iterator>

using namespace std;

class Graph {
	static int vertices_total;
	static float delta[], CB[];

	static list<int> pred;
	static stack<int> st;
	static int sigma[], dist[];
	static queue<int> q;

	Graph(int v) {
		vertices_total = v;
		for (int i = 0; i < v; ++i) {
			Graph_Declarations.adj[i] = new LinkedList<Integer>();
			pred[i] = new list<int>();
			sigma[i] = 0;
			dist[i] = -1;
			CB[i] = 0;
			delta[i] = 0;

		}
	}
};
