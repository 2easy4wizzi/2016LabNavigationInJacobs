#ifndef NODE_H
#define NODE_H


#define neighborPair pair< Direction,string >
#define NUMOFNEIGBHORS 4 // update to 8 ?

#include "includes.h"
enum Direction {North, East, South, West, Invalid};
const string direction_names[] = { "North", "East", "South" ,"West" };
const map<string, Direction> dirMap = { { "North", North },{ "East", East },{ "South", South },{ "West", West } };

class Node
{
private:
    string _name;
    int _id;
    int _floor;
    int _edgeWeightToPrevious; // usedfor back tracking the weights on the shortest path
    Node* _previosNode; // used for backtracking the nodes on the shortest path
    neighborPair _neighbors[NUMOFNEIGBHORS]; // contains the neighbors at 4 directions
public:
    Node();
    Node(string name,int floor, neighborPair (&neighbors)[NUMOFNEIGBHORS]);
    ~Node();

    string GetName() const;
    void SetName(string const *name);
    int GetId() const;
    int GetNodeFloor() const;
    void SetNodeFloor(int floorNum);
    int GetEdgeWeightToPrevious() const;
    void SetEdgeWeightToPrevious(int eWeight);
    Node* GetPreviosNode() const;
    void SetPreviosNode(Node* previos) ;
    const basic_string<char>GetNeighborDirection(string const *neighborName);
    const pair<Direction, string> * GetNeihbors() const;
    // add set for _neighbors ?
};

#endif // NODE_H
