#ifndef EDGE_H
#define EDGE_H

#include "includes.h"
#include "node.h"

enum EdgeType {Regular,Elevator,Stairs };

class Edge
{

public:
    Edge(double weight, Node* node1, Node* node2, EdgeType edgeType);
    ~Edge();

    double GetWeight() const;
    EdgeType GetEdgeType() const;
    Node* GetNode1() const;
    Node* GetNode2() const;

private:
    double _weight;
    EdgeType _edgeType;
    Node* _node1;
    Node* _node2;
};

#endif // EDGE_H
