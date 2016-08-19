#ifndef GRAPH_H
#define GRAPH_H
#include "engine/rapidxml.h"
#include "includes.h"

//class Node;
#include "node.h"
//class Edge;
#include "edge.h"

class Graph
{
private:
    list<Node>* _nodes;
    list<Edge>* _edges;
public:
    Graph(string xmlPath);
    ~Graph();
    list<Node> GetGrapghNodes() const;
    list<Edge> GetGrapghEdges() const;
    void AddToNodesList(Node* node);
    void AddToEdgesList(Edge* edge);
    //add func for return size of _nodes, _edges ?
};

#endif // GRAPH_H
