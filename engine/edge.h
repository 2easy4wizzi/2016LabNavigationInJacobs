#ifndef EDGE_H
#define EDGE_H

#include "includes.h"
#include "node.h"

enum EdgeType { NotInitialized ,Regular,Elevator,Stairs };
//const string EdgeType_name[] = { "NotInitialized", "Regular", "Elevator" ,"Stairs" };

class Edge
{

private:
    double _weight;
    int _floor;
    EdgeType _edgeType;
    Node* _node1;
    Node* _node2;
public:
    Edge(double weight, int floor, Node* node1, Node* node2, EdgeType edgeType = NotInitialized);
    ~Edge();

    double GetWeight() const;
    void SetWeight(double eWeight);

    int GetEdgeFloor() const;
    void SetEdgeFloor(int floorNum);

    EdgeType GetEdgeType() const;
    void SetEdgeType(EdgeType eType);

    Node* GetNode1() const;
    void SetNode1(Node* node1);

    Node* GetNode2() const;
    void SetNode2(Node* node2);
};

#endif // EDGE_H
