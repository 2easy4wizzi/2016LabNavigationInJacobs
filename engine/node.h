#ifndef NODE_H
#define NODE_H
#include "includes.h"
#include "videoinfo.h"

#define ASSUMING_NO_MORE_THAN_NODES_IN_PATH 20
#define NUMBER_OF_CLASSES 5

class Node
{
public:
    Node(int id, string name, string number, int floor, int (&classes)[NUMBER_OF_CLASSES], int howManyClassesFound);
    ~Node();
    string GetName() const;
    int GetId() const;
    int GetNodeFloor() const;
    double GetEdgeWeightToPrevious() const;
    void SetEdgeWeightToPrevious(double eWeight);
    Node* GetPreviosNode() const;
    void SetPreviosNode(Node* previos) ;
    int * GetClasses();
    string GetNumber() const;
    string ToString() const;
    string ClassesToString() const;
    int howManyClassesFound() const;
    double distanceToNextNodeInPath() const;
    void setdistanceToNextNodeInPath(double distanceToNextNodeInPath);
    void addTodistanceToNextNodeInPath(double add);
    int nextRoomInPathId() const;
    void setnextRoomInPathId(int nextRoomInPathId);
    void saveVideoInfoOfNodesInPath(videoInfo vidInfo);
    videoInfo getMyVideoInfo();
    int videoInfoOfNodesInPathConter() const;
    videoInfo* GetAllVideoInfos();

private:
    string _name;
    string _number; // used for numbering the rooms, uniqued
    int _id; // unique field
    int _floor;
    double _edgeWeightToPrevious; // used for back tracking the weights on the shortest path
    Node* _previosNode; // used for backtracking the nodes on the shortest path
    int _classes[NUMBER_OF_CLASSES];
    int _howManyClassesFound;
    int _nextRoomInPathId;
    double _distanceToNextNodeInPath;
    videoInfo _videoInfoOfNodesInPath[ASSUMING_NO_MORE_THAN_NODES_IN_PATH];
    int _videoInfoOfNodesInPathConter;
};

#endif // NODE_H
