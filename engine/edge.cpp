#include "edge.h"


Edge::Edge(double weight, Node* node1, Node* node2, EdgeType edgeType )
{
    _weight = weight;
    _edgeType = edgeType;
    _node1 = node1;
    _node2 = node2;
}

Edge::~Edge()
{
    delete(_node1);
    delete(_node2);
}

double Edge::GetWeight() const
{
    return _weight;
}

EdgeType Edge::GetEdgeType() const
{
    return _edgeType;
}

Node* Edge::GetNode1() const
{
    return _node1;
}

Node* Edge::GetNode2() const
{
    return _node2;
}

void Edge::SetVideoInfo(int index, videoInfo vidInfo)
{
    if(index == 1){
        vidInfo1 = vidInfo;
    }
    else{//index == 2
        vidInfo2 = vidInfo;
    }
}

videoInfo Edge::GetVideoInfo(int index)
{
    if(index == 1){
        return vidInfo1;
    }
    //else index == 2
    return vidInfo2;

}


