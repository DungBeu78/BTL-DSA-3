#include "KnowledgeGraph.h"

// =============================================================================
// Class Edge Implementation
// =============================================================================

template <class T>
Edge<T>::Edge()
{
    this->from = nullptr;
    this->to = nullptr;
    this->weight = 0.0f;
}

template <class T>
Edge<T>::Edge(VertexNode<T> *from, VertexNode<T> *to, float weight)
{
    this->from = from;
    this->to = to;
    this->weight = weight;
}

template <class T>
bool Edge<T>::equals(Edge<T> *edge)
{
    return (this->from == edge->from && this->to == edge->to);
}

// Not implement
template <class T>
string Edge<T>::toString()
{
    // TODO: Return the string representation of the edge
    return "";
}

// TODO: Implement other methods of Edge:

// =============================================================================
// Class VertexNode Implementation
// =============================================================================

template <class T>
VertexNode<T>::VertexNode(T vertex, bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &))
{
    this->vertex = vertex;
    this->vertexEQ = vertexEQ;
    this->vertex2str = vertex2str;
    this->inDegree_ = 0;
    this->outDegree_ = 0;
}

template <class T>
T &VertexNode<T>::getVertex()
{
    return *this->vertex;
}

template <class T>
void VertexNode<T>::connect(VertexNode<T> *to, float weight)
{
    // TODO: Connect this vertex to the 'to' vertex
    Edge<T> *newEdge = new Edge<T>(this, to, weight);

    this->adList.push_back(newEdge);

    // Update data
    to->inDegree_++;
    this->outDegree_++;
}

template <class T>
Edge<T> *VertexNode<T>::getEdge(VertexNode<T> *to)
{
    for (Edge<T> *edge : this->adList)
        if (edge->to == to)
            return edge;
    return nullptr;
}

template <class T>
bool VertexNode<T>::equals(VertexNode<T> *node)
{
    return this->vertexEQ(this->vertex, node->vertex);
}

template <class T>
void VertexNode<T>::removeTo(VertexNode<T> *to)
{
    Edge<T> *toRemove = this->getEdge(to);

    if (toRemove == nullptr)
        return;

    // Update data
    this->outDegree_--;
    to->inDegree_--;

    for (auto it = this->adList.begin(); it != this->adList.end(); it++)
    {
        if (*it == toRemove)
        {
            this->adList.erase(it);
            break;
        }
    }

    // Remove
    delete toRemove;
}

template <class T>
int VertexNode<T>::inDegree()
{
    return this->inDegree_;
}

template <class T>
int VertexNode<T>::outDegree()
{
    return this->outDegree_;
}

// Not implement
template <class T>
std::string VertexNode<T>::toString()
{
    return "";
}

// Not implement
template <class T>
std::vector<T> VertexNode<T>::getOutwardEdges()
{
    // vector<T> result;

    // for (Edge<T>* edge : this->adList)
    //     result.push_back(edge->to->vertex);

    // return result;

    return this->adList;

    // Should return in vector<Edge<T> *> ???
}

// =============================================================================
// Class DGraphModel Implementation
// =============================================================================

template <class T>
DGraphModel<T>::DGraphModel(bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &))
{
    this->vertexEQ = vertexEQ;
    this->vertex2str = vertex2str;
}

template <class T>
DGraphModel<T>::~DGraphModel()
{
    // TODO: Clear all vertices and edges to avoid memory leaks
    for (VertexNode<T> *vertex : this->nodeList)
    {
        for (Edge<T> *edge : vertex->adList)
            delete edge;
        delete vertex;
    }

    this->nodeList.clear();
}

template <class T>
VertexNode<T> *DGraphModel<T>::getVertexNode(T &vertex)
{
    for (VertexNode<T> *current : this->nodeList)
    {
        if (this->vertexEQ(current->vertex, vertex))
            return current;
    }

    return nullptr;
}

// Not implement
template <class T>
std::string DGraphModel<T>::vertex2Str(VertexNode<T> &node)
{
    return "";
}

// Not implement
template <class T>
std::string DGraphModel<T>::edge2Str(Edge<T> &edge)
{
    return "";
}

template <class T>
void DGraphModel<T>::add(T vertex)
{
    // TODO: Add a new vertex to the graph
    if (this->contains(vertex))
        return;

    VertexNode<T> *newNode = new VertexNode<T>(vertex, this->vertexEQ, this->vertex2str);

    // Add
    this->nodeList.push_back(newNode);
}

template <class T>
bool DGraphModel<T>::contains(T vertex)
{
    for (VertexNode<T> *current : this->nodeList)
    {
        if (this->vertexEQ(current->vertex, vertex))
            return true;
    }
    return false;
}

template <class T>
float DGraphModel<T>::weight(T from, T to)
{
    // Find vertex from and to
    VertexNode<T> *fromNode = this->getVertexNode(from);
    VertexNode<T> *toNode = this->getVertexNode(to);

    // Throw exception
    if (fromNode == nullptr || toNode == nullptr)
        throw VertexNotFoundException();
    
    // Find edge
    Edge<T> *edge = fromNode->getEdge(toNode);

    // Throw exception
    if (edge == nullptr)
        throw EdgeNotFoundException();

    return edge->weight;
}

// Not implement
template<class T>
std::vector<T> DGraphModel<T>::getOutwardEdges(T from)
{
    // Find vertex from
    VertexNode<T> *fromNode = this->getVertexNode(from);

    // Throw exception
    if (fromNode == nullptr)
        throw VertexNotFoundException();
    
    return fromNode->getOutwardEdges();

    // Should return in vector<Edge<T> *> ???
}

// Not implement
template <class T>
void DGraphModel<T>::connect(T from, T to, float weight)
{
    // TODO: Connect two vertices 'from' and 'to'

    // Find vertex from
    VertexNode<T> *fromNode = this->getVertexNode(from);
    VertexNode<T> *toNode = this->getVertexNode(to);

    // Throw exception
    if (fromNode == nullptr || toNode == nullptr)
        throw VertexNotFoundException();


}

// TODO: Implement other methods of DGraphModel:

// =============================================================================
// Class KnowledgeGraph Implementation
// =============================================================================

KnowledgeGraph::KnowledgeGraph()
{
    // TODO: Initialize the KnowledgeGraph
}

void KnowledgeGraph::addEntity(string entity)
{
    // TODO: Add a new entity to the Knowledge Graph
}

void KnowledgeGraph::addRelation(string from, string to, float weight)
{
    // TODO: Add a directed relation
}

// TODO: Implement other methods of KnowledgeGraph:

// =============================================================================
// Explicit Template Instantiation
// =============================================================================

template class Edge<string>;
template class Edge<int>;
template class Edge<float>;
template class Edge<char>;

template class VertexNode<string>;
template class VertexNode<int>;
template class VertexNode<float>;
template class VertexNode<char>;

template class DGraphModel<string>;
template class DGraphModel<int>;
template class DGraphModel<float>;
template class DGraphModel<char>;