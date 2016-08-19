#include "node.h"


Node::Node(string name, int floor, neighborPair (&neighbors)[NUM_OF_NEIGBHORS])
{
        static int NodesId = 1;
//    if (floor < 0) /*throw new exception("Error! floor input is out of range")*/;
//    if (name.empty()) /*throw new exception("Error! name input is empty")*/;
    _id = NodesId++;
    _floor = floor;
    _name = name;
    _previosNode = NULL;
    for (int i = 0; i < NUM_OF_NEIGBHORS; i++)
    {
        _neighbors[i] = neighbors[i];
    }
    _edgeWeightToPrevious = 0;
}

Node::~Node()
{
}

string Node::GetName() const
{
    return _name;
}

void Node::SetName(string const *name)
{
//    if ((*name).empty()) /*throw new exception("Error! input name is empty")*/;
        _name = *name;
}

int Node::GetId() const
{
    return _id;
}

int Node::GetNodeFloor() const
{
    return _floor;
}

void Node::SetNodeFloor(int floorNum)
{
//    if (floorNum < 0) /*throw new exception("Error!input floor number is negative")*/;
    _floor = floorNum;
}

int Node::GetEdgeWeightToPrevious() const
{
    return _edgeWeightToPrevious;
}

void Node::SetEdgeWeightToPrevious(int eWeight)
{
//    sif (eWeight < 0) /*throw new exception("Error!input Weight is negative")*/;

    _edgeWeightToPrevious = eWeight;
}

Node* Node::GetPreviosNode() const
{
    return _previosNode;
}

void Node::SetPreviosNode(Node* previos)
{
    _previosNode = previos;
}


 pair<Direction, string> * Node::GetNeihbors()
{
    return _neighbors;
}
