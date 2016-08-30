#include "node.h"


Node::Node(string name,  string number, int floor, neighborPair (&neighbors)[NUM_OF_NEIGBHORS], int videoStartIndex, int videoEndIndex)
{
        static int NodesId = 1;
//    if (floor < 0) /*throw new exception("Error! floor input is out of range")*/;
//    if (name.empty()) /*throw new exception("Error! name input is empty")*/;
    _id = NodesId++;
	_videoStartIndex = videoStartIndex;
	_videoEndIndex = videoEndIndex;
    _floor = floor;
    _name = name;
	_number = number;
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

const basic_string<char>Node::GetNeighborDirection(string neighborName)
{
	string result = "";
	if ((neighborName).empty()) return NULL;
    for (size_t i = 0; i < NUM_OF_NEIGBHORS; i++)
	{
		if (_neighbors[i].second == neighborName)
        {
            map<Direction,basic_string<char> >::const_iterator it = dirRevMap.find(_neighbors[i].first);
            return (*it).second;
        }
	}
	return result;
}

 pair<Direction, string> * Node::GetNeihbors()
{
    return _neighbors;
}

int Node::GetVideoStartIndex() const
{
	return _videoStartIndex;
}

int Node::GetVideoEndIndex() const
{
    return _videoEndIndex;
}

string Node::GetNumber() const
{
    return _number;
}
