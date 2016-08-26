#ifndef NODE_H
#define NODE_H
#include "includes.h"


#define neighborPair pair< Direction,string >
#define NUM_OF_NEIGBHORS 4

enum Direction {North, East, South, West, Invalid};
const map<string, Direction> dirMap = { { "North", North },{ "East", East },{ "South", South },{ "West", West } };
const map< Direction, string> dirRevMap = { { North,"North"  },{ East,"East"  },{ South, "South"  },{ West,"West"  } };


class Node
{
private:
    string _name;
	string _number; // used for numbering the rooms, uniqued 
    int _id; // unique field
    int _floor;
    int _edgeWeightToPrevious; // used for back tracking the weights on the shortest path
    Node* _previosNode; // used for backtracking the nodes on the shortest path
    neighborPair _neighbors[NUM_OF_NEIGBHORS]; // contains the neighbors at 4 directions
public:
    Node(string name,string number,int floor, neighborPair (&neighbors)[NUM_OF_NEIGBHORS]);
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
	const basic_string<char>GetNeighborDirection(string neighborName);
     pair<Direction, string> *GetNeihbors() ;
};

#endif // NODE_H
