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
    string _xmlPath;
public:
    Graph(string xmlPath);
    ~Graph();
    void ParseXml(string xmlPath);
    list<Node> GetGrapghNodes() const;
    list<Edge> GetGrapghEdges() const;
};

#endif // GRAPH_H
