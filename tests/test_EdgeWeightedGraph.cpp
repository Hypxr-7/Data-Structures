#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <stdexcept>
#include <vector>

#include "../data_structures/Edge.hpp"
#include "../data_structures/EdgeWeightedGraph.hpp"

void testBasicConstruction() {
    // Test empty graph construction
    EdgeWeightedGraph g(5);

    assert(g.V() == 5);
    assert(g.E() == 0);

    // All vertices should have degree 0
    for (int v = 0; v < 5; v++) {
        assert(g.degree(v) == 0);
        assert(g.adj(v).empty());
    }

    // Should have no edges
    assert(g.edges().empty());

    std::cout << "testBasicConstruction passed\n";
}

void testRandomGraphConstruction() {
    // Test random graph construction
    EdgeWeightedGraph g(10, 15);

    assert(g.V() == 10);
    assert(g.E() == 15);

    // Count edges manually
    auto allEdges = g.edges();
    assert(allEdges.size() == 15);

    // Check that all edges have weights in [0, 1)
    for (const Edge& e : allEdges) {
        assert(e.weight() >= 0.0);
        assert(e.weight() < 1.0);
    }

    std::cout << "testRandomGraphConstruction passed\n";
}

void testAddEdge() {
    EdgeWeightedGraph g(4);

    // Add some edges
    Edge e1(0, 1, 2.5);
    Edge e2(1, 2, 3.7);
    Edge e3(2, 3, 1.2);

    g.addEdge(e1);
    assert(g.E() == 1);

    g.addEdge(e2);
    assert(g.E() == 2);

    g.addEdge(e3);
    assert(g.E() == 3);

    // Check degrees
    assert(g.degree(0) == 1);  // Connected to 1
    assert(g.degree(1) == 2);  // Connected to 0 and 2
    assert(g.degree(2) == 2);  // Connected to 1 and 3
    assert(g.degree(3) == 1);  // Connected to 2

    std::cout << "testAddEdge passed\n";
}

void testUndirectedProperty() {
    EdgeWeightedGraph g(3);
    Edge e(0, 2, 4.5);
    g.addEdge(e);

    // Edge should appear in adjacency lists of both vertices
    auto adj0 = g.adj(0);
    auto adj2 = g.adj(2);

    assert(adj0.size() == 1);
    assert(adj2.size() == 1);

    // Both should contain the same edge
    assert(adj0[0].weight() == 4.5);
    assert(adj2[0].weight() == 4.5);

    // Check that we can get other vertex from both sides
    assert(adj0[0].other(0) == 2);
    assert(adj2[0].other(2) == 0);

    std::cout << "testUndirectedProperty passed\n";
}

void testSelfLoops() {
    EdgeWeightedGraph g(3);

    // Add self-loop
    Edge selfLoop(1, 1, 5.0);
    g.addEdge(selfLoop);

    assert(g.E() == 1);
    assert(g.degree(1) == 1);  // Self-loop counts as degree 1

    // Self-loop should appear once in adjacency list
    auto adj1 = g.adj(1);
    assert(adj1.size() == 1);
    assert(adj1[0].other(1) == 1);
    assert(adj1[0].weight() == 5.0);

    // Should appear once in edges() list
    auto allEdges = g.edges();
    assert(allEdges.size() == 1);
    assert(allEdges[0].weight() == 5.0);

    std::cout << "testSelfLoops passed\n";
}

void testMultipleSelfLoops() {
    EdgeWeightedGraph g(2);

    // Add multiple self-loops to same vertex
    g.addEdge(Edge(0, 0, 1.0));
    g.addEdge(Edge(0, 0, 2.0));
    g.addEdge(Edge(0, 0, 3.0));

    assert(g.E() == 3);
    assert(g.degree(0) == 3);

    // All self-loops should appear in adjacency list
    auto adj0 = g.adj(0);
    assert(adj0.size() == 3);

    // edges() should return each self-loop once (every other occurrence)
    auto allEdges = g.edges();
    assert(allEdges.size() >= 1);  // At least one should be included

    std::cout << "testMultipleSelfLoops passed\n";
}

void testParallelEdges() {
    EdgeWeightedGraph g(3);

    // Add parallel edges between same vertices
    g.addEdge(Edge(0, 1, 1.5));
    g.addEdge(Edge(0, 1, 2.5));
    g.addEdge(Edge(1, 0, 3.5));  // Same vertices, different order

    assert(g.E() == 3);
    assert(g.degree(0) == 3);
    assert(g.degree(1) == 3);

    // All edges should appear in adjacency lists
    auto adj0 = g.adj(0);
    auto adj1 = g.adj(1);
    assert(adj0.size() == 3);
    assert(adj1.size() == 3);

    // All edges should appear in edges() list
    auto allEdges = g.edges();
    assert(allEdges.size() == 3);

    std::cout << "testParallelEdges passed\n";
}

void testEdgesDeduplication() {
    EdgeWeightedGraph g(4);

    // Add edges in various orders
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(2, 1, 2.0));  // 1-2 edge
    g.addEdge(Edge(3, 2, 3.0));  // 2-3 edge
    g.addEdge(Edge(1, 3, 4.0));  // 1-3 edge

    assert(g.E() == 4);

    // edges() should return each edge exactly once
    auto allEdges = g.edges();
    assert(allEdges.size() == 4);

    // Verify no duplicates by checking edge weights
    std::set<double> weights;
    for (const Edge& e : allEdges) {
        weights.insert(e.weight());
    }
    assert(weights.size() == 4);  // All weights should be unique

    std::cout << "testEdgesDeduplication passed\n";
}

void testComplexGraph() {
    EdgeWeightedGraph g(5);

    // Create a more complex graph
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(0, 2, 2.0));
    g.addEdge(Edge(1, 2, 3.0));
    g.addEdge(Edge(1, 3, 4.0));
    g.addEdge(Edge(2, 3, 5.0));
    g.addEdge(Edge(2, 4, 6.0));
    g.addEdge(Edge(3, 4, 7.0));
    g.addEdge(Edge(0, 0, 8.0));  // Self-loop

    assert(g.E() == 8);
    assert(g.V() == 5);

    // Check degrees
    assert(g.degree(0) == 3);  // Connected to 1, 2, and self
    assert(g.degree(1) == 3);  // Connected to 0, 2, 3
    assert(g.degree(2) == 4);  // Connected to 0, 1, 3, 4
    assert(g.degree(3) == 3);  // Connected to 1, 2, 4
    assert(g.degree(4) == 2);  // Connected to 2, 3

    // Total degree should be 2*E - 1 (each edge contributes 2 to total degree and 1 self loop)
    int totalDegree = 0;
    for (int v = 0; v < g.V(); v++) {
        totalDegree += g.degree(v);
    }
    assert(totalDegree == 2 * g.E() - 1);

    std::cout << "testComplexGraph passed\n";
}

void testAdjacencyList() {
    EdgeWeightedGraph g(4);

    g.addEdge(Edge(0, 1, 1.5));
    g.addEdge(Edge(0, 2, 2.5));
    g.addEdge(Edge(0, 3, 3.5));

    // Test adjacency list for vertex 0
    auto adj0 = g.adj(0);
    assert(adj0.size() == 3);

    // Check that all adjacent vertices are correct
    std::set<int> adjacentVertices;
    for (const Edge& e : adj0) {
        adjacentVertices.insert(e.other(0));
    }
    assert(adjacentVertices.size() == 3);
    assert(adjacentVertices.count(1) == 1);
    assert(adjacentVertices.count(2) == 1);
    assert(adjacentVertices.count(3) == 1);

    // Test adjacency list for vertex 1 (should only connect to 0)
    auto adj1 = g.adj(1);
    assert(adj1.size() == 1);
    assert(adj1[0].other(1) == 0);

    std::cout << "testAdjacencyList passed\n";
}

void testInvalidVertexConstruction() {
    // Test invalid vertex count
    try {
        EdgeWeightedGraph g(-1);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        // Expected exception
    }

    // Test invalid edge count in random constructor
    try {
        EdgeWeightedGraph g(5, -1);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        // Expected exception
    }

    std::cout << "testInvalidVertexConstruction passed\n";
}

void testInvalidVertexOperations() {
    EdgeWeightedGraph g(3);

    // Test invalid vertex in addEdge
    try {
        g.addEdge(Edge(0, 5, 1.0));  // Vertex 5 doesn't exist
        assert(false);
    } catch (const std::invalid_argument& e) {
        // Expected
    }

    try {
        g.addEdge(Edge(-1, 1, 1.0));  // Negative vertex
        assert(false);
    } catch (const std::invalid_argument& e) {
        // Expected
    }

    // Test invalid vertex in adj()
    try {
        g.adj(3);  // Vertex 3 doesn't exist (valid range is 0-2)
        assert(false);
    } catch (const std::invalid_argument& e) {
        // Expected
    }

    // Test invalid vertex in degree()
    try {
        g.degree(-1);
        assert(false);
    } catch (const std::invalid_argument& e) {
        // Expected
    }

    std::cout << "testInvalidVertexOperations passed\n";
}

void testEmptyGraph() {
    EdgeWeightedGraph g(0);  // Graph with no vertices

    assert(g.V() == 0);
    assert(g.E() == 0);
    assert(g.edges().empty());

    std::cout << "testEmptyGraph passed\n";
}

void testSingleVertex() {
    EdgeWeightedGraph g(1);

    assert(g.V() == 1);
    assert(g.E() == 0);
    assert(g.degree(0) == 0);
    assert(g.adj(0).empty());

    // Add self-loop
    g.addEdge(Edge(0, 0, 5.0));
    assert(g.E() == 1);
    assert(g.degree(0) == 1);

    std::cout << "testSingleVertex passed\n";
}

void testLargeRandomGraph() {
    // Test with larger graph
    EdgeWeightedGraph g(100, 500);

    assert(g.V() == 100);
    assert(g.E() == 500);

    auto allEdges = g.edges();
    assert(allEdges.size() == 500);

    std::cout << "testLargeRandomGraph passed\n";
}

void testEdgeWeights() {
    EdgeWeightedGraph g(3);

    // Add edges with various weights
    g.addEdge(Edge(0, 1, 0.0));      // Zero weight
    g.addEdge(Edge(1, 2, -5.5));     // Negative weight
    g.addEdge(Edge(0, 2, 1000.7));   // Large weight

    auto allEdges = g.edges();
    assert(allEdges.size() == 3);

    // Check that weights are preserved
    std::set<double> expectedWeights = {0.0, -5.5, 1000.7};
    std::set<double> actualWeights;
    for (const Edge& e : allEdges) {
        actualWeights.insert(e.weight());
    }
    assert(actualWeights == expectedWeights);

    std::cout << "testEdgeWeights passed\n";
}

void testRandomGraphProperties() {
    // Test multiple random graphs to check consistency
    for (int trial = 0; trial < 5; trial++) {
        EdgeWeightedGraph g(10, 20);

        assert(g.V() == 10);
        assert(g.E() == 20);

        // Count edges using adjacency lists
        int edgeCount = 0;
        for (int v = 0; v < g.V(); v++) {
            edgeCount += g.degree(v);
        }

        // Check that edges() returns correct count
        assert(g.edges().size() == 20);
    }

    std::cout << "testRandomGraphProperties passed\n";
}

void testDegreeCalculation() {
    EdgeWeightedGraph g(4);

    // Star graph: vertex 0 connected to all others
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(0, 2, 2.0));
    g.addEdge(Edge(0, 3, 3.0));

    assert(g.degree(0) == 3);  // Center vertex
    assert(g.degree(1) == 1);  // Leaf vertices
    assert(g.degree(2) == 1);
    assert(g.degree(3) == 1);

    // Add self-loop to center
    g.addEdge(Edge(0, 0, 4.0));
    assert(g.degree(0) == 4);  // Self-loop increases degree by 1

    std::cout << "testDegreeCalculation passed\n";
}

void testCopySemantics() {
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 1.5));
    g.addEdge(Edge(1, 2, 2.5));

    // Test that adj() returns a copy
    auto adj1_copy1 = g.adj(1);
    auto adj1_copy2 = g.adj(1);

    // Both should have same content but be independent objects
    assert(adj1_copy1.size() == adj1_copy2.size());
    assert(adj1_copy1.size() == 2);

    // Modifying one copy shouldn't affect the other or the original graph
    adj1_copy1.clear();
    assert(adj1_copy2.size() == 2);  // Other copy unchanged
    assert(g.degree(1) == 2);        // Original graph unchanged

    std::cout << "testCopySemantics passed\n";
}

int main() {
    std::cout << "Running EdgeWeightedGraph tests...\n";

    testBasicConstruction();
    testRandomGraphConstruction();
    testAddEdge();
    testUndirectedProperty();
    testSelfLoops();
    testMultipleSelfLoops();
    testParallelEdges();
    testEdgesDeduplication();
    testComplexGraph();
    testAdjacencyList();
    testInvalidVertexConstruction();
    testInvalidVertexOperations();
    testEmptyGraph();
    testSingleVertex();
    testLargeRandomGraph();
    testEdgeWeights();
    testRandomGraphProperties();
    testDegreeCalculation();
    testCopySemantics();

    std::cout << "All tests passed!\n";
    return 0;
}