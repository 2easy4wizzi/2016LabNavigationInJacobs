#include "graph.h"
#include <QDebug>
#define cout qDebug()<< __LINE__
#define xxx qDebug()<< __LINE__ ;
#define DEBUGCPP 1

const char* fieldGraph= "Graph";
const char* fieldEdge= "Edge";
const char* fieldWeight= "Weight";
const char* fieldFloor= "Floor";
const char* fieldType= "Type";
const char* fieldNode1= "Node1";
const char* fieldNode2= "Node2";
const char* fieldNode= "Node";
const char* fieldName= "Name";
const char* fieldNumber= "Number";
const char* fieldNotInitialized= "NotInitialized";
const char* fieldRegular= "Regular";
const char* fieldElevator= "Elevator";
const char* fieldStairs= "Stairs";
const char* fieldVideoStartIndex= "VideoStartIndex";
const char* fieldVideoEndIndex= "VideoEndIndex";
const char* fieldNeighbor= "Neighbor";
const char* fieldDirection= "Direction";
const char* fieldId= "Id";
const char* fieldVideoPath= "videoPath";
const char* fieldClass0= "Class0";
const char* fieldClass1= "Class1";
const char* fieldClass2= "Class2";
const char* fieldClass3= "Class3";
const char* fieldClass4= "Class4";



map<string, EdgeType> typeMap = { { fieldNotInitialized, NotInitialized },{ fieldRegular, Regular },{ fieldElevator, Elevator },{ fieldStairs, Stairs } };

using namespace rapidxml;

Graph::Graph(string xmlPathNodes, string xmlPathEdges, bool& succesReadingXmls)
{
    succesReadingXmls = ParseXmlNodes(xmlPathNodes);
    succesReadingXmls &= ParseXmlEdges(xmlPathEdges);//#mark 
}
Graph::~Graph()
{
    delete(_nodes);
    delete(_edges);
}

bool Graph::ParseXmlNodes(string xmlPathNodes)
{
    _nodes = new list<Node*>();
    xml_document<> doc;
    xml_node<> * root;
    // Read the xml file into a vector
    ifstream xmlFile(xmlPathNodes);
    vector<char> buffer((istreambuf_iterator<char>(xmlFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc
    doc.parse<0>(&buffer[0]);
    // Find our root
    root = doc.first_node(fieldGraph);
    if (!root) return 0;
    // Iterate over the nodes
    int floor = -1;
	int videoStartIndex = -1;
	int videoEndIndex = -1;
    string name = "\0";
	string number = "\0";
    string videoPath = "\0";
    neighborPair neighbors[NUM_OF_NEIGBHORS];
    int classes[NUMBER_OF_NEIGBHORS];
    Direction neighborDir;
    int neighborId = 0;
    for (xml_node<> * vertex_node = root->first_node(fieldNode); vertex_node; vertex_node = vertex_node->next_sibling())
    {
            name = vertex_node->first_attribute(fieldName)->value();
            number = vertex_node->first_attribute(fieldNumber)->value();
            floor = atoi(vertex_node->first_attribute(fieldFloor)->value());
            videoStartIndex = atoi(vertex_node->first_attribute(fieldVideoStartIndex)->value());
            videoEndIndex = atoi(vertex_node->first_attribute(fieldVideoEndIndex)->value());
            videoPath = vertex_node->first_attribute(fieldVideoPath)->value();
            classes[0] = atoi(vertex_node->first_attribute(fieldClass0)->value());
            classes[1] = atoi(vertex_node->first_attribute(fieldClass1)->value());
            classes[2] = atoi(vertex_node->first_attribute(fieldClass2)->value());
            classes[3] = atoi(vertex_node->first_attribute(fieldClass3)->value());
            classes[4] = atoi(vertex_node->first_attribute(fieldClass4)->value());
			// Interate over the nodes neighbors
            int i = 0;
            for (xml_node<> * neighbor_node = vertex_node->first_node(fieldNeighbor); neighbor_node; neighbor_node = neighbor_node->next_sibling())
            {
                    neighborId = atoi(neighbor_node->first_attribute(fieldId)->value());
                    string tempDir = neighbor_node->first_attribute(fieldDirection)->value();
                    map<basic_string<char>, Direction>::const_iterator it = dirMap.find(tempDir);
                    neighborDir = (*it).second;
                    if (i > 3) { /*throw new exception("i is bigger then 3");*/ }
                    neighbors[i++] =  neighborPair(neighborDir, neighborId);
            }
            Node *node = new Node(name,number, floor, neighbors,classes, videoStartIndex, videoEndIndex, videoPath);
            _nodes->push_back(node);
    }
    return 1;
}

bool Graph::ParseXmlEdges(string xmlPathEdges)
{
    _edges = new list<Edge*>();
    xml_document<> doc;
    xml_node<> * root;
    // Read the xml file into a vector
    ifstream xmlFile(xmlPathEdges);
    vector<char> buffer((istreambuf_iterator<char>(xmlFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc
    doc.parse<0>(&buffer[0]);
    // Find our root
    root = doc.first_node(fieldGraph);
    if (!root) return 0;
    // Iterate over the nodes
    double weight = -1;
    int floor = -1;
    EdgeType type = NotInitialized;
    int nodeId1 = 0;
    int nodeId2 = 0;
    for (xml_node<> * vertex_node = root->first_node(fieldEdge); vertex_node; vertex_node = vertex_node->next_sibling())
    {
            weight = atof(vertex_node->first_attribute(fieldWeight)->value());
            floor = atoi(vertex_node->first_attribute(fieldFloor)->value());
            string typeStr = vertex_node->first_attribute(fieldType)->value();
            type = typeMap.find(typeStr)->second;
            nodeId1 = atoi(vertex_node->first_attribute(fieldNode1)->value());
            nodeId2 = atoi(vertex_node->first_attribute(fieldNode2)->value());
            // find node1 and node2 of the edge
            Node* node1 = NULL;
            Node* node2 = NULL;
            for (Node* node : *_nodes)
            {
                if (node->GetId() == nodeId1)
                {
                    node1 = node;
                }
                else if (node->GetId() == nodeId2)
                {
                    node2 = node;
                }
            }
            // if node1 or node2 null throw exception
            Edge *edge = new Edge(weight,floor,node1,node2,type);
            _edges->push_back(edge);
    }
    return 1;
}

list<Node*> Graph::GetGrapghNodes() const
{
    return *_nodes;
}

list<Edge*> Graph::GetGrapghEdges() const
{
    return *_edges;
}

list<Node *> Graph::GetShortestpath(Node* start, Node* end, EdgeType pref)
{
    int prefAddon = 0;
    bool wasChecked[MAXNODES];
    int distanceV[MAXNODES];
    vector< qPair > Adjacency[MAXNODES];
    priority_queue< qPair, vector< qPair >, comp > Queue;
	//if (_nodes == NULL || _edges == NULL) return NULL;
    list<Node*>* shortestNodesPath = new list<Node*>;
	Node* u = NULL;
	Node* v = NULL;
	Node* startingNode = end;
	Node* endingNode = start;
    double weight = -1;
    int size;
	// initizlized adjacency container
	for (Edge* edge : *_edges)
    {
		u = edge->GetNode1();
		v = edge->GetNode2();
		weight = edge->GetWeight();
        EdgeType edgeType = edge->GetEdgeType();
        if (edgeType != EdgeType::Regular){
            if(pref == EdgeType::Stairs && edgeType == EdgeType::Elevator ){
                prefAddon = INF;
            }
            else if(pref == EdgeType::Elevator && edgeType == EdgeType::Stairs ){
                prefAddon = INF;
            }
        }
        Adjacency[u->GetId()].push_back(qPair(v, weight + prefAddon));
        Adjacency[v->GetId()].push_back(qPair(u, weight + prefAddon));
        prefAddon = 0;
    }
	// initialize distance vector
    for (unsigned int i = 1; i <= _nodes->size(); i++) {
        distanceV[i] = INFVALUE;
        wasChecked[i]=false;
    }
	distanceV[startingNode->GetId()] = 0;
	Queue.push(qPair(startingNode, 0));
	// dijkstra
	while (!Queue.empty()) {
		u = Queue.top().first;
		Queue.pop();
		if (u == endingNode)
		{
			break;
		}
		if (wasChecked[u->GetId()]) continue;
		size = Adjacency[u->GetId()].size();
		for (int i = 0; i<size; i++) {
			v = Adjacency[u->GetId()][i].first;
			weight = Adjacency[u->GetId()][i].second;
            if (!wasChecked[v->GetId()] && distanceV[u->GetId()] + weight < distanceV[v->GetId()]) {
				distanceV[v->GetId()] = distanceV[u->GetId()] + weight;
				v->SetPreviosNode(u);
				v->SetEdgeWeightToPrevious(weight);				
				Queue.push(qPair(v, distanceV[v->GetId()]));
			}
		}
		wasChecked[u->GetId()] = 1; // done with u
	}
    Node* trace = start;
	// push to list the starting room
    Node *next = trace->GetPreviosNode();
    trace->_roomPathDistance = trace->GetEdgeWeightToPrevious();
    trace->_roomPathDirection = trace->GetNeighborDirection(trace->GetPreviosNode()->GetId());
    trace->_roomPathNextRoomInPathId = next->GetId();
    shortestNodesPath->push_back(trace);
    if(DEBUGCPP)
    {
        cout << "###\nstart of full path";
        cout << trace->GetId() << trace->GetName().c_str() << trace->_roomPathNextRoomInPathId << trace->_roomPathDirection.c_str();
    }
    while (next != NULL && next != end)
    {
		if (next->GetPreviosNode() != NULL)
        {
            next->_roomPathDistance = next->GetEdgeWeightToPrevious();
            next->_roomPathDirection = next->GetNeighborDirection(next->GetPreviosNode()->GetId());
            next->_roomPathNextRoomInPathId = next->GetPreviosNode()->GetId();
            shortestNodesPath->push_back(next);
            if(DEBUGCPP)
            {
                cout << next->GetId() << next->GetName().c_str() << next->_roomPathNextRoomInPathId << next->_roomPathDirection.c_str();
            }
		}
		next = next->GetPreviosNode();
	}
	//push to the list the ending room
    end->_roomPathDistance  = 0;
    end->_roomPathDirection= "";
    end->_roomPathNextRoomInPathId = -1;
    shortestNodesPath->push_back(end);
    if(DEBUGCPP)
    {
        cout << "end of full path\n###";
        cout << trace->GetId() << trace->GetName().c_str() << trace->_roomPathNextRoomInPathId;
    }
    cout << end->GetName().c_str();
    // nothing to shrink if there is only one destination on the path
    return (shortestNodesPath->size() < 2) ? (*shortestNodesPath) :GetShrinkendShortestPath(*shortestNodesPath);
}

list<Node *> Graph::GetShrinkendShortestPath(list<Node *> shortestPath)
{
    //if (shortestPath == NULL)
    list<Node*>* shrinkedShortestPath = new list<Node*>;
    list<Node*>::iterator iter1 = shortestPath.begin();
    list<Node*>::iterator iter2 = shortestPath.begin();
    list<Node*>::iterator iterEnd = shortestPath.end();
	++iter2;	
    double distance = 0;

	while (iter2!= iterEnd )
	{
		bool advanced = false;
        Node* tmp1 = *iter1;
        Node* tmp2 = *iter2;
        distance = tmp1->_roomPathDistance; //distance to the next room on shortest path
        while(iter2!=iterEnd && tmp1->_roomPathDirection== tmp2->_roomPathDirection && tmp1->_roomPathDirection != "")
		{
            distance += tmp2->_roomPathDistance;
			++iter2;
			advanced = true;
            tmp2 = *iter2;
		}

        tmp1->_roomPathDistance = distance;
        tmp1->_roomPathDirection = tmp1->_roomPathDirection;//#mark
        tmp1->_roomPathNextRoomInPathId = tmp2->GetId();
        shrinkedShortestPath->push_back(tmp1);
		if (advanced)
		{
			iter1 = iter2;
		} 
		else
		{
			++iter1;
		}		
		++iter2;
	}

    return *shrinkedShortestPath;
}
