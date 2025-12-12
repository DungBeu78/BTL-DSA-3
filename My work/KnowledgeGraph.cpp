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

// TODO: Implement other methods of Edge:

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

template <class T>
string Edge<T>::toString()
{
    stringstream ss;
    ss << "(" << from->getVertex() << ", "
       << to->getVertex() << ", "
       << weight << ")";
    return ss.str();
}

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
    return this->vertex;
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

template <class T>
string VertexNode<T>::toString()
{
    stringstream ss;
    ss << "(" << vertex << ", "
       << inDegree << ", "
       << outDegree << ", [";

    for (size_t i = 0; i < adList.size(); ++i)
    {
        ss << adList[i]->toString();
        if (i + 1 < adList.size())
            ss << ", ";
    }

    ss << "])";
    return ss.str();
}

template <class T>
std::vector<Edge<T> *> VertexNode<T>::getOutwardEdges()
{
    return this->adList;
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
    this->clear();
}

// TODO: Implement other methods of DGraphModel:

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

template <class T>
std::string DGraphModel<T>::vertex2Str(VertexNode<T> &node)
{
    if (this->vertex2str != nullptr)
        return this->vertex2str(node.vertex);

    return node.toString();
}

template <class T>
std::string DGraphModel<T>::edge2Str(Edge<T> &edge)
{
    return edge.toString();
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

template <class T>
std::vector<Edge<T> *> DGraphModel<T>::getOutwardEdges(T from)
{
    // Find vertex from
    VertexNode<T> *fromNode = this->getVertexNode(from);

    // Throw exception
    if (fromNode == nullptr)
        throw VertexNotFoundException();

    return fromNode->getOutwardEdges();
}

template <class T>
void DGraphModel<T>::connect(T from, T to, float weight)
{
    // TODO: Connect two vertices 'from' and 'to'

    // Find vertex from and to
    VertexNode<T> *fromNode = this->getVertexNode(from);
    VertexNode<T> *toNode = this->getVertexNode(to);

    // Throw exception
    if (fromNode == nullptr || toNode == nullptr)
        throw VertexNotFoundException();

    fromNode->connect(toNode, weight);
}

template <class T>
void DGraphModel<T>::disconnect(T from, T to)
{
    // Find vertex from and to
    VertexNode<T> *fromNode = this->getVertexNode(from);
    VertexNode<T> *toNode = this->getVertexNode(to);

    // Throw exception
    if (fromNode == nullptr || toNode == nullptr)
        throw VertexNotFoundException();

    fromNode->removeTo(toNode);
}

template <class T>
bool DGraphModel<T>::connected(T from, T to)
{
    // Find vertex from and to
    VertexNode<T> *fromNode = this->getVertexNode(from);
    VertexNode<T> *toNode = this->getVertexNode(to);

    // Throw exception
    if (fromNode == nullptr || toNode == nullptr)
        throw VertexNotFoundException();

    return (fromNode->getEdge(toNode) != nullptr);
}

template <class T>
int DGraphModel<T>::size()
{
    return this->nodeList.size();
}

template <class T>
bool DGraphModel<T>::empty()
{
    return (this->nodeList.size() == 0);
}

template <class T>
void DGraphModel<T>::clear()
{
    for (VertexNode<T> *vertex : this->nodeList)
    {
        for (Edge<T> *edge : vertex->adList)
            delete edge;
        delete vertex;
    }

    this->nodeList.clear();
}

template <class T>
int DGraphModel<T>::inDegree(T vertex)
{
    // Find vertex
    VertexNode<T> *current = this->getVertexNode(vertex);

    // Throw exception
    if (current == nullptr)
        throw VertexNotFoundException();

    return current->inDegree();
}

template <class T>
int DGraphModel<T>::outDegree(T vertex)
{
    // Find vertex
    VertexNode<T> *current = this->getVertexNode(vertex);

    // Throw exception
    if (current == nullptr)
        throw VertexNotFoundException();

    return current->outDegree();
}

template <class T>
std::vector<T> DGraphModel<T>::vertices()
{
    vector<T> result;

    for (VertexNode<T> *ver : nodeList)
        result.push_back(ver->vertex);

    return result;
}

template <class T>
string DGraphModel<T>::toString()
{
    stringstream ss;
    ss << "[";

    for (size_t i = 0; i < nodeList.size(); ++i)
    {
        ss << nodeList[i]->toString();
        if (i + 1 < nodeList.size())
            ss << ", ";
    }

    ss << "]";
    return ss.str();
}

// Not implement
template <class T>
std::string DGraphModel<T>::BFS(T start)
{
    return "";
}

// Not implement
template <class T>
std::string DGraphModel<T>::DFS(T start)
{
    return "";
}

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