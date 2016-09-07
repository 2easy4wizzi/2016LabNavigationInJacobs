#include "node.h"


string Node::GetVideoPath() const
{
    return _videoPath;
}

Node::Node(string name,  string number, int floor, neighborPair (&neighbors)[NUM_OF_NEIGBHORS], int (&classes)[NUMBER_OF_NEIGBHORS], int videoStartIndex, int videoEndIndex, string videoPath)
{
        static int NodesId = 1; //#mark cahnge to attribute
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
    for (int i = 0; i < NUMBER_OF_NEIGBHORS; i++)
    {
        _classes[i] = classes[i];
    }
    _edgeWeightToPrevious = 0;
    _videoPath = videoPath;
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

double Node::GetEdgeWeightToPrevious() const
{
    return _edgeWeightToPrevious;
}

void Node::SetEdgeWeightToPrevious(double eWeight)
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

const basic_string<char> Node::GetNeighborDirection(int neighborId)
{
    for (size_t i = 0; i < NUM_OF_NEIGBHORS; i++)
	{
        if (_neighbors[i].second == neighborId)
        {
            map<Direction,basic_string<char> >::const_iterator it = dirRevMap.find(_neighbors[i].first);
            return (*it).second;
        }
	}
    return "";
}

 pair<Direction, int> * Node::GetNeihbors()
{
    return _neighbors;
}

int * Node::GetClasses()
{
    return _classes;
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

string Node::ToString() const
{
    string toString =
            "_name:" + _name + " " +
            "_number:" + _number + " " +
            "_id:" + std::to_string(_id) + " " +
            "_floor:" + std::to_string(_floor) + " " +
            "_edgeWPrev:" + std::to_string(_edgeWeightToPrevious) + " " +
            "_videoSI:" + std::to_string(_videoStartIndex) + " " +
            "_videoEI:" + std::to_string(_videoEndIndex) + " " +
            "_videoPath:" + _videoPath + " " +
            "roomPathDistance:" + std::to_string(_roomPathDistance) + " " +
            "roomPathDirection:" + _roomPathDirection + " " +
            "roomPathNextRoomInPathId:" + std::to_string(_roomPathNextRoomInPathId) + " "
            ;
    return toString;
}
