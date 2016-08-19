#include "edge.h"

Edge::Edge()
{
    _weight = -1;
    _floor = -1;
    _edgeType = NotInitialized;
    _node1 = NULL;
    _node2 = NULL;
}

Edge::Edge(int weight, int floor, Node* node1, Node* node2, EdgeType edgeType )
{
//    if (weight < 0) /*throw new exception("Error! weight input is negative")*/;
//    if (floor < 0) /*throw new exception("Error! floor input is out of range")*/;
//    if (node1 == NULL) /*throw new exception("Error! node1 input is NULL")*/;
//    if (node2 == NULL) /*throw new exception("Error! node2 input is NULL")*/;
    _weight = weight;
    _floor = floor;
    _edgeType = edgeType;
    _node1 = node1;
    _node2 = node2;
}

Edge::~Edge()
{
    delete(_node1);
    delete(_node2);
}

int Edge::GetWeight() const
{
    return _weight;
}

void Edge::SetWeight(int eWeight)
{
    _weight = eWeight;
}

int Edge::GetEdgeFloor() const
{
    return _floor;
}

void Edge::SetEdgeFloor(int floorNum)
{
   // if (floorNum < 0) /*throw new exception("Error!input floor number is negative")*/;
    _floor = floorNum;
}

EdgeType Edge::GetEdgeType() const
{
    return _edgeType;
}

void Edge::SetEdgeType(EdgeType eType)
{
    _edgeType = eType;
}

Node* Edge::GetNode1() const
{
    return _node1;
}

void Edge::SetNode1(Node* node1)
{
   // if (node1 == NULL) /*throw new exception("Error! input node is NULL")*/;
    _node1 = node1;
}

Node* Edge::GetNode2() const
{
    return _node2;
}

void Edge::SetNode2(Node* node2)
{
   // if (node2 == NULL) /*throw new exception("Error! input node is NULL")*/;
    _node2 = node2;
}

