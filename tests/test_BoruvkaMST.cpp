#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include <vector>

#include "../data_structures/BoruvkaMST.hpp"
#include "../data_structures/Edge.hpp"
#include "../data_structures/EdgeWeightedGraph.hpp"

// Helper function to check if MST has expected properties
bool isMSTValid(const std::vector<Edge>& mst, const EdgeWeightedGraph& g, double expectedWeight) {
    const double EPS = 1e-9;

    // Check weight
    double totalWeight = 0.0;
    for (const Edge& e : mst) {
        totalWeight += e.weight();
    }

    return std::abs(totalWeight - expectedWeight) <= EPS;
}

void testSingleVertex() {
    EdgeWeightedGraph g(1);
    BoruvkaMST mst(g);

    // Single vertex has empty MST
    assert(mst.edges().empty());
    assert(mst.weight() == 0.0);

    std::cout << "testSingleVertex passed\n";
}

void testTwoVertices() {
    EdgeWeightedGraph g(2);
    g.addEdge(Edge(0, 1, 3.5));

    BoruvkaMST mst(g);

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

    BoruvkaMST mst(g);

    // MST should have 2 edges with total weight 3.0
    auto edges = mst.edges();
    assert(edges.size() == 2);
    assert(mst.weight() == 3.0);

    // Should contain edges (0,1) and (1,2) - the two lightest
    std::set<double> weights;
    for (const Edge& e : edges) {
        weights.insert(e.weight());
    }
    assert(weights.count(1.0) == 1);
    assert(weights.count(2.0) == 1);
    assert(weights.count(3.0) == 0);  // Heaviest edge should be excluded

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

    BoruvkaMST mst(g);

    // MST should have 3 edges with minimum total weight
    auto edges = mst.edges();
    assert(edges.size() == 3);

    // Should select edges 1.0, 2.0, 3.0 (total = 6.0)
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

    BoruvkaMST mst(g);

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

    BoruvkaMST mst(g);

    // MST should have 3 edges
    auto edges = mst.edges();
    assert(edges.size() == 3);

    // Should select edges 1.0, 2.0, 3.0 (total = 6.0)
    assert(mst.weight() == 6.0);

    // Verify the specific edges selected
    std::set<double> weights;
    for (const Edge& e : edges) {
        weights.insert(e.weight());
    }
    assert(weights.count(1.0) == 1);  // Edge (2,3)
    assert(weights.count(2.0) == 1);  // Edge (0,2)
    assert(weights.count(3.0) == 1);  // Edge (1,2)

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

    BoruvkaMST mst(g);

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

    BoruvkaMST mst(g);

    // MST should only contain the two connecting edges
    auto edges = mst.edges();
    assert(edges.size() == 2);
    assert(mst.weight() == 3.0);

    // Should not include any self-loops
    for (const Edge& e : edges) {
        assert(e.either() != e.other(e.either()));
    }

    std::cout << "testSelfLoops passed\n";
}

void testParallelEdges() {
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 5.0));   // Heavier parallel edge
    g.addEdge(Edge(0, 1, 1.0));   // Lighter parallel edge
    g.addEdge(Edge(1, 2, 2.0));
    g.addEdge(Edge(0, 2, 4.0));

    BoruvkaMST mst(g);

    // MST should choose lighter edges
    auto edges = mst.edges();
    assert(edges.size() == 2);
    assert(mst.weight() == 3.0);  // Should use edges with weights 1.0 and 2.0

    // Verify specific weights
    std::set<double> weights;
    for (const Edge& e : edges) {
        weights.insert(e.weight());
    }
    assert(weights.count(1.0) == 1);
    assert(weights.count(2.0) == 1);
    assert(weights.count(4.0) == 0);
    assert(weights.count(5.0) == 0);

    std::cout << "testParallelEdges passed\n";
}

void testNegativeWeights() {
    EdgeWeightedGraph g(4);
    g.addEdge(Edge(0, 1, -1.0));
    g.addEdge(Edge(1, 2, -2.0));
    g.addEdge(Edge(2, 3, -3.0));
    g.addEdge(Edge(0, 3, 10.0));   // Expensive edge
    g.addEdge(Edge(0, 2, 5.0));    // Another expensive edge
    g.addEdge(Edge(1, 3, 8.0));    // Another expensive edge

    BoruvkaMST mst(g);

    // MST should use the three negative weight edges
    auto edges = mst.edges();
    assert(edges.size() == 3);
    assert(mst.weight() == -6.0);

    // Verify all selected edges are negative
    for (const Edge& e : edges) {
        assert(e.weight() < 0);
    }

    std::cout << "testNegativeWeights passed\n";
}

void testZeroWeights() {
    EdgeWeightedGraph g(4);
    g.addEdge(Edge(0, 1, 0.0));
    g.addEdge(Edge(1, 2, 0.0));
    g.addEdge(Edge(2, 3, 0.0));
    g.addEdge(Edge(0, 2, 1.0));
    g.addEdge(Edge(1, 3, 1.0));

    BoruvkaMST mst(g);

    // MST should use zero-weight edges
    auto edges = mst.edges();
    assert(edges.size() == 3);
    assert(mst.weight() == 0.0);

    std::cout << "testZeroWeights passed\n";
}

void testBoruvkaIterations() {
    // Test that demonstrates Borůvka's iteration behavior
    // Each component finds its cheapest outgoing edge in parallel
    EdgeWeightedGraph g(8);

    // Create structure where components will merge in predictable pattern
    g.addEdge(Edge(0, 1, 1.0));  // Component {0,1} after iteration 1
    g.addEdge(Edge(2, 3, 2.0));  // Component {2,3} after iteration 1
    g.addEdge(Edge(4, 5, 3.0));  // Component {4,5} after iteration 1
    g.addEdge(Edge(6, 7, 4.0));  // Component {6,7} after iteration 1

    // Edges that will be selected in iteration 2
    g.addEdge(Edge(1, 2, 5.0));  // Connects {0,1} and {2,3}
    g.addEdge(Edge(5, 6, 6.0));  // Connects {4,5} and {6,7}

    // Edge that will be selected in iteration 3
    g.addEdge(Edge(3, 4, 7.0));  // Connects {0,1,2,3} and {4,5,6,7}

    BoruvkaMST mst(g);

    auto edges = mst.edges();
    assert(edges.size() == 7);  // Should have V-1 = 7 edges

    // Total weight: 1+2+3+4+5+6+7 = 28
    assert(mst.weight() == 28.0);

    std::cout << "testBoruvkaIterations passed\n";
}

void testLargerGraph() {
    // Create a larger test graph with known MST
    EdgeWeightedGraph g(6);
    g.addEdge(Edge(0, 1, 7.0));
    g.addEdge(Edge(0, 2, 8.0));
    g.addEdge(Edge(1, 2, 11.0));
    g.addEdge(Edge(1, 3, 21.0));
    g.addEdge(Edge(1, 4, 4.0));
    g.addEdge(Edge(2, 4, 1.0));   // Lightest edge
    g.addEdge(Edge(2, 5, 6.0));
    g.addEdge(Edge(3, 4, 14.0));
    g.addEdge(Edge(3, 5, 9.0));
    g.addEdge(Edge(4, 5, 2.0));   // Second lightest

    BoruvkaMST mst(g);

    // MST should have 5 edges
    auto edges = mst.edges();
    assert(edges.size() == 5);

    assert(std::abs(mst.weight() - 23.0) < 1e-9);

    std::cout << "testLargerGraph passed\n";
}

void testEqualWeights() {
    EdgeWeightedGraph g(4);
    // All edges have same weight - any spanning tree is optimal
    g.addEdge(Edge(0, 1, 5.0));
    g.addEdge(Edge(1, 2, 5.0));
    g.addEdge(Edge(2, 3, 5.0));
    g.addEdge(Edge(0, 3, 5.0));
    g.addEdge(Edge(0, 2, 5.0));
    g.addEdge(Edge(1, 3, 5.0));

    BoruvkaMST mst(g);

    // Any spanning tree is optimal
    auto edges = mst.edges();
    assert(edges.size() == 3);
    assert(mst.weight() == 15.0);

    std::cout << "testEqualWeights passed\n";
}

void testFloatingPointPrecision() {
    EdgeWeightedGraph g(4);
    g.addEdge(Edge(0, 1, 0.1));
    g.addEdge(Edge(1, 2, 0.2));
    g.addEdge(Edge(2, 3, 0.3));
    g.addEdge(Edge(0, 2, 0.4));
    g.addEdge(Edge(1, 3, 0.5));
    g.addEdge(Edge(0, 3, 0.6));

    BoruvkaMST mst(g);

    auto edges = mst.edges();
    assert(edges.size() == 3);

    // Should choose edges with weights 0.1, 0.2, 0.3
    double expectedWeight = 0.1 + 0.2 + 0.3;
    assert(std::abs(mst.weight() - expectedWeight) < 1e-9);

    std::cout << "testFloatingPointPrecision passed\n";
}

void testMSTProperties() {
    // Test fundamental MST properties
    EdgeWeightedGraph g(5);
    g.addEdge(Edge(0, 1, 2.0));
    g.addEdge(Edge(0, 3, 6.0));
    g.addEdge(Edge(1, 2, 3.0));
    g.addEdge(Edge(1, 3, 8.0));
    g.addEdge(Edge(1, 4, 5.0));
    g.addEdge(Edge(2, 4, 7.0));

    BoruvkaMST mst(g);
    auto edges = mst.edges();

    // Property 1: MST has V-1 edges for connected graph
    assert(edges.size() == g.V() - 1);

    // Property 2: Total weight should be minimal
    // Edges should be: (0,1)=2, (1,2)=3, (1,4)=5, (0,3)=6 = 16
    assert(mst.weight() == 16.0);

    std::cout << "testMSTProperties passed\n";
}

void testRandomGraph() {
    // Test with random graph
    EdgeWeightedGraph g(8, 15);
    BoruvkaMST mst(g);

    // MST should have at most V-1 edges (could be less if disconnected)
    auto edges = mst.edges();
    assert(edges.size() <= 7);

    // Weight should be non-negative for random graph with positive weights
    assert(mst.weight() >= 0.0);

    std::cout << "testRandomGraph passed\n";
}

void testEmptyGraph() {
    EdgeWeightedGraph g(3);  // No edges added
    BoruvkaMST mst(g);

    // Empty graph has no MST edges
    assert(mst.edges().empty());
    assert(mst.weight() == 0.0);

    std::cout << "testEmptyGraph passed\n";
}

void testComplexDisconnectedGraph() {
    // Multiple disconnected components
    EdgeWeightedGraph g(8);

    // Component 1: vertices 0,1,2
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(1, 2, 2.0));
    g.addEdge(Edge(0, 2, 5.0));  // Should be excluded

    // Component 2: vertices 3,4,5
    g.addEdge(Edge(3, 4, 3.0));
    g.addEdge(Edge(4, 5, 4.0));
    g.addEdge(Edge(3, 5, 7.0));  // Should be excluded

    // Component 3: vertices 6,7 (single edge)
    g.addEdge(Edge(6, 7, 6.0));

    BoruvkaMST mst(g);

    // Should have 5 edges total (2+2+1)
    auto edges = mst.edges();
    assert(edges.size() == 5);

    // Total weight: (1+2) + (3+4) + (6) = 16
    assert(mst.weight() == 16.0);

    // Should not include heavy edges within components
    std::set<double> weights;
    for (const Edge& e : edges) {
        weights.insert(e.weight());
    }
    assert(weights.count(5.0) == 0);  // Heavy edge in component 1
    assert(weights.count(7.0) == 0);  // Heavy edge in component 2

    std::cout << "testComplexDisconnectedGraph passed\n";
}

void testComponentMerging() {
    // Test that demonstrates how components merge in Borůvka's algorithm
    EdgeWeightedGraph g(6);

    // Initial phase: each vertex finds its cheapest neighbor
    g.addEdge(Edge(0, 1, 1.0));  // 0 chooses 1, 1 chooses 0
    g.addEdge(Edge(2, 3, 2.0));  // 2 chooses 3, 3 chooses 2
    g.addEdge(Edge(4, 5, 3.0));  // 4 chooses 5, 5 chooses 4

    // Second phase: components find cheapest connections
    g.addEdge(Edge(1, 2, 4.0));  // Connects {0,1} with {2,3}
    g.addEdge(Edge(3, 4, 5.0));  // Connects {2,3} with {4,5}

    // Add some heavier alternatives
    g.addEdge(Edge(0, 3, 10.0));
    g.addEdge(Edge(1, 5, 12.0));

    BoruvkaMST mst(g);

    auto edges = mst.edges();
    assert(edges.size() == 5);

    // Should use edges: 1+2+3+4+5 = 15
    assert(mst.weight() == 15.0);

    std::cout << "testComponentMerging passed\n";
}

void testCycleAvoidance() {
    // Test that Borůvka correctly avoids creating cycles
    EdgeWeightedGraph g(4);
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(1, 2, 2.0));
    g.addEdge(Edge(2, 3, 3.0));
    g.addEdge(Edge(3, 0, 4.0));   // This would create a cycle
    g.addEdge(Edge(0, 2, 5.0));   // This would also create a cycle

    BoruvkaMST mst(g);

    auto edges = mst.edges();
    assert(edges.size() == 3);  // Should have V-1 edges, not V
    assert(mst.weight() == 6.0);  // 1+2+3, excluding the 4.0 and 5.0 edges

    std::cout << "testCycleAvoidance passed\n";
}

void testSymmetricGraph() {
    // Test with a symmetric graph structure
    EdgeWeightedGraph g(6);

    // Create symmetric structure
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(1, 2, 2.0));
    g.addEdge(Edge(3, 4, 1.0));  // Mirror of (0,1)
    g.addEdge(Edge(4, 5, 2.0));  // Mirror of (1,2)
    g.addEdge(Edge(2, 3, 10.0)); // Connection between symmetric parts

    BoruvkaMST mst(g);

    auto edges = mst.edges();
    assert(edges.size() == 5);
    assert(mst.weight() == 16.0);  // 1+2+1+2+10

    std::cout << "testSymmetricGraph passed\n";
}

void testLargeWeights() {
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 1000000.0));
    g.addEdge(Edge(1, 2, 2000000.0));
    g.addEdge(Edge(0, 2, 3000000.0));

    BoruvkaMST mst(g);

    auto edges = mst.edges();
    assert(edges.size() == 2);
    assert(mst.weight() == 3000000.0);  // Should use the two smaller edges

    std::cout << "testLargeWeights passed\n";
}

void testMinimumOutgoingEdge() {
    // Test that each component correctly finds its minimum outgoing edge
    EdgeWeightedGraph g(6);

    // Component 1: {0, 1}
    g.addEdge(Edge(0, 1, 1.0));

    // Component 2: {2, 3}
    g.addEdge(Edge(2, 3, 2.0));

    // Component 3: {4, 5}
    g.addEdge(Edge(4, 5, 3.0));

    // Cross-component edges (some are traps - heavier than alternatives)
    g.addEdge(Edge(1, 2, 4.0));   // Should be selected (cheapest from {0,1} to others)
    g.addEdge(Edge(0, 2, 10.0));  // Should not be selected (heavier alternative)
    g.addEdge(Edge(3, 4, 5.0));   // Should be selected (cheapest from {2,3} to others)
    g.addEdge(Edge(2, 4, 15.0));  // Should not be selected (heavier alternative)

    BoruvkaMST mst(g);

    auto edges = mst.edges();
    assert(edges.size() == 5);

    // Should select: 1+2+3+4+5 = 15
    assert(mst.weight() == 15.0);

    // Verify heavy edges are not included
    std::set<double> weights;
    for (const Edge& e : edges) {
        weights.insert(e.weight());
    }
    assert(weights.count(10.0) == 0);
    assert(weights.count(15.0) == 0);

    std::cout << "testMinimumOutgoingEdge passed\n";
}

int main() {
    std::cout << "Running BoruvkaMST tests...\n";

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
    testBoruvkaIterations();
    testLargerGraph();
    testEqualWeights();
    testFloatingPointPrecision();
    testMSTProperties();
    testRandomGraph();
    testEmptyGraph();
    testComplexDisconnectedGraph();
    testComponentMerging();
    testCycleAvoidance();
    testSymmetricGraph();
    testLargeWeights();
    testMinimumOutgoingEdge();

    std::cout << "All tests passed!\n";
    return 0;
}