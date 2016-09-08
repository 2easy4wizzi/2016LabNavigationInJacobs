#ifndef NODE_H
#define NODE_H
#include "includes.h"


#define neighborPair pair< Direction,int >
#define NUM_OF_NEIGBHORS 4
#define NUMBER_OF_CLASSES 5

enum Direction {North, East, south, West, Invalid};
const map<string, Direction> dirMap = { { "North", North },{ "East", East },{ "south", south },{ "West", West } };
const map< Direction, string> dirRevMap = { { North,"North"  },{ East,"East"  },{ south, "south"  },{ West,"West"  } };


class Node
{
private:
    string _name;
	string _number; // used for numbering the rooms, uniqued 
    int _id; // unique field
    int _floor;
    double _edgeWeightToPrevious; // used for back tracking the weights on the shortest path
	int _videoStartIndex; // used for indexing the relative timing showing the node on the video - in miliseconds
	int _videoEndIndex;
    Node* _previosNode; // used for backtracking the nodes on the shortest path
    neighborPair _neighbors[NUM_OF_NEIGBHORS]; // contains the neighbors at 4 directions
    int _classes[NUMBER_OF_CLASSES];
    int _howManyClassesFound;
    string _videoPath;

public:
    Node(string name, string number, int floor, neighborPair (&neighbors)[NUM_OF_NEIGBHORS], int (&classes)[NUMBER_OF_CLASSES],int howManyClassesFound, int videoStartIndex, int videoEndIndex, string videoPath);
    //Node(Node* other);
    ~Node();

    string GetName() const;
    void SetName(string const *name);
    int GetId() const;
    int GetNodeFloor() const;
    void SetNodeFloor(int floorNum);
    double GetEdgeWeightToPrevious() const;
    void SetEdgeWeightToPrevious(double eWeight);
    Node* GetPreviosNode() const;
    void SetPreviosNode(Node* previos) ;
    const basic_string<char> GetNeighborDirection(int neighborId);
    pair<Direction, int> *GetNeihbors() ;
    int * GetClasses();
	int GetVideoStartIndex() const;
	int GetVideoEndIndex() const;
    string GetNumber() const;
    string ToString() const;
    string ClassesToString() const;
    double _roomPathDistance;
    string _roomPathDirection;
    int _roomPathNextRoomInPathId;
    string GetVideoPath() const;
    int howManyClassesFound() const;
};

#endif // NODE_H
