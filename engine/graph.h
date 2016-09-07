#ifndef GRAPH_H
#define GRAPH_H
#include "engine/rapidxml.h"
#include "includes.h"
#include "node.h"
#include "edge.h"
#define MAXNODES 10000
#define INFVALUE 1000000
#define qPair pair< Node*, int >
#define INF 1000000;

//comperator
struct comp {
	bool operator() (const qPair &a, const qPair &b) {
		return a.second > b.second;
	}
};


class Graph
{
public:
    Graph(string xmlPathNodes, string xmlPathEdges, bool& succesReadingXmls);
    ~Graph();
    bool ParseXmlNodes(string xmlPathNodes);
    bool ParseXmlEdges(string xmlPathEdges);
    list<Node*> GetGrapghNodes() const;
    list<Edge*> GetGrapghEdges() const;
    list<Node*> GetShortestpath(Node* start, Node* end, EdgeType pref);
    list<Node*> GetShrinkendShortestPath(list<Node*> shortestPath);

private:
	list<Node*>* _nodes;
	list<Edge*>* _edges;
};

#endif // GRAPH_H
