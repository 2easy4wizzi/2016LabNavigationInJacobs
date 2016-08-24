#include "graph.h"
//#include <QDebug>
//#define cout qDebug()<< __LINE__
//#define xxx qDebug()<< __LINE__ ;

map<string, EdgeType> typeMap = { { "NotInitialized", NotInitialized },{ "Regular", Regular },{ "Elevator", Elevator },{ "Stairs", Stairs } };

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
    root = doc.first_node("Graph");
    if (!root) return 0;
    // Iterate over the nodes
    int floor = -1;
    string name = "\0";
    neighborPair neighbors[NUM_OF_NEIGBHORS];
    Direction neighborDir;
    string neighborName = "\0";
    for (xml_node<> * vertex_node = root->first_node("Node"); vertex_node; vertex_node = vertex_node->next_sibling())
    {
            name = vertex_node->first_attribute("Name")->value();
            floor = atoi(vertex_node->first_attribute("Floor")->value());
            // Interate over the nodes neighbors
            int i = 0;
            for (xml_node<> * neighbor_node = vertex_node->first_node("Neighbor"); neighbor_node; neighbor_node = neighbor_node->next_sibling())
            {
                    neighborName = neighbor_node->first_attribute("Name")->value();
                    string tempDir = neighbor_node->first_attribute("Direction")->value();
                    map<basic_string<char>, Direction>::const_iterator it = dirMap.find(tempDir);
                    neighborDir = (*it).second;
                    if (i > 3) { /*throw new exception("i is bigger then 3");*/ }
                    neighbors[i++] =  neighborPair(neighborDir, neighborName);
            }
            Node *node = new Node(name, floor, neighbors);
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
    root = doc.first_node("Graph");
    if (!root) return 0;
    // Iterate over the nodes
    int weight = -1;
    int floor = -1;
    EdgeType type = NotInitialized;
    string nodeName1 = "\0";
    string nodeName2 = "\0";
    for (xml_node<> * vertex_node = root->first_node("Edge"); vertex_node; vertex_node = vertex_node->next_sibling())
    {
            weight = atoi(vertex_node->first_attribute("Weight")->value());
            floor = atoi(vertex_node->first_attribute("Floor")->value());
            string typeStr = vertex_node->first_attribute("Type")->value();
            type = typeMap.find(typeStr)->second;
            nodeName1 = vertex_node->first_attribute("Node1")->value();
            nodeName2 = vertex_node->first_attribute("Node2")->value();
            // find node1 and node2 of the edge
            Node* node1 = NULL;
            Node* node2 = NULL;
            for (Node* node : *_nodes)
            {
                if (node->GetName() == nodeName1)
                {
                    node1 = node;
                }
                else if (node->GetName() == nodeName2)
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

list<pathRoom> Graph::GetShortestpath(Node* start, Node* end)
{
    bool wasChecked[MAXNODES];
    int distanceV[MAXNODES];
    vector< qPair > Adjacency[MAXNODES];
    priority_queue< qPair, vector< qPair >, comp > Queue;
	//if (_nodes == NULL || _edges == NULL) return NULL;
    list<pathRoom>* shortestPath = new list<pathRoom>;
	Node* u = NULL;
	Node* v = NULL;
	Node* startingNode = end;
	Node* endingNode = start;
	int weight = -1;
	int size;
	// initizlized adjacency container
	for (Edge* edge : *_edges)
	{
		u = edge->GetNode1();
		v = edge->GetNode2();
		weight = edge->GetWeight();
		Adjacency[u->GetId()].push_back(qPair(v, weight));
		Adjacency[v->GetId()].push_back(qPair(u, weight));
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
    pathRoom startingRoom = { trace,trace->GetEdgeWeightToPrevious(),trace->GetNeighborDirection(trace->GetPreviosNode()->GetName()), next->GetId() };
    shortestPath->push_back(startingRoom);

    while (next != NULL && next != end)
    {
		if (next->GetPreviosNode() != NULL)
        {
			pathRoom pRoom = { next,
				next->GetEdgeWeightToPrevious(),
                next->GetNeighborDirection(next->GetPreviosNode()->GetName()) ,
                next->GetPreviosNode()->GetId()
                };
            shortestPath->push_back(pRoom);
		}
		next = next->GetPreviosNode();
	}
	//push to the list the ending room
//    pathRoom endingRoom = { end,0,"" };
//    shortestPath->push_back(endingRoom);

    return *shortestPath;
}

list<pathRoom> Graph::GetShrinkendShortestPath()
{
    //if (_shortestPath == NULL)
    list<pathRoom>* shrinkedShortestPath = new list<pathRoom>;
    list<pathRoom>::iterator iter1 = _shortestPath->begin();
    list<pathRoom>::iterator iter2 = _shortestPath->begin();
    ++iter2;
    for (; iter1 != _shortestPath->end() && iter2!= _shortestPath->end(); ++iter1,++iter2)
    {
        bool stop = false;
        bool advance = false;
        string dir = iter1->direction;
        int dis = iter1->distance;
        while ((!stop) && iter2 != _shortestPath->end())
        {
            if (iter1->direction == iter2->direction)
            {
                dis += iter2->distance;
                advance = true;
                ++iter2;
            }
            else
            {
                stop = true;
            }
        }
        pathRoom pRoom = { iter1->room,dis,dir };
        shrinkedShortestPath->push_back(pRoom);
        if (advance) { iter1 = iter2; }

    }
    iter1 = _shortestPath->end();
    --iter1;
    iter2 = shrinkedShortestPath->end();
    --iter2;
    if (iter2->room != iter1->room)
    {
        pathRoom endPathRoom = { iter1->room,0,"" };
        shrinkedShortestPath->push_back(endPathRoom);
    }
    return *shrinkedShortestPath;

}
