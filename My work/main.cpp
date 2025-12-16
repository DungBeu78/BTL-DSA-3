#include "KnowledgeGraph.h"

static void printVec(const vector<string> &v)
{
    cout << "[";
    for (size_t i = 0; i < v.size(); ++i)
    {
        cout << v[i];
        if (i + 1 < v.size())
            cout << ", ";
    }
    cout << "]";
}

void tc_KG_001_entities_basic()
{
    cout << "tc_KG_001_entities_basic\n";
    KnowledgeGraph kg;

    kg.addEntity("E1");
    kg.addEntity("E2");
    kg.addEntity("E3");

    vector<string> ents = kg.getAllEntities();
    cout << "Entities size = " << ents.size() << " (expect 3)\n";
    printVec(ents);
    cout << "\n";

    // duplicate addEntity
    try
    {
        kg.addEntity("E2");
        cout << "[FAIL] expected exception when adding duplicate entity\n";
    }
    catch (...)
    {
        cout << "[OK] duplicate addEntity throws exception\n";
    }
    cout << "\n";
}

void tc_KG_002_relation_neighbors()
{
    cout << "tc_KG_002_relation_neighbors\n";
    KnowledgeGraph kg;

    kg.addEntity("A");
    kg.addEntity("B");
    kg.addEntity("C");

    kg.addRelation("A", "B", 1.0f);
    kg.addRelation("A", "C", 2.0f);
    kg.addRelation("B", "C", 3.0f);

    vector<string> nbA = kg.getNeighbors("A");
    cout << "Neighbors(A) size = " << nbA.size() << " (expect 2)\n";
    printVec(nbA);
    cout << "\n";

    vector<string> nbC = kg.getNeighbors("C");
    cout << "Neighbors(C) size = " << nbC.size() << " (expect 0)\n";
    printVec(nbC);
    cout << "\n";

    // EntityNotFound for getNeighbors
    try
    {
        kg.getNeighbors("X");
        cout << "[FAIL] expected exception for getNeighbors(X)\n";
    }
    catch (...)
    {
        cout << "[OK] getNeighbors(X) throws exception\n";
    }
    cout << "\n";
}

void tc_KG_003_bfs_dfs_order()
{
    cout << "tc_KG_003_bfs_dfs_order\n";
    KnowledgeGraph kg;

    // Graph:
    // A -> B, A -> C, B -> D, C -> E
    kg.addEntity("A");
    kg.addEntity("B");
    kg.addEntity("C");
    kg.addEntity("D");
    kg.addEntity("E");

    kg.addRelation("A", "B", 1);
    kg.addRelation("A", "C", 1);
    kg.addRelation("B", "D", 1);
    kg.addRelation("C", "E", 1);

    cout << "BFS(A) = " << kg.bfs("A") << "\n";
    cout << "DFS(A) = " << kg.dfs("A") << "\n";

    // start not found
    try
    {
        kg.bfs("X");
        cout << "[FAIL] expected bfs(X) throws\n";
    }
    catch (...)
    {
        cout << "[OK] bfs(X) throws exception\n";
    }

    try
    {
        kg.dfs("X");
        cout << "[FAIL] expected dfs(X) throws\n";
    }
    catch (...)
    {
        cout << "[OK] dfs(X) throws exception\n";
    }

    cout << "\n";
}

void tc_KG_004_isReachable()
{
    cout << "tc_KG_004_isReachable\n";
    KnowledgeGraph kg;

    kg.addEntity("A");
    kg.addEntity("B");
    kg.addEntity("C");
    kg.addEntity("D");

    kg.addRelation("A", "B", 1);
    kg.addRelation("B", "C", 1);
    // D isolated

    cout << "isReachable(A,C) = " << (kg.isReachable("A", "C") ? "true" : "false") << " (expect true)\n";
    cout << "isReachable(C,A) = " << (kg.isReachable("C", "A") ? "true" : "false") << " (expect false)\n";
    cout << "isReachable(A,D) = " << (kg.isReachable("A", "D") ? "true" : "false") << " (expect false)\n";

    // entity not found
    try
    {
        kg.isReachable("A", "X");
        cout << "[FAIL] expected exception\n";
    }
    catch (...)
    {
        cout << "[OK] isReachable(A,X) throws exception\n";
    }

    cout << "\n";
}

void tc_KG_005_relatedEntities_depth()
{
    cout << "tc_KG_005_relatedEntities_depth\n";
    KnowledgeGraph kg;

    // A -> B -> C -> D
    kg.addEntity("A");
    kg.addEntity("B");
    kg.addEntity("C");
    kg.addEntity("D");

    kg.addRelation("A", "B", 1);
    kg.addRelation("B", "C", 1);
    kg.addRelation("C", "D", 1);

    auto r0 = kg.getRelatedEntities("A", 0);
    cout << "Related(A,0) size = " << r0.size() << " (expect 0)\n";

    auto r1 = kg.getRelatedEntities("A", 1);
    cout << "Related(A,1) = ";
    printVec(r1);
    cout << " (expect [B])\n";

    auto r2 = kg.getRelatedEntities("A", 2);
    cout << "Related(A,2) = ";
    printVec(r2);
    cout << " (expect contains B,C)\n";

    auto r3 = kg.getRelatedEntities("A", 3);
    cout << "Related(A,3) = ";
    printVec(r3);
    cout << " (expect contains B,C,D)\n";

    try
    {
        kg.getRelatedEntities("X", 2);
        cout << "[FAIL] expected exception\n";
    }
    catch (...)
    {
        cout << "[OK] getRelatedEntities(X,2) throws exception\n";
    }

    cout << "\n";
}

void tc_KG_006_cycle_safety()
{
    cout << "tc_KG_006_cycle_safety\n";
    KnowledgeGraph kg;

    // Cycle: A -> B -> C -> A
    kg.addEntity("A");
    kg.addEntity("B");
    kg.addEntity("C");
    kg.addRelation("A", "B", 1);
    kg.addRelation("B", "C", 1);
    kg.addRelation("C", "A", 1);

    cout << "BFS(A) = " << kg.bfs("A") << "\n";
    cout << "DFS(A) = " << kg.dfs("A") << "\n";
    cout << "isReachable(A,C) = " << (kg.isReachable("A", "C") ? "true" : "false") << " (expect true)\n";
    cout << "isReachable(C,B) = " << (kg.isReachable("C", "B") ? "true" : "false") << " (expect true)\n";

    cout << "Related(A,2) = ";
    auto rel = kg.getRelatedEntities("A", 2);
    printVec(rel);
    cout << " (expect contains B,C)\n";

    cout << "\n";
}

void tc_KG_007_commonAncestors_basic()
{
    cout << "tc_KG_007_commonAncestors_basic\n";
    KnowledgeGraph kg;

    //      R
    //    /   \
    //   A     B
    //    \   /
    //      C
    // Edges: R->A, R->B, A->C, B->C
    kg.addEntity("R");
    kg.addEntity("A");
    kg.addEntity("B");
    kg.addEntity("C");
    kg.addRelation("R", "A", 1);
    kg.addRelation("R", "B", 1);
    kg.addRelation("A", "C", 1);
    kg.addRelation("B", "C", 1);

    // common ancestors of A and B => [R], LCA should be R
    cout << "findCommonAncestors(A,B) = " << kg.findCommonAncestors("A", "B") << " (expect R)\n";

    // common ancestors of C and A => A (A is ancestor of C), maybe also R; lowest should be A
    cout << "findCommonAncestors(C,A) = " << kg.findCommonAncestors("C", "A") << " (expect A)\n";

    // no common ancestor
    kg.addEntity("X");
    cout << "findCommonAncestors(A,X) = " << kg.findCommonAncestors("A", "X") << " (expect No common ancestor)\n";

    try
    {
        kg.findCommonAncestors("A", "NOPE");
        cout << "[FAIL] expected exception\n";
    }
    catch (...)
    {
        cout << "[OK] findCommonAncestors(A,NOPE) throws exception\n";
    }

    cout << "\n";
}

void tc_KG_008_basic_like_sample()
{
    cout << "tc_KG_008_basic_like_sample\n";
    KnowledgeGraph kg;

    kg.addEntity("Entity1");
    kg.addEntity("Entity2");
    kg.addEntity("Entity3");

    vector<string> entities = kg.getAllEntities();
    cout << "Number of entities: " << entities.size() << " (expect 3)\n";

    kg.addRelation("Entity1", "Entity2", 1.0f);
    kg.addRelation("Entity2", "Entity3", 1.0f);

    vector<string> neighbors = kg.getNeighbors("Entity1");
    cout << "Entity1 has " << neighbors.size() << " neighbor(s) (expect 1)\n";
    cout << "\n";
}

int main()
{
    cout << "Nigga";
    tc_KG_001_entities_basic();
    tc_KG_002_relation_neighbors();
    tc_KG_003_bfs_dfs_order();
    tc_KG_004_isReachable();
    tc_KG_005_relatedEntities_depth();
    tc_KG_006_cycle_safety();
    tc_KG_007_commonAncestors_basic();
    tc_KG_008_basic_like_sample();
    cout << "All test cases done.\n";
    return 0;
}
