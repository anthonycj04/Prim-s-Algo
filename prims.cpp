#include <iostream>
#include <queue>
#include <cstdlib>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <climits>
#include <string>

using namespace std;

class Graph{
private:
	unordered_map<string, int> edges;
	unordered_set<char> vertices;
public:
	void insertEdge(char start, char end, int weight){
		vertices.insert(start);
		vertices.insert(end);
		string string1, string2;
		if (edges.find(string1.append(1, start).append(1, end)) == edges.end() && 
			edges.find(string2.append(1, end).append(1, start)) == edges.end())
			edges.insert(make_pair(string1, weight));
	}

	unordered_set<char> getVertices(){
		return vertices;
	}

	unordered_map<string, int> getEdges(){
		return edges;
	}

	int getWeight(char start, char end){
		for (auto it = edges.begin(); it != edges.end(); it++){
			if ((it->first.c_str()[0] == start && it->first.c_str()[1] == end) || 
				(it->first.c_str()[1] == start && it->first.c_str()[0] == end))
				return it->second;
		}
		cout << "error finding edge" << endl;
		exit(1);
	}
};

typedef struct node{
	char name;
	int key;
	char pi;
} Node;

class CompareFunction{
public:
	bool operator()(Node& left, Node& right){
		return left.key > right.key;
	}
};

class PriorityQueue: public priority_queue<Node, vector<Node>, CompareFunction>{
private:
	unordered_set<char> nodes;

public:
	void push(const Node& x){
		if (nodes.find(x.name) == nodes.end()){
			priority_queue::push(x);
			nodes.insert(x.name);
		}
	}

	void pop(){
		if (!priority_queue::empty()){
			nodes.erase(priority_queue::top().name);
			// Node temp = priority_queue::top();
			priority_queue::pop();
			
		}
	}

	bool find(char node){
		return nodes.find(node) != nodes.end();
	}

	Node getNode(char name){
		if (find(name)){
			PriorityQueue tempQueue;
			while (top().name != name){
				tempQueue.push(top());
				pop();
			}
			Node node = top();
			while (!tempQueue.empty()){
				push(tempQueue.top());
				tempQueue.pop();
			}
			return node;
		}
	}

	void decreaseKey(char node, int newKey, int newPi){
		if (find(node)){
			PriorityQueue tempQueue;
			while (top().name != node){
				tempQueue.push(top());
				pop();
			}
			Node node = top();
			pop();
			node.key = newKey;
			node.pi = newPi;
			push(node);
			while (!tempQueue.empty()){
				push(tempQueue.top());
				tempQueue.pop();
			}
		}
	}

	Node extractMin(){
		if (!priority_queue::empty()){
			Node node = priority_queue::top();
			pop();
			return node;
		}
		else{
			cout << "check if empty before extract min!!!" << endl;
			exit(1);
		}
	}
};

void prim(Graph graph, char startNode){
	PriorityQueue priorityQueue;
	Node node;
	node.key = INT_MAX;
	node.pi = '\0';

	// for (auto it = graph.getVertices().begin(); it != graph.getVertices().end(); it++){
		// node.name = *it;
		// priorityQueue.push(node);
	// }

	string tempString = "";
	for (auto it = graph.getVertices().begin(); it != graph.getVertices().end(); it++)
		tempString += *it;

	for (int i = 0; i < tempString.length(); i++){
		node.name = tempString.c_str()[i];
		priorityQueue.push(node);
	}
	// Node node;
	// node.key = INT_MAX;
	// node.pi = '\0';

	// node.name = 'i';
	// priorityQueue.push(node);
	// node.name = 'a';
	// priorityQueue.push(node);
	// node.name = 'b';
	// priorityQueue.push(node);
	// node.name = 'c';
	// priorityQueue.push(node);
	// node.name = 'd';
	// priorityQueue.push(node);
	// node.name = 'e';
	// priorityQueue.push(node);
	// node.name = 'f';
	// priorityQueue.push(node);
	// node.name = 'g';
	// priorityQueue.push(node);
	// node.name = 'h';
	// priorityQueue.push(node);
	

	cout << "finished inserting vertices" << endl;
	priorityQueue.decreaseKey(startNode, 0, '\0');
	while (!priorityQueue.empty()){
		Node node = priorityQueue.extractMin();
		if (node.name != node.pi && node.pi != '\0')
			cout << "(" << node.name << ", " << node.pi << ") ";
		for (auto it = graph.getEdges().begin(); it != graph.getEdges().end(); it++){
			if (it->first.c_str()[0] == node.name || it->first.c_str()[1] == node.name){// Adjacency
				char adjacency = (it->first.c_str()[0] == node.name)?it->first.c_str()[1]:it->first.c_str()[0];
				if (priorityQueue.find(adjacency) && priorityQueue.getNode(adjacency).key > graph.getWeight(node.name, adjacency))
					priorityQueue.decreaseKey(adjacency, graph.getWeight(node.name, adjacency), node.name);
			}
		}
	}
	cout << endl;
}

int main(){
	cout << "Prim's algorithm for graph in lecture" << endl;
	Graph G1;
	G1.insertEdge('a', 'b', 10);
	G1.insertEdge('a', 'c', 12);
	G1.insertEdge('b', 'c', 9);
	G1.insertEdge('b', 'd', 8);
	G1.insertEdge('c', 'e', 3);
	G1.insertEdge('c', 'f', 1);
	G1.insertEdge('d', 'e', 7);
	G1.insertEdge('d', 'g', 8);
	G1.insertEdge('d', 'h', 5);
	G1.insertEdge('e', 'f', 3);
	G1.insertEdge('f', 'h', 6);
	G1.insertEdge('g', 'h', 9);
	G1.insertEdge('g', 'i', 2);
	G1.insertEdge('h', 'i', 11);
	prim(G1, 'a');

	cout << "Prim's algorithm for graph in book" << endl;
	Graph G2;
	G2.insertEdge('a', 'b', 4);
	G2.insertEdge('a', 'h', 8);
	G2.insertEdge('b', 'c', 8);
	G2.insertEdge('b', 'h', 11);
	G2.insertEdge('c', 'd', 7);
	G2.insertEdge('c', 'f', 4);
	G2.insertEdge('c', 'i', 2);
	G2.insertEdge('d', 'e', 9);
	G2.insertEdge('d', 'f', 14);
	G2.insertEdge('e', 'f', 10);
	G2.insertEdge('f', 'g', 2);
	G2.insertEdge('g', 'h', 1);
	G2.insertEdge('g', 'i', 6);
	G2.insertEdge('h', 'i', 7);
	prim(G2, 'a');

	return 0;
}
/*
MST-PRIM(G, w, r)
	Q = NULL
	foreach u in G.V
		u.key = 9999999
		u.pi = NIL
		INSERT(Q, u)
	DECREASE-KEY(Q, r, 0)
	while (Q != NULL)
		u = EXTRACT-MIN(Q)
		foreach v in G.Adj[i]
			if (v is in Q && w(u, v) < v.key)
				v.pi = u
				DECREASE-KEY(Q, v, w(u, v))
*/
/*
Prim's algorithm for graph in lecture, starting with vertex a
(a, b) (b, d) (d, h) (h, f) (f, c) (f, e) (d, g) (g, i)
Prim's algorithm for graph in book, starting with vertex a
(a, b) (a, h) (h, g) (g, f) (f, c) (c, i) (c, d) (d, e)
*/
