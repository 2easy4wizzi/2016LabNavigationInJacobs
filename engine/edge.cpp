#include "edge.h"


Edge::Edge(double weight, Node* node1, Node* node2, EdgeType edgeType )
{
    _weight = weight;
    _edgeType = edgeType;
    _node1 = node1;
    _node2 = node2;
}

Edge::~Edge()
{
    delete(_node1);
    delete(_node2);
}

double Edge::GetWeight() const
{
    return _weight;
}

EdgeType Edge::GetEdgeType() const
{
    return _edgeType;
}

Node* Edge::GetNode1() const
{
    return _node1;
}

Node* Edge::GetNode2() const
{
    return _node2;
}


