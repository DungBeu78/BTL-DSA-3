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
    string fromStr = (from->vertex2str ? from->vertex2str(from->vertex) : "");
    string toStr = (to->vertex2str ? to->vertex2str(to->vertex) : "");

    return "(" + fromStr + ", " + toStr + ", " + to_string(weight) + ")";
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

    // Update adjacency list
    this->adList.push_back(newEdge);
    to->adList.push_back(newEdge);

    // Update data
    this->outDegree_++;
    to->inDegree_++;
}

template <class T>
Edge<T> *VertexNode<T>::getEdge(VertexNode<T> *to)
{
    for (Edge<T> *edge : this->adList)
        if (edge != nullptr && edge->to == to && edge->from == this)
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
    Edge<T> *edge = this->getEdge(to);
    if (edge == nullptr)
        return;

    // Update adjacency list
    for (auto it = this->adList.begin(); it != this->adList.end(); ++it)
    {
        if (*it == edge)
        {
            this->adList.erase(it);
            break;
        }
    }

    for (auto it = to->adList.begin(); it != to->adList.end(); ++it)
    {
        if (*it == edge)
        {
            to->adList.erase(it);
            break;
        }
    }

    // Update data
    this->outDegree_--;
    to->inDegree_--;

    // Delete edge
    delete edge;
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
       << this->inDegree() << ", "
       << this->outDegree() << ", [";

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
    vector<Edge<T> *> out;
    for (Edge<T> *edge : this->adList)
    {
        if (edge != nullptr && edge->from == this)
            out.push_back(edge);
    }
    return out;
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
        if (this->vertexEQ != nullptr)
            if (this->vertexEQ(current->vertex, vertex))
                return current;
        else
            if (current->vertex == vertex)
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
        if (this->vertexEQ != nullptr)
            if (this->vertexEQ(current->vertex, vertex))
                return true;
            else if (current->vertex == vertex)
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
    for (VertexNode<T> *node : nodeList)
    {
        for (Edge<T> *edge : node->adList)
        {
            if (edge != nullptr && edge->from == node)
                delete edge;
        }
        node->adList.clear();
    }

    // Delete nodes
    for (VertexNode<T> *node : nodeList)
        delete node;
    nodeList.clear();
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

template <class T>
string DGraphModel<T>::BFS(T start)
{
    if (this->nodeList.size() == 0)
        return "[]";

    VertexNode<T> *startNode = this->getVertexNode(start);
    if (startNode == nullptr)
        throw VertexNotFoundException();

    vector<VertexNode<T> *> visited;
    vector<VertexNode<T> *> q;
    int idx = 0;

    visited.push_back(startNode);
    q.push_back(startNode);

    stringstream ss;
    ss << "[";
    bool first = true;

    while (idx < (int)q.size())
    {
        VertexNode<T> *u = q[idx++];

        if (!first)
            ss << ", ";
        ss << this->vertex2Str(*u);
        first = false;

        vector<Edge<T> *> edges = u->getOutwardEdges();
        for (Edge<T> *e : edges)
        {
            VertexNode<T> *v = e->getTo();

            bool seen = false;
            for (VertexNode<T> *x : visited)
            {
                if (x == v)
                {
                    seen = true;
                    break;
                }
            }

            if (!seen)
            {
                visited.push_back(v);
                q.push_back(v);
            }
        }
    }

    ss << "]";
    return ss.str();
}

template <class T>
void DGraphModel<T>::DFS_helper(
    VertexNode<T> *u,
    vector<VertexNode<T> *> &visited,
    stringstream &ss,
    bool &first)
{
    visited.push_back(u);

    if (!first)
        ss << ", ";
    ss << this->vertex2Str(*u);
    first = false;

    vector<Edge<T> *> edges = u->getOutwardEdges();
    for (Edge<T> *e : edges)
    {
        VertexNode<T> *v = e->getTo(); // bạn đã thêm getTo() cho Edge rồi

        bool seen = false;
        for (VertexNode<T> *x : visited)
        {
            if (x == v)
            {
                seen = true;
                break;
            }
        }
        if (!seen)
            DFS_helper(v, visited, ss, first);
    }
}

template <class T>
string DGraphModel<T>::DFS(T start)
{
    if (this->nodeList.size() == 0)
        return "[]";

    VertexNode<T> *startNode = this->getVertexNode(start);
    if (startNode == nullptr)
        throw VertexNotFoundException();

    vector<VertexNode<T> *> visited;
    stringstream ss;
    ss << "[";
    bool first = true;

    DFS_helper(startNode, visited, ss, first);

    ss << "]";
    return ss.str();
}

// =============================================================================
// Class KnowledgeGraph Implementation
// =============================================================================

KnowledgeGraph::KnowledgeGraph()
{
    // TODO: Initialize the KnowledgeGraph
    auto vertexEQ = [](string &a, string &b) -> bool
    { return a == b; };
    auto vertex2str = [](string &s) -> string
    { return s; };

    this->graph = DGraphModel<string>(vertexEQ, vertex2str);
    this->entities = vector<string>();
}

void KnowledgeGraph::addEntity(string entity)
{
    // TODO: Add a new entity to the Knowledge Graph
    if (this->graph.contains(entity))
        throw EntityExistsException();

    this->graph.add(entity);
    this->entities.push_back(entity);
}

void KnowledgeGraph::addRelation(string from, string to, float weight)
{
    // TODO: Add a directed relation
    if (!this->graph.contains(from) || !this->graph.contains(to))
        throw EntityNotFoundException();

    this->graph.connect(from, to, weight);
}

// TODO: Implement other methods of KnowledgeGraph:

vector<string> KnowledgeGraph::getAllEntities()
{
    return this->entities;
}

vector<string> KnowledgeGraph::getNeighbors(string entity)
{
    if (!this->graph.contains(entity))
        throw EntityNotFoundException();

    vector<Edge<string> *> edges = this->graph.getOutwardEdges(entity);
    vector<string> neighbors;

    for (Edge<string> *edge : edges)
    {
        neighbors.push_back(edge->getTo()->getVertex());
    }

    return neighbors;
}

string KnowledgeGraph::bfs(string start)
{
    if (!this->graph.contains(start))
        throw EntityNotFoundException();

    return this->graph.BFS(start);
}

string KnowledgeGraph::dfs(string start)
{
    if (!this->graph.contains(start))
        throw EntityNotFoundException();

    return this->graph.DFS(start);
}

bool KnowledgeGraph::isReachable(string from, string to)
{
    if (!this->graph.contains(from) || !this->graph.contains(to))
        throw EntityNotFoundException();

    vector<string> visited;
    vector<string> q;
    int idx = 0;

    visited.push_back(from);
    q.push_back(from);

    while (idx < (int)q.size())
    {
        string current = q[idx++];

        if (current == to)
            return true;

        vector<Edge<string> *> edges = this->graph.getOutwardEdges(current);
        for (Edge<string> *edge : edges)
        {
            string neighbor = edge->getTo()->getVertex();

            bool seen = false;
            for (string &v : visited)
            {
                if (v == neighbor)
                {
                    seen = true;
                    break;
                }
            }

            if (!seen)
            {
                visited.push_back(neighbor);
                q.push_back(neighbor);
            }
        }
    }

    return false;
}

string KnowledgeGraph::toString()
{
    return this->graph.toString();
}

vector<string> KnowledgeGraph::getRelatedEntities(string entity, int depth)
{
    if (!this->graph.contains(entity))
        throw EntityNotFoundException();

    // depth <= 0 -> không có liên quan nào (không tính chính nó)
    if (depth <= 0)
        return vector<string>();

    vector<string> related; // kết quả, không trùng
    vector<string> qNode;   // queue giả: danh sách node
    vector<int> qDepth;     // queue giả: độ sâu tương ứng
    int idx = 0;

    qNode.push_back(entity);
    qDepth.push_back(0);

    while (idx < (int)qNode.size())
    {
        string current = qNode[idx];
        int currDepth = qDepth[idx];
        idx++;

        if (currDepth >= depth)
            continue;

        vector<Edge<string> *> edges = this->graph.getOutwardEdges(current);
        for (Edge<string> *edge : edges)
        {
            string neighbor = edge->getTo()->getVertex();
            if (neighbor == entity)
                continue;

            // check duplicate trong related
            bool existed = false;
            for (string &s : related)
            {
                if (s == neighbor)
                {
                    existed = true;
                    break;
                }
            }

            if (!existed)
            {
                related.push_back(neighbor);
                qNode.push_back(neighbor);
                qDepth.push_back(currDepth + 1);
            }
        }
    }

    return related;
}

string KnowledgeGraph::findCommonAncestors(string entity1, string entity2)
{
    if (!this->graph.contains(entity1) || !this->graph.contains(entity2))
        throw EntityNotFoundException();

    vector<string> a1, a2;
    vector<int> d1, d2;

    reverseBfsDistances(entity1, a1, d1);
    reverseBfsDistances(entity2, a2, d2);

    // tìm ancestor chung có d1+d2 nhỏ nhất
    string best = "";
    int bestSum = 0;

    for (int i = 0; i < (int)a1.size(); ++i)
    {
        for (int j = 0; j < (int)a2.size(); ++j)
        {
            if (a1[i] == a2[j])
            {
                int sum = d1[i] + d2[j];
                if (best == "" || sum < bestSum)
                {
                    best = a1[i];
                    bestSum = sum;
                }
            }
        }
    }

    if (best == "")
        return "No common ancestor";
    return best;
}

vector<string> KnowledgeGraph::getIncomingNeighbors(const string &target)
{
    vector<string> incoming;
    vector<string> all = this->getAllEntities(); // entities list trong KG

    for (string &u : all)
    {
        vector<Edge<string> *> edges = this->graph.getOutwardEdges(u);
        for (Edge<string> *e : edges)
        {
            string v = e->getTo()->getVertex();
            if (v == target)
            {
                // tránh trùng
                bool seen = false;
                for (string &x : incoming)
                    if (x == u)
                    {
                        seen = true;
                        break;
                    }
                if (!seen)
                    incoming.push_back(u);
            }
        }
    }
    return incoming;
}

void KnowledgeGraph::reverseBfsDistances(
    const string &start,
    vector<string> &nodes,
    vector<int> &dist)
{
    nodes.clear();
    dist.clear();

    vector<string> q;
    vector<int> qd;
    int idx = 0;

    q.push_back(start);
    qd.push_back(0);

    // visited list
    vector<string> visited;
    visited.push_back(start);

    while (idx < (int)q.size())
    {
        string cur = q[idx];
        int cd = qd[idx];
        idx++;

        // ancestors: không tính chính nó nếu bạn muốn
        if (cur != start)
        {
            nodes.push_back(cur);
            dist.push_back(cd);
        }

        vector<string> incoming = getIncomingNeighbors(cur);
        for (string &p : incoming)
        {
            bool seen = false;
            for (string &v : visited)
                if (v == p)
                {
                    seen = true;
                    break;
                }
            if (!seen)
            {
                visited.push_back(p);
                q.push_back(p);
                qd.push_back(cd + 1);
            }
        }
    }
}

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