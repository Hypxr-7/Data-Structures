#include <cassert>
#include <iostream>
#include <sstream>

#include "../data_structures/Digraph.hpp"

void testDigraph() {
    // Test 1: Constructor and basic properties
    Digraph g1(5);
    assert(g1.V() == 5);
    assert(g1.E() == 0);

    // Test 2: Invalid constructor
    try {
        Digraph g(-1);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        // Expected exception
    }

    // Test 3: Add edges and check counts
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(2, 0);
    assert(g1.E() == 3);

    // Test 4: Check degrees
    assert(g1.outdegree(0) == 1);
    assert(g1.outdegree(1) == 1);
    assert(g1.outdegree(2) == 1);
    assert(g1.indegree(0) == 1);
    assert(g1.indegree(1) == 1);
    assert(g1.indegree(2) == 1);

    // Test 5: Check adjacency lists
    std::vector<int> adj0 = g1.adj(0);
    assert(adj0.size() == 1);
    assert(adj0[0] == 1);

    std::vector<int> adj1 = g1.adj(1);
    assert(adj1.size() == 1);
    assert(adj1[0] == 2);

    // Test 6: Self-loop
    g1.addEdge(3, 3);
    assert(g1.E() == 4);
    assert(g1.outdegree(3) == 1);
    assert(g1.indegree(3) == 1);

    // Test 7: Multiple edges from same vertex
    g1.addEdge(0, 3);
    g1.addEdge(0, 4);
    assert(g1.outdegree(0) == 3);
    assert(g1.indegree(3) == 2);
    assert(g1.indegree(4) == 1);

    // Test 8: Vertex validation
    try {
        g1.addEdge(-1, 0);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        // Expected exception
    }

    try {
        g1.addEdge(0, 5);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        // Expected exception
    }

    try {
        g1.adj(-1);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        // Expected exception
    }

    // Test 9: Reverse digraph
    Digraph g2(3);
    g2.addEdge(0, 1);
    g2.addEdge(1, 2);
    g2.addEdge(0, 2);

    Digraph rev = g2.reverse();
    assert(rev.V() == 3);
    assert(rev.E() == 3);
    assert(rev.outdegree(1) == 1); // Original edge 0->1 becomes 1->0
    assert(rev.outdegree(2) == 2); // Original edges 1->2, 0->2 become 2->1, 2->0
    assert(rev.indegree(0) == 2);  // Receives edges from 1 and 2

    // Test 10: Empty adjacency list
    assert(g1.adj(4).empty());
    assert(g1.outdegree(4) == 0);

    // Test 11: Stream output (basic test)
    std::ostringstream oss;
    oss << g2;
    std::string output = oss.str();
    assert(output.find("Number of Vertices: 3") != std::string::npos);
    assert(output.find("Number of Edges: 3") != std::string::npos);

    // Test 12: Single vertex graph
    Digraph g3(1);
    assert(g3.V() == 1);
    assert(g3.E() == 0);
    assert(g3.outdegree(0) == 0);
    assert(g3.indegree(0) == 0);

    std::cout << "All Digraph tests passed!" << std::endl;
}

int main() {
    testDigraph();
    return 0;
}