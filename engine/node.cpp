#include "node.h"

Node::Node()
{
    int NodesId = 1;
    _id = NodesId++;
    _floor = -1;
    _edgeWeightToPrevious = 0;
    _name = "";
    _previosNode = NULL;
    _neighbors[0] = neighborPair(North, "");
    _neighbors[1] = neighborPair(East, "");
    _neighbors[2] = neighborPair(South, "");
    _neighbors[3] = neighborPair(West, "");
}

Node::Node(string name, int floor, neighborPair (&neighbors)[NUMOFNEIGBHORS])
{
        static int NodesId = 1;
    if (floor < 0) /*throw new exception("Error! floor input is out of range")*/;
    if (name.empty()) /*throw new exception("Error! name input is empty")*/;
    _id = NodesId++;
    _floor = floor;
    _name = name;
    _previosNode = NULL;
    for (int i = 0; i < NUMOFNEIGBHORS; i++)
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
    if ((*name).empty()) /*throw new exception("Error! input name is empty")*/;
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
    if (floorNum < 0) /*throw new exception("Error!input floor number is negative")*/;
    _floor = floorNum;
}

int Node::GetEdgeWeightToPrevious() const
{
    return _edgeWeightToPrevious;
}

void Node::SetEdgeWeightToPrevious(int eWeight)
{
    if (eWeight < 0) /*throw new exception("Error!input Weight is negative")*/;

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

const basic_string<char>Node::GetNeighborDirection(string const *neighborName)
{
    string *result = NULL;
    if ((*neighborName).empty()) return NULL;
    for (size_t i = 0; i < NUMOFNEIGBHORS; i++)
    {
        if (_neighbors[i].second == *neighborName)
            return direction_names[i];
    }
    return *result;
}

const pair<Direction, string> * Node::GetNeihbors() const
{
    return _neighbors;
}
