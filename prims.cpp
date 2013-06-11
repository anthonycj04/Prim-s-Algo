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
			cout << "pushing" << endl;
			priority_queue::push(x);
			nodes.insert(x.name);
		}
	}

	void pop(){
		// if (!priority_queue::empty()){
		if (!nodes.empty()){
			cout << "popping" << endl;
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

	void decreaseKey(char node, int newKey, char newPi){
		cout << "decreaseKey(" << node << ", " << newKey << ", " << newPi << endl;
		if (find(node)){
			cout << "found" << endl;
			PriorityQueue tempQueue;
			while (top().name != node){
				cout << "popping out" << endl;
				tempQueue.push(top());
				pop();
			}
			Node node = top();
			cout << "orig: " << node.name << ", " << node.pi << ", " << node.key << endl;
			pop();
			node.key = newKey;
			node.pi = newPi;
			push(node);
			while (!tempQueue.empty()){
				cout << "pushing back" << endl;
				push(tempQueue.top());
				tempQueue.pop();
			}
		}
		cout << "leaving" << endl;
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

	cout << tempString << "!!!" << endl;
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
		cout << "extracted: " << node.name << ": " << node.key << ", " << node.pi << endl;
		if (node.name != node.pi && node.pi != '\0')
			cout << "(" << node.name << ", " << node.pi << ") " << endl;
		for (auto it = graph.getEdges().begin(); it != graph.getEdges().end(); it++){
			cout << "checking " << it->first << endl;
			if (it->first.c_str()[0] == node.name || it->first.c_str()[1] == node.name){// Adjacency
				char adjacency = (it->first.c_str()[0] == node.name)?it->first.c_str()[1]:it->first.c_str()[0];
				cout << "going for adj" << adjacency << endl;
				if (priorityQueue.find(adjacency) && priorityQueue.getNode(adjacency).key > graph.getWeight(node.name, adjacency)){
					cout << "decreasingKey: " << adjacency << ", w: " << graph.getWeight(node.name, adjacency) << ", " << node.name << endl;
					priorityQueue.decreaseKey(adjacency, graph.getWeight(node.name, adjacency), node.name);
				}
			}
		}
	}
	cout << endl;
}

int main(){
	cout << "Prim's algorithm for graph in lecture" << endl;
	Graph G1;
	G1.insertEdge('a','b',1);
	G1.insertEdge('a','c',2);
	G1.insertEdge('a','d',3);
	G1.insertEdge('b','e',4);
	G1.insertEdge('c','f',5);
	G1.insertEdge('c','g',6);
	G1.insertEdge('d','h',7);
	G1.insertEdge('e','i',11);
	G1.insertEdge('f','j',9);
	G1.insertEdge('g','j',10);
	G1.insertEdge('h','k',8);
	G1.insertEdge('i','l',12);
	G1.insertEdge('j','l',13);
	G1.insertEdge('k','l',14);

	// G1.insertEdge('a', 'b', 10);
	// G1.insertEdge('a', 'c', 12);
	// G1.insertEdge('b', 'c', 9);
	// G1.insertEdge('b', 'd', 8);
	// G1.insertEdge('c', 'e', 3);
	// G1.insertEdge('c', 'f', 1);
	// G1.insertEdge('d', 'e', 7);
	// G1.insertEdge('d', 'g', 8);
	// G1.insertEdge('d', 'h', 5);
	// G1.insertEdge('e', 'f', 3);
	// G1.insertEdge('f', 'h', 6);
	// G1.insertEdge('g', 'h', 9);
	// G1.insertEdge('g', 'i', 2);
	// G1.insertEdge('h', 'i', 11);
	prim(G1, 'a');

	cout << "Prim's algorithm for graph in book" << endl;
	Graph G2;
	G2.insertEdge('a','b',2);
	G2.insertEdge('a','c',3);
	G2.insertEdge('a','d',4);
	G2.insertEdge('b','c',4);
	G2.insertEdge('b','e',5);
	G2.insertEdge('b','i',11);
	G2.insertEdge('c','d',5);
	G2.insertEdge('c','e',1);
	G2.insertEdge('c','f',7);
	G2.insertEdge('d','f',6);
	G2.insertEdge('d','g',11);
	G2.insertEdge('e','f',8);
	G2.insertEdge('e','h',9);
	G2.insertEdge('f','g',9);
	G2.insertEdge('i','k',10);

	// G2.insertEdge('a', 'b', 4);
	// G2.insertEdge('a', 'h', 8);
	// G2.insertEdge('b', 'c', 8);
	// G2.insertEdge('b', 'h', 11);
	// G2.insertEdge('c', 'd', 7);
	// G2.insertEdge('c', 'f', 4);
	// G2.insertEdge('c', 'i', 2);
	// G2.insertEdge('d', 'e', 9);
	// G2.insertEdge('d', 'f', 14);
	// G2.insertEdge('e', 'f', 10);
	// G2.insertEdge('f', 'g', 2);
	// G2.insertEdge('g', 'h', 1);
	// G2.insertEdge('g', 'i', 6);
	// G2.insertEdge('h', 'i', 7);
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
