#include <algorithm>
#include <cassert>
#include <iostream>

#include "../data_structures/DepthFirstOrder.hpp"
#include "../data_structures/Digraph.hpp"

void testSimpleDAG() {
    // Create simple DAG: 0->1->2, 0->2
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(0, 2);

    DepthFirstOrder dfo(g);

    // Check that all vertices have valid pre/post orders
    for (int v = 0; v < 3; v++) {
        assert(dfo.pre(v) >= 0 && dfo.pre(v) < 3);
        assert(dfo.post(v) >= 0 && dfo.post(v) < 3);
    }

    // Check preorder and postorder sequences
    std::vector<int> preorder = dfo.pre();
    std::vector<int> postorder = dfo.post();
    assert(preorder.size() == 3);
    assert(postorder.size() == 3);

    // Vertex 0 should be visited first (lowest preorder)
    assert(dfo.pre(0) == 0);

    // Vertex 2 should finish first in this case (lowest postorder)
    // since it has no outgoing edges

    std::cout << "testSimpleDAG passed\n";
}

void testLinearChain() {
    // Create linear chain: 0->1->2->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    DepthFirstOrder dfo(g);

    // In a linear chain starting from 0:
    // Preorder should be: 0, 1, 2, 3
    // Postorder should be: 3, 2, 1, 0

    std::vector<int> preorder = dfo.pre();
    std::vector<int> postorder = dfo.post();

    assert(preorder.size() == 4);
    assert(postorder.size() == 4);

    // Check preorder sequence
    assert(preorder[0] == 0);
    assert(preorder[1] == 1);
    assert(preorder[2] == 2);
    assert(preorder[3] == 3);

    // Check postorder sequence (reverse of chain)
    assert(postorder[0] == 3);
    assert(postorder[1] == 2);
    assert(postorder[2] == 1);
    assert(postorder[3] == 0);

    // Check individual pre/post numbers
    assert(dfo.pre(0) == 0);
    assert(dfo.pre(1) == 1);
    assert(dfo.pre(2) == 2);
    assert(dfo.pre(3) == 3);

    assert(dfo.post(0) == 3);
    assert(dfo.post(1) == 2);
    assert(dfo.post(2) == 1);
    assert(dfo.post(3) == 0);

    std::cout << "testLinearChain passed\n";
}

void testDisconnectedComponents() {
    // Create disconnected components: 0->1 and 2->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(2, 3);

    DepthFirstOrder dfo(g);

    std::vector<int> preorder = dfo.pre();
    std::vector<int> postorder = dfo.post();

    assert(preorder.size() == 4);
    assert(postorder.size() == 4);

    // Should visit vertices 0,1 first, then 2,3
    // since DFS starts from vertex 0
    assert(dfo.pre(0) == 0);
    assert(dfo.pre(1) == 1);
    assert(dfo.pre(2) == 2);
    assert(dfo.pre(3) == 3);

    // Postorder: 1,0,3,2
    assert(dfo.post(1) == 0);
    assert(dfo.post(0) == 1);
    assert(dfo.post(3) == 2);
    assert(dfo.post(2) == 3);

    std::cout << "testDisconnectedComponents passed\n";
}

void testSingleVertex() {
    // Single vertex with no edges
    Digraph g(1);

    DepthFirstOrder dfo(g);

    assert(dfo.pre(0) == 0);
    assert(dfo.post(0) == 0);

    std::vector<int> preorder = dfo.pre();
    std::vector<int> postorder = dfo.post();

    assert(preorder.size() == 1);
    assert(postorder.size() == 1);
    assert(preorder[0] == 0);
    assert(postorder[0] == 0);

    std::cout << "testSingleVertex passed\n";
}

void testTreeStructure() {
    // Create tree: 0->1, 0->2, 1->3, 1->4
    Digraph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);

    DepthFirstOrder dfo(g);

    std::vector<int> preorder = dfo.pre();
    std::vector<int> postorder = dfo.post();

    assert(preorder.size() == 5);
    assert(postorder.size() == 5);

    // Root should be first in preorder, last in postorder
    assert(dfo.pre(0) == 0);
    assert(dfo.post(0) == 4);  // Last to finish

    // Leaves should finish first
    // The exact order depends on adjacency list order, but leaves finish before parents

    std::cout << "testTreeStructure passed\n";
}

void testReversePostorder() {
    // Create DAG for topological sorting: 0->1->3, 0->2->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 3);
    g.addEdge(0, 2);
    g.addEdge(2, 3);

    DepthFirstOrder dfo(g);

    std::vector<int> reversePost = dfo.reversePost();
    std::vector<int> postorder = dfo.post();

    assert(reversePost.size() == 4);

    // Reverse postorder should be reverse of postorder
    for (int i = 0; i < 4; i++) {
        assert(reversePost[i] == postorder[3 - i]);
    }

    // In a valid topological order, vertex 0 should come before 1 and 2
    // and vertex 3 should come last
    auto pos0 = std::find(reversePost.begin(), reversePost.end(), 0);
    auto pos1 = std::find(reversePost.begin(), reversePost.end(), 1);
    auto pos2 = std::find(reversePost.begin(), reversePost.end(), 2);
    auto pos3 = std::find(reversePost.begin(), reversePost.end(), 3);

    assert(pos0 < pos1);  // 0 comes before 1
    assert(pos0 < pos2);  // 0 comes before 2
    assert(pos1 < pos3);  // 1 comes before 3
    assert(pos2 < pos3);  // 2 comes before 3

    std::cout << "testReversePostorder passed\n";
}

void testComplexDAG() {
    // More complex DAG: multiple paths
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(3, 5);

    DepthFirstOrder dfo(g);

    std::vector<int> preorder = dfo.pre();
    std::vector<int> postorder = dfo.post();
    std::vector<int> reversePost = dfo.reversePost();

    assert(preorder.size() == 6);
    assert(postorder.size() == 6);
    assert(reversePost.size() == 6);

    // Check that pre and post numbers are unique and in range
    std::vector<bool> preUsed(6, false);
    std::vector<bool> postUsed(6, false);

    for (int v = 0; v < 6; v++) {
        int preNum = dfo.pre(v);
        int postNum = dfo.post(v);

        assert(preNum >= 0 && preNum < 6);
        assert(postNum >= 0 && postNum < 6);
        assert(!preUsed[preNum]);
        assert(!postUsed[postNum]);

        preUsed[preNum] = true;
        postUsed[postNum] = true;
    }

    std::cout << "testComplexDAG passed\n";
}

void testDirectedCycle() {
    // Create directed cycle: 0->1->2->0
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    DepthFirstOrder dfo(g);

    std::vector<int> preorder = dfo.pre();
    std::vector<int> postorder = dfo.post();

    assert(preorder.size() == 3);
    assert(postorder.size() == 3);

    // Even with cycles, DFS still assigns valid pre/post numbers
    for (int v = 0; v < 3; v++) {
        assert(dfo.pre(v) >= 0 && dfo.pre(v) < 3);
        assert(dfo.post(v) >= 0 && dfo.post(v) < 3);
    }

    std::cout << "testDirectedCycle passed\n";
}

void testEmptyDigraph() {
    // Empty digraph (vertices but no edges)
    Digraph g(3);

    DepthFirstOrder dfo(g);

    std::vector<int> preorder = dfo.pre();
    std::vector<int> postorder = dfo.post();

    assert(preorder.size() == 3);
    assert(postorder.size() == 3);

    // Each vertex visited in isolation
    assert(dfo.pre(0) == 0 && dfo.post(0) == 0);
    assert(dfo.pre(1) == 1 && dfo.post(1) == 1);
    assert(dfo.pre(2) == 2 && dfo.post(2) == 2);

    std::cout << "testEmptyDigraph passed\n";
}

void testParentChildRelationship() {
    // Test that parents finish after children in DFS tree
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);

    DepthFirstOrder dfo(g);

    // In DFS tree: 0 is parent of 1,2; 1 is parent of 3
    // Children should finish before parents
    assert(dfo.post(3) < dfo.post(1));  // 3 finishes before 1
    assert(dfo.post(1) < dfo.post(0));  // 1 finishes before 0
    assert(dfo.post(2) < dfo.post(0));  // 2 finishes before 0

    // Parents should be visited before children
    assert(dfo.pre(0) < dfo.pre(1));    // 0 visited before 1
    assert(dfo.pre(0) < dfo.pre(2));    // 0 visited before 2
    assert(dfo.pre(1) < dfo.pre(3));    // 1 visited before 3

    std::cout << "testParentChildRelationship passed\n";
}

void testInvalidVertex() {
    Digraph g(3);
    g.addEdge(0, 1);

    DepthFirstOrder dfo(g);

    // Test invalid vertex indices
    try {
        dfo.pre(-1);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        dfo.post(3);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "testInvalidVertex passed\n";
}

int main() {
    std::cout << "Running DepthFirstOrder tests...\n";

    testSimpleDAG();
    testLinearChain();
    testDisconnectedComponents();
    testSingleVertex();
    testTreeStructure();
    testReversePostorder();
    testComplexDAG();
    testDirectedCycle();
    testEmptyDigraph();
    testParentChildRelationship();
    testInvalidVertex();

    std::cout << "All tests passed!\n";
    return 0;
}