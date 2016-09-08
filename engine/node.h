#ifndef NODE_H
#define NODE_H
#include "includes.h"


#define NUMBER_OF_CLASSES 5

class Node
{
public:
    Node(int id, string name, string number, int floor, int (&classes)[NUMBER_OF_CLASSES],int howManyClassesFound, int videoStartIndex, int videoEndIndex, string videoPath);
    ~Node();
    string GetName() const;
    int GetId() const;
    int GetNodeFloor() const;
    double GetEdgeWeightToPrevious() const;
    void SetEdgeWeightToPrevious(double eWeight);
    Node* GetPreviosNode() const;
    void SetPreviosNode(Node* previos) ;
    int * GetClasses();
	int GetVideoStartIndex() const;
	int GetVideoEndIndex() const;
    string GetNumber() const;
    string ToString() const;
    string ClassesToString() const;
    string GetVideoPath() const;
    int howManyClassesFound() const;
    double distanceToNextNodeInPath() const;
    void setdistanceToNextNodeInPath(double distanceToNextNodeInPath);
    void addTodistanceToNextNodeInPath(double add);
    int nextRoomInPathId() const;
    void setnextRoomInPathId(int nextRoomInPathId);


private:
    string _name;
    string _number; // used for numbering the rooms, uniqued
    int _id; // unique field
    int _floor;
    double _edgeWeightToPrevious; // used for back tracking the weights on the shortest path
    int _videoStartIndex; // used for indexing the relative timing showing the node on the video - in miliseconds
    int _videoEndIndex;
    Node* _previosNode; // used for backtracking the nodes on the shortest path
    int _classes[NUMBER_OF_CLASSES];
    int _howManyClassesFound;
    string _videoPath;
    int _nextRoomInPathId;
    double _distanceToNextNodeInPath;
};

#endif // NODE_H
