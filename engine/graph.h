#ifndef GRAPH_H
#define GRAPH_H
#include "engine/rapidxml.h"
#include "includes.h"
#include "node.h"
#include "edge.h"
#define MAXNODES 10000
#define INFVALUE 1000000
#define qPair pair< Node*, int >

//comperator
struct comp {
	bool operator() (const qPair &a, const qPair &b) {
		return a.second > b.second;
	}
};

//return nodes from GetShortestpath() in this struct
struct pathRoom {
    Node* room;
    int distance;
    string direction;
    int nextRoomInPathId;
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
    list<pathRoom> GetShortestpath(Node* start, Node* end);

private:
	list<Node*>* _nodes;
	list<Edge*>* _edges;
};

#endif // GRAPH_H
