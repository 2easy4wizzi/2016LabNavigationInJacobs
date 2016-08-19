#include "graph.h"
#include <QDebug>
using namespace rapidxml;
#define cout qDebug()<< __LINE__
Graph::Graph(string xmlPath)
{
    cout <<xmlPath.c_str();
//    char* path;
//    _get_pgmptr(&path);
//    cout << path;
    _nodes = new list<Node>();
    _edges = new list<Edge>();
    xml_document<> doc;
    xml_node<> * root_node;
    // Read the xml file into a vector

    ifstream theFile("C:/workspace/Nav/Nav/dependencies/rooms.xml");
//    ifstream theFile0("../../dependencies/rooms.xml");
//    ifstream theFile1("../../../dependencies/rooms.xml");
//    ifstream theFile2("../dependencies/rooms.xml");
//    ifstream theFile3("dependencies/rooms.xml");






    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc
    doc.parse<0>(&buffer[0]);
    // Find our root node
    root_node = doc.first_node("Graph");
//    if (!root_node)
//    {
//        cout << "root is null";
//        exit;
//    }
    // Iterate over the nodes
    int nodeFloor = -1;
    string nodeName = "\0";
    neighborPair neighbors[NUMOFNEIGBHORS];
    Direction neighborDir;
    string neighborName = "\0";
    for (xml_node<> * vertex_node = root_node->first_node("Node"); vertex_node; vertex_node = vertex_node->next_sibling())
    {
            nodeName = vertex_node->first_attribute("name")->value();
            nodeFloor = atoi(vertex_node->first_attribute("floor")->value());
            // Interate over the nodes neighbors
            int i = 0;
            for (xml_node<> * neighbor_node = vertex_node->first_node("Neighbor"); neighbor_node; neighbor_node = neighbor_node->next_sibling())
            {
                    neighborName = neighbor_node->first_attribute("name")->value();
                    string tempDir = neighbor_node->first_attribute("direction")->value();
                    map<basic_string<char>, Direction>::const_iterator it = dirMap.find(tempDir);
                    neighborDir = (*it).second;
                    if (i > 3) { /*throw new exception("i is bigger then 3");*/ }
                    neighbors[i++] =  neighborPair(neighborDir, neighborName);
            }
            Node *node = new Node(nodeName, nodeFloor, neighbors);
            _nodes->push_back(*node);
    }
    cout << "finished"<<endl;
}



Graph::~Graph()
{
    delete(_nodes);
    delete(_edges);
}

list<Node> Graph::GetGrapghNodes() const
{
    return *_nodes;
}

list<Edge> Graph::GetGrapghEdges() const
{
    return *_edges;
}

void Graph::AddToNodesList(Node* node)
{
    _nodes->push_back(*node);
}

void Graph::AddToEdgesList(Edge* edge)
{
    _edges->push_back(*edge);
}
