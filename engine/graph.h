#ifndef GRAPH_H
#define GRAPH_H
#include "engine/rapidxml.h"
#include "includes.h"
#include "node.h"
#include "edge.h"
#define MAXNODES 10000
#define INFVALUE 1000000
#define qPair pair< Node*, int >

struct comp {
	bool operator() (const qPair &a, const qPair &b) {
		return a.second > b.second;
	}
};
struct pathRoom { Node* room; int distance; string direction; };

class Graph
{
private:
	priority_queue< qPair, vector< qPair >, comp > Queue;
	vector< qPair > Adjacency[MAXNODES];
	int distanceV[MAXNODES];
	bool wasChecked[MAXNODES];
	list<Node*>* _nodes;
	list<Edge*>* _edges;
	list<pathRoom>* _shortestPath;
public:
    Graph(string xmlPathNodes,string xmlPathEdges);
    ~Graph();
    void ParseXmlNodes(string xmlPathNodes);
    void ParseXmlEdges(string xmlPathEdges);
	list<Node*> GetGrapghNodes() const;
	list<Edge*> GetGrapghEdges() const;
	Node* GetNodeByName(string nodeName);
	list<pathRoom> GetShortestpath(Node* start, Node* end);
};

#endif // GRAPH_H
