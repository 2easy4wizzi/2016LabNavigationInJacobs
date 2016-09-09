#include "node.h"

//string Node::GetVideoPath() const
//{
//    return _videoPath;
//}

int Node::howManyClassesFound() const
{
    return _howManyClassesFound;
}

double Node::distanceToNextNodeInPath() const
{
    return _distanceToNextNodeInPath;
}

void Node::setdistanceToNextNodeInPath(double distanceToNextNodeInPath)
{
    _distanceToNextNodeInPath = distanceToNextNodeInPath;
}

void Node::addTodistanceToNextNodeInPath(double add)
{
    _distanceToNextNodeInPath += add;
}

int Node::nextRoomInPathId() const
{
    return _nextRoomInPathId;
}

void Node::setnextRoomInPathId(int nextRoomInPathId)
{
    _nextRoomInPathId = nextRoomInPathId;
}

Node::Node(int id, string name,  string number, int floor, int (&classes)[NUMBER_OF_CLASSES], int howManyClassesFound)
{
    _id = id;
//	_videoStartIndex = videoStartIndex;
//	_videoEndIndex = videoEndIndex;
    _floor = floor;
    _name = name;
	_number = number;
    _previosNode = NULL;
    for (int i = 0; i < NUMBER_OF_CLASSES; i++)
    {
        _classes[i] = classes[i];
    }
    _howManyClassesFound = howManyClassesFound;
    _edgeWeightToPrevious = 0;
//    _videoPath = videoPath;
}


Node::~Node()
{
}

string Node::GetName() const
{
    return _name;
}

int Node::GetId() const
{
    return _id;
}

int Node::GetNodeFloor() const
{
    return _floor;
}

double Node::GetEdgeWeightToPrevious() const
{
    return _edgeWeightToPrevious;
}

void Node::SetEdgeWeightToPrevious(double eWeight)
{
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

int * Node::GetClasses()
{
    return _classes;
}

//int Node::GetVideoStartIndex() const
//{
//	return _videoStartIndex;
//}

//int Node::GetVideoEndIndex() const
//{
//    return _videoEndIndex;
//}

string Node::GetNumber() const
{
    return _number;
}

string Node::ToString() const
{
    string toString =
            "name:" + _name + "; " +
            "number:" + _number + "; " +
            "id:" + std::to_string(_id) + "; " +
            "floor:" + std::to_string(_floor) + "; " +
            "edgeWPrev:" + std::to_string(_edgeWeightToPrevious) + "; " +
//            "videoSI:" + std::to_string(_videoStartIndex) + "; " +
//            "videoEI:" + std::to_string(_videoEndIndex) + " "; +
//            "videoPath:" + _videoPath + "; " +
            "distanceToNext:" + std::to_string(_distanceToNextNodeInPath) + "; " +
            "nextRoomId:" + std::to_string(_nextRoomInPathId) + "; " +
            "classes:" + ClassesToString()
            ;
    return toString;
}

string Node::ClassesToString() const
{
    int i = 0;
    string classes = "";
    while(_classes[i] != 0 && i < NUMBER_OF_CLASSES)
    {
        classes.append(std::to_string(_classes[i]));
        classes.append(", ");
        i++;
    }
    return (i!=0) ? classes : ""; //if i==0, then classes are empty
}
