#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../data_structures/DirectedEdge.hpp"
#include "../data_structures/EdgeWeightedDigraph.hpp"

void testBasicConstruction() {
    // Test construction with valid vertex count
    EdgeWeightedDigraph g(5);

    assert(g.V() == 5);
    assert(g.E() == 0);

    // Test that all vertices have zero indegree and outdegree initially
    for (int v = 0; v < 5; ++v) {
        assert(g.indegree(v) == 0);
        assert(g.outdegree(v) == 0);
        assert(g.adj(v).empty());
    }

    std::cout << "testBasicConstruction passed\n";
}

void testInvalidConstruction() {
    // Test construction with negative vertex count
    try {
        EdgeWeightedDigraph invalid(-1);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Number of vertices cannot be negative") != std::string::npos);
        assert(msg.find("-1") != std::string::npos);
    }

    // Test construction with zero vertices (should be valid)
    EdgeWeightedDigraph empty(0);
    assert(empty.V() == 0);
    assert(empty.E() == 0);

    std::cout << "testInvalidConstruction passed\n";
}

void testRandomConstruction() {
    // Test construction with random edges
    EdgeWeightedDigraph g(10, 15);

    assert(g.V() == 10);
    assert(g.E() == 15);

    // Verify that all edges are valid
    auto edges = g.edges();
    assert(edges.size() == 15);

    for (const auto& edge : edges) {
        assert(edge.from() >= 0 && edge.from() < 10);
        assert(edge.to() >= 0 && edge.to() < 10);
        assert(edge.weight() >= 0.0 && edge.weight() < 1.0);  // Random weights in [0,1)
    }

    std::cout << "testRandomConstruction passed\n";
}

void testInvalidRandomConstruction() {
    // Test random construction with negative vertices
    try {
        EdgeWeightedDigraph invalid(-1, 5);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Number of vertices cannot be negative") != std::string::npos);
    }

    // Test random construction with negative edges
    try {
        EdgeWeightedDigraph invalid(5, -1);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Number of edges cannot be negative") != std::string::npos);
    }

    std::cout << "testInvalidRandomConstruction passed\n";
}

void testAddEdge() {
    EdgeWeightedDigraph g(4);

    // Add some directed edges
    DirectedEdge e1(0, 1, 2.5);
    DirectedEdge e2(1, 2, 3.0);
    DirectedEdge e3(0, 2, 4.0);

    g.addEdge(e1);
    assert(g.E() == 1);

    g.addEdge(e2);
    assert(g.E() == 2);

    g.addEdge(e3);
    assert(g.E() == 3);

    // Check adjacency lists
    auto adj0 = g.adj(0);
    assert(adj0.size() == 2);  // 0 -> 1, 0 -> 2

    auto adj1 = g.adj(1);
    assert(adj1.size() == 1);  // 1 -> 2

    auto adj2 = g.adj(2);
    assert(adj2.size() == 0);  // No outgoing edges

    std::cout << "testAddEdge passed\n";
}

void testInvalidVertexAccess() {
    EdgeWeightedDigraph g(5);

    // Test invalid vertex in addEdge
    try {
        g.addEdge(DirectedEdge(-1, 0, 1.0));
        assert(false);
    } catch (const std::invalid_argument& e) {
        // std::string msg = e.what();
        // assert(msg.find("Vertex -1 is out of range") != std::string::npos);
    }

    try {
        g.addEdge(DirectedEdge(0, 5, 1.0));  // 5 is out of range for 5 vertices
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex 5 is out of range") != std::string::npos);
    }

    // Test invalid vertex in adj()
    try {
        g.adj(-1);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex -1 is out of range") != std::string::npos);
    }

    try {
        g.adj(5);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex 5 is out of range") != std::string::npos);
    }

    std::cout << "testInvalidVertexAccess passed\n";
}

void testDegreeCalculations() {
    EdgeWeightedDigraph g(4);

    // Add edges: 0->1, 1->2, 0->2, 2->1
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(1, 2, 2.0));
    g.addEdge(DirectedEdge(0, 2, 3.0));
    g.addEdge(DirectedEdge(2, 1, 4.0));

    // Test outdegrees
    assert(g.outdegree(0) == 2);  // 0 -> {1, 2}
    assert(g.outdegree(1) == 1);  // 1 -> {2}
    assert(g.outdegree(2) == 1);  // 2 -> {1}
    assert(g.outdegree(3) == 0);  // No outgoing edges

    // Test indegrees
    assert(g.indegree(0) == 0);   // No incoming edges
    assert(g.indegree(1) == 2);   // 0->1, 2->1
    assert(g.indegree(2) == 2);   // 1->2, 0->2
    assert(g.indegree(3) == 0);   // No incoming edges

    std::cout << "testDegreeCalculations passed\n";
}

void testSelfLoops() {
    EdgeWeightedDigraph g(3);

    // Add self-loops
    g.addEdge(DirectedEdge(0, 0, 1.0));
    g.addEdge(DirectedEdge(1, 1, 2.0));

    assert(g.E() == 2);

    // Self-loops should contribute to both indegree and outdegree
    assert(g.indegree(0) == 1);
    assert(g.outdegree(0) == 1);
    assert(g.indegree(1) == 1);
    assert(g.outdegree(1) == 1);
    assert(g.indegree(2) == 0);
    assert(g.outdegree(2) == 0);

    // Check adjacency lists contain self-loops
    auto adj0 = g.adj(0);
    assert(adj0.size() == 1);
    assert(adj0[0].from() == 0 && adj0[0].to() == 0);

    std::cout << "testSelfLoops passed\n";
}

void testParallelEdges() {
    EdgeWeightedDigraph g(3);

    // Add parallel edges between same vertices
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(0, 1, 2.0));  // Parallel edge with different weight
    g.addEdge(DirectedEdge(0, 1, 3.0));  // Another parallel edge

    assert(g.E() == 3);
    assert(g.outdegree(0) == 3);
    assert(g.indegree(1) == 3);

    // Check that all parallel edges are stored
    auto adj0 = g.adj(0);
    assert(adj0.size() == 3);

    std::set<double> weights;
    for (const auto& edge : adj0) {
        assert(edge.from() == 0 && edge.to() == 1);
        weights.insert(edge.weight());
    }

    assert(weights.size() == 3);
    assert(weights.count(1.0) == 1);
    assert(weights.count(2.0) == 1);
    assert(weights.count(3.0) == 1);

    std::cout << "testParallelEdges passed\n";
}

void testEdgesMethod() {
    EdgeWeightedDigraph g(4);

    // Add some edges
    DirectedEdge e1(0, 1, 1.0);
    DirectedEdge e2(1, 2, 2.0);
    DirectedEdge e3(0, 2, 3.0);
    DirectedEdge e4(2, 3, 4.0);

    g.addEdge(e1);
    g.addEdge(e2);
    g.addEdge(e3);
    g.addEdge(e4);

    auto edges = g.edges();
    assert(edges.size() == 4);

    // Verify all edges are present (order may vary due to adjacency list implementation)
    std::set<std::pair<int, int>> edgeSet;
    for (const auto& edge : edges) {
        edgeSet.insert({edge.from(), edge.to()});
    }

    assert(edgeSet.count({0, 1}) == 1);
    assert(edgeSet.count({1, 2}) == 1);
    assert(edgeSet.count({0, 2}) == 1);
    assert(edgeSet.count({2, 3}) == 1);

    std::cout << "testEdgesMethod passed\n";
}

void testEmptyGraph() {
    EdgeWeightedDigraph g(0);

    assert(g.V() == 0);
    assert(g.E() == 0);

    auto edges = g.edges();
    assert(edges.empty());

    std::cout << "testEmptyGraph passed\n";
}

void testSingleVertex() {
    EdgeWeightedDigraph g(1);

    assert(g.V() == 1);
    assert(g.E() == 0);
    assert(g.indegree(0) == 0);
    assert(g.outdegree(0) == 0);

    // Add self-loop
    g.addEdge(DirectedEdge(0, 0, 5.0));
    assert(g.E() == 1);
    assert(g.indegree(0) == 1);
    assert(g.outdegree(0) == 1);

    std::cout << "testSingleVertex passed\n";
}

void testCompleteDigraph() {
    // Create complete digraph (every vertex connected to every other vertex)
    const int n = 4;
    EdgeWeightedDigraph g(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {  // No self-loops in this test
                g.addEdge(DirectedEdge(i, j, i * n + j));  // Unique weights
            }
        }
    }

    assert(g.E() == n * (n - 1));  // n*(n-1) edges in complete digraph without self-loops

    // Each vertex should have outdegree and indegree of n-1
    for (int v = 0; v < n; ++v) {
        assert(g.outdegree(v) == n - 1);
        assert(g.indegree(v) == n - 1);
    }

    std::cout << "testCompleteDigraph passed\n";
}

void testReverse() {
    EdgeWeightedDigraph g(4);

    // Add edges to original graph
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(1, 2, 2.0));
    g.addEdge(DirectedEdge(0, 2, 3.0));
    g.addEdge(DirectedEdge(2, 3, 4.0));

    // Create reverse graph
    EdgeWeightedDigraph rev = g.reverse();

    assert(rev.V() == g.V());
    assert(rev.E() == g.E());

    // Check that edges are reversed
    auto originalEdges = g.edges();
    auto reversedEdges = rev.edges();

    for (const auto& originalEdge : originalEdges) {
        bool foundReverse = false;
        for (const auto& reversedEdge : reversedEdges) {
            if (reversedEdge.from() == originalEdge.to() &&
                reversedEdge.to() == originalEdge.from() &&
                reversedEdge.weight() == originalEdge.weight()) {
                foundReverse = true;
                break;
            }
        }
        assert(foundReverse);
    }

    std::cout << "testReverse passed\n";
}

void testNegativeWeights() {
    EdgeWeightedDigraph g(3);

    // Add edges with negative weights
    g.addEdge(DirectedEdge(0, 1, -1.5));
    g.addEdge(DirectedEdge(1, 2, -2.0));
    g.addEdge(DirectedEdge(0, 2, 0.5));

    assert(g.E() == 3);

    auto edges = g.edges();
    bool hasNegative = false;
    for (const auto& edge : edges) {
        if (edge.weight() < 0) {
            hasNegative = true;
        }
    }
    assert(hasNegative);

    std::cout << "testNegativeWeights passed\n";
}

void testLargeGraph() {
    // Test with larger graph
    const int V = 1000;
    EdgeWeightedDigraph g(V);

    // Add edges in a chain: 0->1->2->...->999
    for (int i = 0; i < V - 1; ++i) {
        g.addEdge(DirectedEdge(i, i + 1, static_cast<double>(i)));
    }

    assert(g.V() == V);
    assert(g.E() == V - 1);

    // Check degrees
    assert(g.indegree(0) == 0);      // First vertex has no incoming edges
    assert(g.outdegree(V - 1) == 0); // Last vertex has no outgoing edges

    for (int i = 1; i < V - 1; ++i) {
        assert(g.indegree(i) == 1);   // Middle vertices have indegree 1
        assert(g.outdegree(i) == 1);  // Middle vertices have outdegree 1
    }

    std::cout << "testLargeGraph passed\n";
}

void testRandomGraphProperties() {
    // Test properties of random graphs
    EdgeWeightedDigraph g(50, 100);

    assert(g.V() == 50);
    assert(g.E() == 100);

    // Sum of all indegrees should equal sum of all outdegrees (= number of edges)
    int totalIndegree = 0;
    int totalOutdegree = 0;

    for (int v = 0; v < g.V(); ++v) {
        totalIndegree += g.indegree(v);
        totalOutdegree += g.outdegree(v);
    }

    assert(totalIndegree == g.E());
    assert(totalOutdegree == g.E());
    assert(totalIndegree == totalOutdegree);

    std::cout << "testRandomGraphProperties passed\n";
}

void testDegreeInvariant() {
    EdgeWeightedDigraph g(5);

    // Add various edges
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(1, 2, 2.0));
    g.addEdge(DirectedEdge(2, 0, 3.0));
    g.addEdge(DirectedEdge(0, 0, 4.0));  // Self-loop
    g.addEdge(DirectedEdge(1, 1, 5.0));  // Self-loop

    // Verify fundamental property: sum of indegrees = sum of outdegrees = number of edges
    int totalIndegree = 0;
    int totalOutdegree = 0;

    for (int v = 0; v < g.V(); ++v) {
        totalIndegree += g.indegree(v);
        totalOutdegree += g.outdegree(v);
    }

    assert(totalIndegree == g.E());
    assert(totalOutdegree == g.E());

    std::cout << "testDegreeInvariant passed\n";
}

void testCopyBehavior() {
    EdgeWeightedDigraph g1(3);
    g1.addEdge(DirectedEdge(0, 1, 1.0));
    g1.addEdge(DirectedEdge(1, 2, 2.0));

    // Test copy constructor
    EdgeWeightedDigraph g2 = g1;
    assert(g2.V() == g1.V());
    assert(g2.E() == g1.E());

    // Verify edges are the same
    auto edges1 = g1.edges();
    auto edges2 = g2.edges();
    assert(edges1.size() == edges2.size());

    // Test that copies are independent
    g1.addEdge(DirectedEdge(2, 0, 3.0));
    assert(g1.E() == 3);
    assert(g2.E() == 2);  // g2 should be unchanged

    std::cout << "testCopyBehavior passed\n";
}

void testEdgeOrdering() {
    EdgeWeightedDigraph g(3);

    // Add edges in specific order
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(0, 2, 2.0));
    g.addEdge(DirectedEdge(0, 1, 3.0));  // Another edge 0->1

    // Check that adjacency list maintains some order (depends on implementation)
    auto adj0 = g.adj(0);
    assert(adj0.size() == 3);

    // Since we use push_front, most recent edge should be first
    assert(adj0[0].weight() == 3.0);  // Last added edge

    std::cout << "testEdgeOrdering passed\n";
}

void testStronglyConnectedComponent() {
    // Create a simple strongly connected component
    EdgeWeightedDigraph g(3);

    // Create cycle: 0->1->2->0
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(1, 2, 2.0));
    g.addEdge(DirectedEdge(2, 0, 3.0));

    assert(g.E() == 3);

    // Each vertex should have indegree and outdegree of 1
    for (int v = 0; v < 3; ++v) {
        assert(g.indegree(v) == 1);
        assert(g.outdegree(v) == 1);
    }

    // Test reverse graph (useful for SCC algorithms)
    EdgeWeightedDigraph rev = g.reverse();

    // Reverse should also be strongly connected
    for (int v = 0; v < 3; ++v) {
        assert(rev.indegree(v) == 1);
        assert(rev.outdegree(v) == 1);
    }

    std::cout << "testStronglyConnectedComponent passed\n";
}

int main() {
    std::cout << "Running EdgeWeightedDigraph tests...\n";

    testBasicConstruction();
    testInvalidConstruction();
    testRandomConstruction();
    testInvalidRandomConstruction();
    testAddEdge();
    testInvalidVertexAccess();
    testDegreeCalculations();
    testSelfLoops();
    testParallelEdges();
    testEdgesMethod();
    testEmptyGraph();
    testSingleVertex();
    testCompleteDigraph();
    testReverse();
    testNegativeWeights();
    testLargeGraph();
    testRandomGraphProperties();
    testDegreeInvariant();
    testCopyBehavior();
    testEdgeOrdering();
    testStronglyConnectedComponent();

    std::cout << "All tests passed!\n";
    return 0;
}
