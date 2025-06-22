#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "../data_structures/DirectedEdge.hpp"

void testBasicConstruction() {
    // Test normal construction
    DirectedEdge edge(0, 1, 5.0);

    assert(edge.from() == 0);
    assert(edge.to() == 1);
    assert(edge.weight() == 5.0);

    std::cout << "testBasicConstruction passed\n";
}

void testSelfLoop() {
    // Test self-loop (edge from vertex to itself)
    DirectedEdge selfLoop(3, 3, 2.5);

    assert(selfLoop.from() == 3);
    assert(selfLoop.to() == 3);
    assert(selfLoop.weight() == 2.5);

    std::cout << "testSelfLoop passed\n";
}

void testNegativeWeight() {
    // Test edge with negative weight
    DirectedEdge negativeEdge(1, 2, -10.5);

    assert(negativeEdge.from() == 1);
    assert(negativeEdge.to() == 2);
    assert(negativeEdge.weight() == -10.5);

    std::cout << "testNegativeWeight passed\n";
}

void testZeroWeight() {
    // Test edge with zero weight
    DirectedEdge zeroEdge(0, 5, 0.0);

    assert(zeroEdge.from() == 0);
    assert(zeroEdge.to() == 5);
    assert(zeroEdge.weight() == 0.0);

    std::cout << "testZeroWeight passed\n";
}

void testLargeVertexNumbers() {
    // Test with large vertex numbers
    DirectedEdge largeEdge(1000, 2000, 15.7);

    assert(largeEdge.from() == 1000);
    assert(largeEdge.to() == 2000);
    assert(largeEdge.weight() == 15.7);

    std::cout << "testLargeVertexNumbers passed\n";
}

void testInvalidVertices() {
    // Test negative source vertex
    try {
        DirectedEdge invalidSource(-1, 5, 3.0);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::string errorMsg = e.what();
        assert(errorMsg.find("Source vertex must be non-negative") != std::string::npos);
        assert(errorMsg.find("-1") != std::string::npos);
    }

    // Test negative destination vertex
    try {
        DirectedEdge invalidDest(5, -2, 3.0);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::string errorMsg = e.what();
        assert(errorMsg.find("Destination vertex must be non-negative") != std::string::npos);
        assert(errorMsg.find("-2") != std::string::npos);
    }

    // Test both vertices negative
    try {
        DirectedEdge bothInvalid(-3, -4, 1.0);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::string errorMsg = e.what();
        assert(errorMsg.find("Source vertex must be non-negative") != std::string::npos);
        assert(errorMsg.find("-3") != std::string::npos);
    }

    std::cout << "testInvalidVertices passed\n";
}

void testEqualityOperators() {
    DirectedEdge e1(0, 1, 5.0);
    DirectedEdge e2(0, 1, 5.0);  // Same as e1
    DirectedEdge e3(1, 0, 5.0);  // Different direction
    DirectedEdge e4(0, 1, 3.0);  // Different weight
    DirectedEdge e5(0, 2, 5.0);  // Different destination

    // Test equality
    assert(e1 == e2);
    assert(e2 == e1);  // Symmetric

    // Test inequality
    assert(e1 != e3);  // Different direction
    assert(e1 != e4);  // Different weight
    assert(e1 != e5);  // Different destination

    // Test reflexivity
    assert(e1 == e1);

    std::cout << "testEqualityOperators passed\n";
}

void testComparisonOperators() {
    DirectedEdge light(0, 1, 2.0);
    DirectedEdge medium(1, 2, 5.0);
    DirectedEdge heavy(2, 3, 8.0);
    DirectedEdge equal(3, 4, 5.0);  // Same weight as medium

    // Test less than
    assert(light < medium);
    assert(medium < heavy);
    assert(!(medium < equal));  // Equal weights
    assert(!(heavy < light));   // Reverse order

    // Test greater than
    assert(heavy > medium);
    assert(medium > light);
    assert(!(medium > equal));  // Equal weights
    assert(!(light > heavy));   // Reverse order

    // Test less than or equal
    assert(light <= medium);
    assert(medium <= heavy);
    assert(medium <= equal);    // Equal weights
    assert(!(heavy <= light));

    // Test greater than or equal
    assert(heavy >= medium);
    assert(medium >= light);
    assert(medium >= equal);    // Equal weights
    assert(!(light >= heavy));

    std::cout << "testComparisonOperators passed\n";
}

void testSorting() {
    std::vector<DirectedEdge> edges;
    edges.emplace_back(2, 3, 8.0);
    edges.emplace_back(0, 1, 2.0);
    edges.emplace_back(1, 2, 5.0);
    edges.emplace_back(3, 4, 1.0);
    edges.emplace_back(4, 5, 5.0);  // Same weight as edges[2]

    std::sort(edges.begin(), edges.end());

    // Should be sorted by weight: 1.0, 2.0, 5.0, 5.0, 8.0
    assert(edges[0].weight() == 1.0);
    assert(edges[1].weight() == 2.0);
    assert(edges[2].weight() == 5.0);
    assert(edges[3].weight() == 5.0);
    assert(edges[4].weight() == 8.0);

    std::cout << "testSorting passed\n";
}

void testInSet() {
    std::set<DirectedEdge> edgeSet;

    DirectedEdge e1(0, 1, 3.0);
    DirectedEdge e2(1, 2, 1.0);
    DirectedEdge e3(2, 3, 5.0);
    DirectedEdge e4(0, 1, 3.0);  // Duplicate of e1

    edgeSet.insert(e1);
    edgeSet.insert(e2);
    edgeSet.insert(e3);
    edgeSet.insert(e4);  // Should not increase set size

    // Set should contain 3 unique edges, sorted by weight
    assert(edgeSet.size() == 3);

    auto it = edgeSet.begin();
    assert(it->weight() == 1.0);  // e2
    ++it;
    assert(it->weight() == 3.0);  // e1 (or e4, they're equal)
    ++it;
    assert(it->weight() == 5.0);  // e3

    std::cout << "testInSet passed\n";
}


void testStreamOperator() {
    DirectedEdge edge(2, 7, -3.14);
    std::ostringstream oss;

    oss << edge;
    std::string output = oss.str();

    // Should contain the same information as toString()
    assert(output.find("2") != std::string::npos);
    assert(output.find("->") != std::string::npos);
    assert(output.find("7") != std::string::npos);
    assert(output.find("-3.14") != std::string::npos);

    std::cout << "testStreamOperator passed\n";
}

void testDirectionalProperty() {
    // Test that direction matters (unlike undirected edges)
    DirectedEdge forward(0, 1, 5.0);
    DirectedEdge backward(1, 0, 5.0);

    // These should be different edges even with same weight
    assert(forward != backward);
    assert(forward.from() == backward.to());
    assert(forward.to() == backward.from());
    assert(forward.weight() == backward.weight());

    std::cout << "testDirectionalProperty passed\n";
}

void testFloatingPointPrecision() {
    // Test with very small and very large weights
    DirectedEdge tiny(0, 1, 1e-10);
    DirectedEdge huge(1, 2, 1e10);
    DirectedEdge precise(2, 3, 3.141592653589793);

    assert(tiny.weight() == 1e-10);
    assert(huge.weight() == 1e10);
    assert(std::abs(precise.weight() - 3.141592653589793) < 1e-15);

    // Test comparison with small differences
    DirectedEdge e1(0, 1, 1.0000001);
    DirectedEdge e2(0, 1, 1.0000002);

    assert(e1 < e2);
    assert(e1 != e2);

    std::cout << "testFloatingPointPrecision passed\n";
}

void testCopySemantics() {
    DirectedEdge original(5, 10, 7.5);

    // Test copy constructor
    DirectedEdge copy1(original);
    assert(copy1 == original);
    assert(copy1.from() == original.from());
    assert(copy1.to() == original.to());
    assert(copy1.weight() == original.weight());

    // Test assignment operator
    DirectedEdge copy2(0, 0, 0.0);
    copy2 = original;
    assert(copy2 == original);
    assert(copy2.from() == original.from());
    assert(copy2.to() == original.to());
    assert(copy2.weight() == original.weight());

    std::cout << "testCopySemantics passed\n";
}

void testEdgeInAlgorithmContext() {
    // Test usage similar to graph algorithms
    std::vector<DirectedEdge> shortestPath;

    // Build a path: 0 -> 1 -> 2 -> 3
    shortestPath.emplace_back(0, 1, 2.0);
    shortestPath.emplace_back(1, 2, 3.0);
    shortestPath.emplace_back(2, 3, 1.0);

    // Calculate total path weight
    double totalWeight = 0.0;
    for (const auto& edge : shortestPath) {
        totalWeight += edge.weight();
    }

    assert(totalWeight == 6.0);

    // Verify path connectivity
    for (size_t i = 1; i < shortestPath.size(); ++i) {
        assert(shortestPath[i-1].to() == shortestPath[i].from());
    }

    std::cout << "testEdgeInAlgorithmContext passed\n";
}

void testMinMaxWeights() {
    // Test with extreme weight values
    DirectedEdge minWeight(0, 1, std::numeric_limits<double>::lowest());
    DirectedEdge maxWeight(1, 2, std::numeric_limits<double>::max());
    DirectedEdge infinity(2, 3, std::numeric_limits<double>::infinity());
    DirectedEdge negInfinity(3, 4, -std::numeric_limits<double>::infinity());

    assert(minWeight.weight() == std::numeric_limits<double>::lowest());
    assert(maxWeight.weight() == std::numeric_limits<double>::max());
    assert(infinity.weight() == std::numeric_limits<double>::infinity());
    assert(negInfinity.weight() == -std::numeric_limits<double>::infinity());

    // Test comparisons with extreme values
    assert(negInfinity < minWeight);
    assert(maxWeight < infinity);
    assert(minWeight < maxWeight);

    std::cout << "testMinMaxWeights passed\n";
}

void testMultipleEdgesBetweenVertices() {
    // Test multiple directed edges between same vertices (parallel edges)
    DirectedEdge edge1(0, 1, 3.0);
    DirectedEdge edge2(0, 1, 5.0);  // Different weight, same vertices
    DirectedEdge edge3(1, 0, 3.0);  // Reverse direction, same weight as edge1

    // All should be different edges
    assert(edge1 != edge2);  // Different weights
    assert(edge1 != edge3);  // Different direction
    assert(edge2 != edge3);  // Different direction and weight

    // Test in container (should store all three)
    std::vector<DirectedEdge> edges = {edge1, edge2, edge3};
    assert(edges.size() == 3);

    std::cout << "testMultipleEdgesBetweenVertices passed\n";
}

void testErrorMessageContent() {
    // Test that error messages are helpful and contain relevant information
    try {
        DirectedEdge invalidEdge(-5, 10, 2.0);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();

        // Should contain information about what went wrong
        assert(msg.find("Source vertex") != std::string::npos);
        assert(msg.find("non-negative") != std::string::npos);
        assert(msg.find("-5") != std::string::npos);
    }

    try {
        DirectedEdge invalidEdge(5, -10, 2.0);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();

        // Should contain information about what went wrong
        assert(msg.find("Destination vertex") != std::string::npos);
        assert(msg.find("non-negative") != std::string::npos);
        assert(msg.find("-10") != std::string::npos);
    }

    std::cout << "testErrorMessageContent passed\n";
}

void testPerformanceWithManyEdges() {
    // Test performance with large number of edges
    const int NUM_EDGES = 10000;
    std::vector<DirectedEdge> edges;
    edges.reserve(NUM_EDGES);

    // Create many edges
    for (int i = 0; i < NUM_EDGES; ++i) {
        edges.emplace_back(i, (i + 1) % NUM_EDGES, static_cast<double>(i));
    }

    // Sort them (this tests comparison operators under load)
    std::sort(edges.begin(), edges.end());

    // Verify they're sorted
    for (int i = 1; i < NUM_EDGES; ++i) {
        assert(edges[i-1].weight() <= edges[i].weight());
    }

    assert(edges.size() == NUM_EDGES);

    std::cout << "testPerformanceWithManyEdges passed\n";
}

int main() {
    std::cout << "Running DirectedEdge tests...\n";

    testBasicConstruction();
    testSelfLoop();
    testNegativeWeight();
    testZeroWeight();
    testLargeVertexNumbers();
    testInvalidVertices();
    testEqualityOperators();
    testComparisonOperators();
    testSorting();
    testInSet();
    testStreamOperator();
    testDirectionalProperty();
    testFloatingPointPrecision();
    testCopySemantics();
    testEdgeInAlgorithmContext();
    testMinMaxWeights();
    testMultipleEdgesBetweenVertices();
    testErrorMessageContent();
    testPerformanceWithManyEdges();

    std::cout << "All tests passed!\n";
    return 0;
}