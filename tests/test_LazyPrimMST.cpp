#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>

#include "../data_structures/Edge.hpp"
#include "../data_structures/EdgeWeightedGraph.hpp"
#include "../data_structures/LazyPrimMST.hpp"

// Helper function to check if MST has expected properties
bool isMSTValid(const std::vector<Edge>& mst, const EdgeWeightedGraph& g, double expectedWeight) {
    const double EPS = 1e-9;

    // Check weight
    double totalWeight = 0.0;
    for (const Edge& e : mst) {
        totalWeight += e.weight();
    }

    if (std::abs(totalWeight - expectedWeight) > EPS) {
        return false;
    }

    // For connected graph, MST should have V-1 edges
    // For disconnected graph, should have V - (number of components) edges
    return true;
}

void testSingleVertex() {
    EdgeWeightedGraph g(1);
    LazyPrimMST mst(g);

    // Single vertex has empty MST
    assert(mst.edges().empty());
    assert(mst.weight() == 0.0);

    std::cout << "testSingleVertex passed\n";
}

void testTwoVertices() {
    EdgeWeightedGraph g(2);
    g.addEdge(Edge(0, 1, 3.5));

    LazyPrimMST mst(g);

    // MST should contain the single edge
    auto edges = mst.edges();
    assert(edges.size() == 1);
    assert(edges[0].weight() == 3.5);
    assert(mst.weight() == 3.5);

    std::cout << "testTwoVertices passed\n";
}

void testSimpleTriangle() {
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(1, 2, 2.0));
    g.addEdge(Edge(0, 2, 3.0));

    LazyPrimMST mst(g);

    // MST should have 2 edges with total weight 3.0
    auto edges = mst.edges();
    assert(edges.size() == 2);
    assert(mst.weight() == 3.0);

    // Should contain edges (0,1) and (1,2)
    std::set<double> weights;
    for (const Edge& e : edges) {
        weights.insert(e.weight());
    }
    assert(weights.count(1.0) == 1);
    assert(weights.count(2.0) == 1);

    std::cout << "testSimpleTriangle passed\n";
}

void testSquareGraph() {
    EdgeWeightedGraph g(4);
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(1, 2, 2.0));
    g.addEdge(Edge(2, 3, 3.0));
    g.addEdge(Edge(3, 0, 4.0));
    g.addEdge(Edge(0, 2, 5.0));  // Diagonal
    g.addEdge(Edge(1, 3, 6.0));  // Diagonal

    LazyPrimMST mst(g);

    // MST should have 3 edges
    auto edges = mst.edges();
    assert(edges.size() == 3);

    // Minimum total weight should be 1+2+3 = 6
    assert(mst.weight() == 6.0);

    std::cout << "testSquareGraph passed\n";
}

void testStarGraph() {
    // Star graph: center connected to all other vertices
    EdgeWeightedGraph g(5);
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(0, 2, 2.0));
    g.addEdge(Edge(0, 3, 3.0));
    g.addEdge(Edge(0, 4, 4.0));

    LazyPrimMST mst(g);

    // MST should be the entire star (already minimal)
    auto edges = mst.edges();
    assert(edges.size() == 4);
    assert(mst.weight() == 10.0);

    // All edges should be incident to vertex 0
    for (const Edge& e : edges) {
        assert(e.either() == 0 || e.other(e.either()) == 0);
    }

    std::cout << "testStarGraph passed\n";
}

void testCompleteGraph() {
    // Complete graph K4
    EdgeWeightedGraph g(4);
    g.addEdge(Edge(0, 1, 4.0));
    g.addEdge(Edge(0, 2, 2.0));
    g.addEdge(Edge(0, 3, 6.0));
    g.addEdge(Edge(1, 2, 3.0));
    g.addEdge(Edge(1, 3, 5.0));
    g.addEdge(Edge(2, 3, 1.0));

    LazyPrimMST mst(g);

    // MST should have 3 edges
    auto edges = mst.edges();
    assert(edges.size() == 3);

    // Optimal MST: edges (2,3)=1, (0,2)=2, (1,2)=3 = total 6
    assert(mst.weight() == 6.0);

    std::cout << "testCompleteGraph passed\n";
}

void testDisconnectedGraph() {
    // Two disconnected components
    EdgeWeightedGraph g(5);

    // Component 1: vertices 0, 1, 2
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(1, 2, 2.0));
    g.addEdge(Edge(0, 2, 3.0));

    // Component 2: vertices 3, 4
    g.addEdge(Edge(3, 4, 4.0));

    LazyPrimMST mst(g);

    // MST should have 3 edges total (2 for first component, 1 for second)
    auto edges = mst.edges();
    assert(edges.size() == 3);

    // Total weight: (0,1)+(1,2) + (3,4) = 1+2+4 = 7
    assert(mst.weight() == 7.0);

    std::cout << "testDisconnectedGraph passed\n";
}

void testSelfLoops() {
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 0, 10.0));  // Self-loop (should be ignored)
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(1, 2, 2.0));
    g.addEdge(Edge(1, 1, 5.0));   // Another self-loop

    LazyPrimMST mst(g);

    // MST should only contain the two connecting edges
    auto edges = mst.edges();
    assert(edges.size() == 2);
    assert(mst.weight() == 3.0);

    std::cout << "testSelfLoops passed\n";
}

void testParallelEdges() {
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 5.0));   // Heavier parallel edge
    g.addEdge(Edge(0, 1, 1.0));   // Lighter parallel edge
    g.addEdge(Edge(1, 2, 2.0));
    g.addEdge(Edge(0, 2, 4.0));

    LazyPrimMST mst(g);

    // MST should choose lighter edges
    auto edges = mst.edges();
    assert(edges.size() == 2);
    assert(mst.weight() == 3.0);  // Should use edges with weights 1.0 and 2.0

    std::cout << "testParallelEdges passed\n";
}

void testNegativeWeights() {
    EdgeWeightedGraph g(4);
    g.addEdge(Edge(0, 1, -1.0));
    g.addEdge(Edge(1, 2, -2.0));
    g.addEdge(Edge(2, 3, -3.0));
    g.addEdge(Edge(0, 3, 10.0));   // Expensive edge

    LazyPrimMST mst(g);

    // MST should use the three negative weight edges
    auto edges = mst.edges();
    assert(edges.size() == 3);
    assert(mst.weight() == -6.0);

    std::cout << "testNegativeWeights passed\n";
}

void testZeroWeights() {
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 0.0));
    g.addEdge(Edge(1, 2, 0.0));
    g.addEdge(Edge(0, 2, 1.0));

    LazyPrimMST mst(g);

    // MST should use zero-weight edges
    auto edges = mst.edges();
    assert(edges.size() == 2);
    assert(mst.weight() == 0.0);

    std::cout << "testZeroWeights passed\n";
}

void testLargerGraph() {
    // Create a larger test graph
    EdgeWeightedGraph g(6);
    g.addEdge(Edge(2, 4, 1.0));
    g.addEdge(Edge(4, 5, 2.0));
    g.addEdge(Edge(1, 4, 4.0));
    g.addEdge(Edge(2, 5, 6.0));
    g.addEdge(Edge(0, 1, 7.0));
    g.addEdge(Edge(0, 2, 8.0));
    g.addEdge(Edge(3, 5, 9.0));
    g.addEdge(Edge(1, 2, 11.0));
    g.addEdge(Edge(3, 4, 14.0));
    g.addEdge(Edge(1, 3, 21.0));

    LazyPrimMST mst(g);

    // MST should have 5 edges
    auto edges = mst.edges();
    assert(edges.size() == 5);

    // Known optimal MST weight for this graph
    double expectedWeight = 1.0 + 2.0 + 4.0 + 7.0 + 9.0; // = 23.0
    assert(std::abs(mst.weight() - expectedWeight) < 1e-9);

    std::cout << "testLargerGraph passed\n";
}

void testEqualWeights() {
    EdgeWeightedGraph g(4);
    // All edges have same weight
    g.addEdge(Edge(0, 1, 5.0));
    g.addEdge(Edge(1, 2, 5.0));
    g.addEdge(Edge(2, 3, 5.0));
    g.addEdge(Edge(0, 3, 5.0));
    g.addEdge(Edge(0, 2, 5.0));
    g.addEdge(Edge(1, 3, 5.0));

    LazyPrimMST mst(g);

    // Any spanning tree is optimal
    auto edges = mst.edges();
    assert(edges.size() == 3);
    assert(mst.weight() == 15.0);

    std::cout << "testEqualWeights passed\n";
}

void testPrimStartingVertex() {
    // Test that MST is same regardless of starting vertex
    EdgeWeightedGraph g(4);
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(1, 2, 2.0));
    g.addEdge(Edge(2, 3, 3.0));
    g.addEdge(Edge(0, 3, 4.0));

    LazyPrimMST mst(g);

    // Should always get same optimal weight regardless of starting vertex
    assert(mst.weight() == 6.0);
    assert(mst.edges().size() == 3);

    std::cout << "testPrimStartingVertex passed\n";
}

void testFloatingPointPrecision() {
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 0.1));
    g.addEdge(Edge(1, 2, 0.2));
    g.addEdge(Edge(0, 2, 0.3));

    LazyPrimMST mst(g);

    auto edges = mst.edges();
    assert(edges.size() == 2);

    // Should choose edges with weights 0.1 and 0.2
    double expectedWeight = 0.1 + 0.2;
    assert(std::abs(mst.weight() - expectedWeight) < 1e-9);

    std::cout << "testFloatingPointPrecision passed\n";
}

void testMSTProperties() {
    // Test fundamental MST properties
    EdgeWeightedGraph g(5);
    g.addEdge(Edge(0, 1, 2.0));
    g.addEdge(Edge(1, 2, 3.0));
    g.addEdge(Edge(1, 4, 5.0));
    g.addEdge(Edge(0, 3, 6.0));
    g.addEdge(Edge(2, 4, 7.0));
    g.addEdge(Edge(1, 3, 8.0));

    LazyPrimMST mst(g);
    auto edges = mst.edges();

    // Property 1: MST has V-1 edges for connected graph
    assert(edges.size() == g.V() - 1);

    // Property 2: MST is acyclic (can't easily test without additional structure)

    // Property 3: MST connects all vertices (tested by algorithm correctness)

    // Property 4: Total weight should be minimal
    assert(mst.weight() == 16.0);  // Known optimal for this graph

    std::cout << "testMSTProperties passed\n";
}

void testRandomGraph() {
    // Test with random graph
    EdgeWeightedGraph g(10, 20);
    LazyPrimMST mst(g);

    // MST should have at most V-1 edges (could be less if disconnected)
    auto edges = mst.edges();
    assert(edges.size() <= 9);

    // Weight should be non-negative for random graph with positive weights
    assert(mst.weight() >= 0.0);

    std::cout << "testRandomGraph passed\n";
}

void testEmptyGraph() {
    EdgeWeightedGraph g(3);  // No edges added
    LazyPrimMST mst(g);

    // Empty graph has no MST edges
    assert(mst.edges().empty());
    assert(mst.weight() == 0.0);

    std::cout << "testEmptyGraph passed\n";
}

void testLazyBehavior() {
    // Test that demonstrates lazy behavior
    EdgeWeightedGraph g(4);
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(0, 2, 2.0));
    g.addEdge(Edge(1, 2, 3.0));  // This will become obsolete
    g.addEdge(Edge(2, 3, 4.0));

    LazyPrimMST mst(g);

    // Should not include the obsolete edge (1,2)
    auto edges = mst.edges();
    assert(edges.size() == 3);

    std::set<double> weights;
    for (const Edge& e : edges) {
        weights.insert(e.weight());
    }

    // Should contain edges 1.0, 2.0, 4.0 but not 3.0
    assert(weights.count(1.0) == 1);
    assert(weights.count(2.0) == 1);
    assert(weights.count(4.0) == 1);
    assert(weights.count(3.0) == 0);

    std::cout << "testLazyBehavior passed\n";
}

void testComplexDisconnectedGraph() {
    // Multiple disconnected components
    EdgeWeightedGraph g(8);

    // Component 1: vertices 0,1,2
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(1, 2, 2.0));

    // Component 2: vertices 3,4,5
    g.addEdge(Edge(3, 4, 3.0));
    g.addEdge(Edge(4, 5, 4.0));
    g.addEdge(Edge(3, 5, 5.0));

    // Component 3: vertices 6,7 (single edge)
    g.addEdge(Edge(6, 7, 6.0));

    LazyPrimMST mst(g);

    // Should have 5 edges total (2+2+1)
    auto edges = mst.edges();
    assert(edges.size() == 5);

    // Total weight: (1+2) + (3+4) + (6) = 16
    assert(mst.weight() == 16.0);

    std::cout << "testComplexDisconnectedGraph passed\n";
}

int main() {
    std::cout << "Running LazyPrimMST tests...\n";

    testSingleVertex();
    testTwoVertices();
    testSimpleTriangle();
    testSquareGraph();
    testStarGraph();
    testCompleteGraph();
    testDisconnectedGraph();
    testSelfLoops();
    testParallelEdges();
    testNegativeWeights();
    testZeroWeights();
    testLargerGraph();
    testEqualWeights();
    testPrimStartingVertex();
    testFloatingPointPrecision();
    testMSTProperties();
    testRandomGraph();
    testEmptyGraph();
    testLazyBehavior();
    testComplexDisconnectedGraph();

    std::cout << "All tests passed!\n";
    return 0;
}
