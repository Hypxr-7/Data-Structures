
#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <vector>

#include "../data_structures/Edge.hpp"

void testBasicConstruction() {
    // Test basic edge construction
    Edge e1(0, 1, 5.0);
    Edge e2(2, 3, 10.5);
    Edge e3(4, 5, -2.3);  // Negative weight should be allowed

    // Test weight retrieval
    assert(e1.weight() == 5.0);
    assert(e2.weight() == 10.5);
    assert(e3.weight() == -2.3);

    std::cout << "testBasicConstruction passed\n";
}

void testEitherMethod() {
    Edge e(7, 3, 4.2);

    // either() should return the first vertex (v)
    assert(e.either() == 7);

    // Test with different vertex order
    Edge e2(15, 8, 1.0);
    assert(e2.either() == 15);

    std::cout << "testEitherMethod passed\n";
}

void testOtherMethod() {
    Edge e(5, 9, 3.7);

    // Test getting other vertex
    assert(e.other(5) == 9);
    assert(e.other(9) == 5);

    // Test with same vertices in different order
    Edge e2(12, 4, 2.1);
    assert(e2.other(12) == 4);
    assert(e2.other(4) == 12);

    std::cout << "testOtherMethod passed\n";
}

void testOtherMethodInvalidVertex() {
    Edge e(2, 8, 1.5);

    // Test invalid vertex - should throw exception
    try {
        e.other(5);  // 5 is not in edge (2, 8)
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& ex) {
        // Expected exception
        assert(std::string(ex.what()) == "Vertex not in edge");
    }

    try {
        e.other(-1);  // Negative vertex
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "testOtherMethodInvalidVertex passed\n";
}

void testComparisonOperators() {
    Edge e1(0, 1, 5.0);
    Edge e2(2, 3, 10.0);
    Edge e3(4, 5, 5.0);   // Same weight as e1
    Edge e4(6, 7, 3.0);

    // Test less than
    assert(e4 < e1);   // 3.0 < 5.0
    assert(e1 < e2);   // 5.0 < 10.0
    assert(!(e2 < e1)); // !(10.0 < 5.0)

    // Test greater than
    assert(e2 > e1);   // 10.0 > 5.0
    assert(e1 > e4);   // 5.0 > 3.0
    assert(!(e1 > e2)); // !(5.0 > 10.0)

    // Test less than or equal
    assert(e4 <= e1);  // 3.0 <= 5.0
    assert(e1 <= e3);  // 5.0 <= 5.0 (equal)
    assert(e1 <= e2);  // 5.0 <= 10.0

    // Test greater than or equal
    assert(e2 >= e1);  // 10.0 >= 5.0
    assert(e1 >= e3);  // 5.0 >= 5.0 (equal)
    assert(e1 >= e4);  // 5.0 >= 3.0

    // Test equality
    assert(e1 == e3);  // 5.0 == 5.0
    assert(!(e1 == e2)); // !(5.0 == 10.0)

    // Test inequality
    assert(e1 != e2);  // 5.0 != 10.0
    assert(!(e1 != e3)); // !(5.0 != 5.0)

    std::cout << "testComparisonOperators passed\n";
}

void testNegativeWeights() {
    Edge e1(1, 2, -5.5);
    Edge e2(3, 4, -10.2);
    Edge e3(5, 6, 0.0);

    // Test with negative weights
    assert(e1.weight() == -5.5);
    assert(e2.weight() == -10.2);

    // Test comparisons with negative weights
    assert(e2 < e1);   // -10.2 < -5.5
    assert(e1 < e3);   // -5.5 < 0.0
    assert(e1 > e2);   // -5.5 > -10.2

    std::cout << "testNegativeWeights passed\n";
}

void testZeroWeight() {
    Edge e1(0, 1, 0.0);
    Edge e2(2, 3, 0.0);
    Edge e3(4, 5, 1.0);

    // Test zero weights
    assert(e1.weight() == 0.0);
    assert(e1 == e2);  // Both have weight 0.0
    assert(e1 < e3);   // 0.0 < 1.0
    assert(e3 > e1);   // 1.0 > 0.0

    std::cout << "testZeroWeight passed\n";
}

void testSameVerticesDifferentWeights() {
    Edge e1(5, 8, 3.0);
    Edge e2(5, 8, 7.0);  // Same vertices, different weight

    // Should be compared by weight only
    assert(e1 < e2);
    assert(e2 > e1);
    assert(e1 != e2);

    // Vertex methods should work the same
    assert(e1.either() == e2.either());
    assert(e1.other(5) == e2.other(5));
    assert(e1.other(8) == e2.other(8));

    std::cout << "testSameVerticesDifferentWeights passed\n";
}

void testUndirectedProperty() {
    // Test that edge (v, w) behaves same as edge (w, v)
    Edge e1(3, 7, 4.5);
    Edge e2(7, 3, 4.5);  // Reversed vertices, same weight

    // Should have same weight
    assert(e1.weight() == e2.weight());
    assert(e1 == e2);  // Equal weights

    // other() method should work symmetrically
    assert(e1.other(3) == 7);
    assert(e1.other(7) == 3);
    assert(e2.other(3) == 7);
    assert(e2.other(7) == 3);

    std::cout << "testUndirectedProperty passed\n";
}

void testSortingEdges() {
    // Test sorting edges by weight (useful for Kruskal's algorithm)
    std::vector<Edge> edges;
    edges.emplace_back(0, 1, 5.0);
    edges.emplace_back(1, 2, 2.0);
    edges.emplace_back(2, 3, 8.0);
    edges.emplace_back(3, 4, 1.0);
    edges.emplace_back(4, 5, 6.0);

    // Sort edges by weight
    std::sort(edges.begin(), edges.end());

    // Check sorted order
    assert(edges[0].weight() == 1.0);  // Minimum weight
    assert(edges[1].weight() == 2.0);
    assert(edges[2].weight() == 5.0);
    assert(edges[3].weight() == 6.0);
    assert(edges[4].weight() == 8.0);  // Maximum weight

    std::cout << "testSortingEdges passed\n";
}

void testPriorityQueue() {
    // Test using edges in priority queue (for Prim's algorithm)
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> minPQ;
    std::priority_queue<Edge> maxPQ;  // Default is max-heap

    // Add edges to priority queues
    minPQ.emplace(0, 1, 5.0);
    minPQ.emplace(1, 2, 2.0);
    minPQ.emplace(2, 3, 8.0);
    minPQ.emplace(3, 4, 1.0);

    maxPQ.emplace(0, 1, 5.0);
    maxPQ.emplace(1, 2, 2.0);
    maxPQ.emplace(2, 3, 8.0);
    maxPQ.emplace(3, 4, 1.0);

    // Min-heap: smallest weight first
    assert(minPQ.top().weight() == 1.0);
    minPQ.pop();
    assert(minPQ.top().weight() == 2.0);

    // Max-heap: largest weight first
    assert(maxPQ.top().weight() == 8.0);
    maxPQ.pop();
    assert(maxPQ.top().weight() == 5.0);

    std::cout << "testPriorityQueue passed\n";
}

void testFloatingPointPrecision() {
    // Test with floating point precision issues
    Edge e1(0, 1, 0.1 + 0.2);  // Might not be exactly 0.3
    Edge e2(2, 3, 0.3);

    // This test might be sensitive to floating point precision
    // In practice, you might want epsilon-based comparison for doubles
    double diff = std::abs(e1.weight() - e2.weight());
    assert(diff < 1e-10);  // Close enough to be considered equal

    std::cout << "testFloatingPointPrecision passed\n";
}

void testLargeVertexNumbers() {
    // Test with large vertex numbers
    Edge e(999999, 1000000, 42.7);

    assert(e.either() == 999999);
    assert(e.other(999999) == 1000000);
    assert(e.other(1000000) == 999999);
    assert(e.weight() == 42.7);

    std::cout << "testLargeVertexNumbers passed\n";
}

void testSelfLoop() {
    // Test edge from vertex to itself (self-loop)
    Edge selfLoop(5, 5, 3.14);

    assert(selfLoop.either() == 5);
    assert(selfLoop.other(5) == 5);  // Other vertex is also 5
    assert(selfLoop.weight() == 3.14);

    std::cout << "testSelfLoop passed\n";
}

void testMultipleEdgesSameEndpoints() {
    // Test multiple edges between same vertices (parallel edges)
    Edge e1(2, 7, 1.5);
    Edge e2(2, 7, 2.5);
    Edge e3(2, 7, 1.5);  // Same as e1

    // Should be distinguished by weight
    assert(e1 < e2);
    assert(e1 == e3);
    assert(e2 > e3);

    // Vertex operations should be same
    assert(e1.either() == e2.either());
    assert(e1.other(2) == e2.other(2));

    std::cout << "testMultipleEdgesSameEndpoints passed\n";
}

void testImmutability() {
    // Test that edge is immutable (all members are const)
    Edge e(3, 8, 4.2);

    // These should all work (const methods)
    int v = e.either();
    int w = e.other(v);
    double weight = e.weight();

    // Edge comparison
    Edge e2(1, 2, 5.0);
    bool less = e < e2;

    // All operations preserve the edge state
    assert(e.either() == 3);
    assert(e.weight() == 4.2);

    std::cout << "testImmutability passed\n";
}

void testEdgeEquality() {
    // Test various equality scenarios
    Edge e1(1, 2, 5.0);
    Edge e2(3, 4, 5.0);  // Different vertices, same weight
    Edge e3(1, 2, 6.0);  // Same vertices, different weight
    Edge e4(2, 1, 5.0);  // Reversed vertices, same weight

    // Equality is based only on weight
    assert(e1 == e2);  // Same weight
    assert(e1 != e3);  // Different weight
    assert(e1 == e4);  // Same weight (vertex order doesn't matter for weight)

    std::cout << "testEdgeEquality passed\n";
}

int main() {
    std::cout << "Running Edge tests...\n";

    testBasicConstruction();
    testEitherMethod();
    testOtherMethod();
    testOtherMethodInvalidVertex();
    testComparisonOperators();
    testNegativeWeights();
    testZeroWeight();
    testSameVerticesDifferentWeights();
    testUndirectedProperty();
    testSortingEdges();
    testPriorityQueue();
    testFloatingPointPrecision();
    testLargeVertexNumbers();
    testSelfLoop();
    testMultipleEdgesSameEndpoints();
    testImmutability();
    testEdgeEquality();

    std::cout << "All tests passed!\n";
    return 0;
}