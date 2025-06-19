#include <cassert>
#include <iostream>

#include "../data_structures/Bipartite.hpp"
#include "../data_structures/Graph.hpp"

void testBipartiteLinearGraph() {
    // Create linear graph: 0-1-2-3 (bipartite)
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    Bipartite bp(g);

    // Should be bipartite
    assert(bp.isBipartite() == true);

    // Check alternating colors
    assert(bp.color(0) != bp.color(1));
    assert(bp.color(1) != bp.color(2));
    assert(bp.color(2) != bp.color(3));
    assert(bp.color(0) == bp.color(2));  // Same partition
    assert(bp.color(1) == bp.color(3));  // Same partition

    // No odd cycle
    assert(bp.oddCycle().empty());

    std::cout << "testBipartiteLinearGraph passed\n";
}

void testBipartiteCompleteBipartite() {
    // Complete bipartite graph K_{2,3}: vertices {0,1} connected to {2,3,4}
    Graph g(5);
    g.addEdge(0, 2);
    g.addEdge(0, 3);
    g.addEdge(0, 4);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);

    Bipartite bp(g);

    // Should be bipartite
    assert(bp.isBipartite() == true);

    // Check partitions
    assert(bp.color(0) == bp.color(1));      // {0,1} in same partition
    assert(bp.color(2) == bp.color(3));      // {2,3,4} in same partition
    assert(bp.color(3) == bp.color(4));
    assert(bp.color(0) != bp.color(2));      // Different partitions

    // No odd cycle
    assert(bp.oddCycle().empty());

    std::cout << "testBipartiteCompleteBipartite passed\n";
}

void testNonBipartiteTriangle() {
    // Triangle: 0-1-2-0 (not bipartite, odd cycle of length 3)
    Graph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    Bipartite bp(g);

    // Should not be bipartite
    assert(bp.isBipartite() == false);

    // Should have odd cycle
    std::vector<int> cycle = bp.oddCycle();
    assert(!cycle.empty());
    assert(cycle.size() == 3);  // Triangle has 3 vertices
    assert(cycle.size() % 2 == 1);  // Odd length

    std::cout << "testNonBipartiteTriangle passed\n";
}

void testNonBipartitePentagon() {
    // Pentagon: 0-1-2-3-4-0 (not bipartite, odd cycle of length 5)
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 0);

    Bipartite bp(g);

    // Should not be bipartite
    assert(bp.isBipartite() == false);

    // Should have odd cycle
    std::vector<int> cycle = bp.oddCycle();
    assert(!cycle.empty());
    assert(cycle.size() == 5);  // Pentagon has 5 vertices
    assert(cycle.size() % 2 == 1);  // Odd length

    std::cout << "testNonBipartitePentagon passed\n";
}

void testBipartiteSquare() {
    // Square: 0-1-2-3-0 (bipartite, even cycle)
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 0);

    Bipartite bp(g);

    // Should be bipartite
    assert(bp.isBipartite() == true);

    // Check alternating colors in cycle
    assert(bp.color(0) != bp.color(1));
    assert(bp.color(1) != bp.color(2));
    assert(bp.color(2) != bp.color(3));
    assert(bp.color(3) != bp.color(0));
    assert(bp.color(0) == bp.color(2));  // Opposite vertices same color
    assert(bp.color(1) == bp.color(3));

    // No odd cycle
    assert(bp.oddCycle().empty());

    std::cout << "testBipartiteSquare passed\n";
}

void testSingleVertex() {
    // Single vertex (trivially bipartite)
    Graph g(1);
    Bipartite bp(g);

    assert(bp.isBipartite() == true);
    assert(bp.color(0) == false);  // Default color
    assert(bp.oddCycle().empty());

    std::cout << "testSingleVertex passed\n";
}

void testDisconnectedBipartite() {
    // Two disconnected edges: 0-1  2-3 (bipartite)
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(2, 3);

    Bipartite bp(g);

    // Should be bipartite
    assert(bp.isBipartite() == true);

    // Each edge should have different colors
    assert(bp.color(0) != bp.color(1));
    assert(bp.color(2) != bp.color(3));

    // No odd cycle
    assert(bp.oddCycle().empty());

    std::cout << "testDisconnectedBipartite passed\n";
}

void testDisconnectedWithOddCycle() {
    // Disconnected: bipartite component 0-1 and triangle 2-3-4-2
    Graph g(5);
    g.addEdge(0, 1);    // Bipartite component
    g.addEdge(2, 3);    // Triangle (non-bipartite)
    g.addEdge(3, 4);
    g.addEdge(4, 2);

    Bipartite bp(g);

    // Should not be bipartite (due to triangle)
    assert(bp.isBipartite() == false);

    // Should have odd cycle from triangle
    std::vector<int> cycle = bp.oddCycle();
    assert(!cycle.empty());
    assert(cycle.size() == 3);

    std::cout << "testDisconnectedWithOddCycle passed\n";
}

void testEmptyGraph() {
    // Graph with no edges (trivially bipartite)
    Graph g(3);
    Bipartite bp(g);

    assert(bp.isBipartite() == true);
    assert(bp.oddCycle().empty());

    // All vertices can have default color
    for (int i = 0; i < 3; i++) {
        assert(bp.color(i) == false);
    }

    std::cout << "testEmptyGraph passed\n";
}

void testColorException() {
    // Create non-bipartite graph
    Graph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    Bipartite bp(g);
    assert(bp.isBipartite() == false);

    // Should throw exception when asking for color of non-bipartite graph
    try {
        bp.color(0);
        assert(false);  // Should not reach here
    } catch (const std::runtime_error&) {
        // Expected exception
    }

    std::cout << "testColorException passed\n";
}

void testInvalidVertex() {
    Graph g(3);
    g.addEdge(0, 1);
    Bipartite bp(g);

    // Test invalid vertex indices
    try {
        bp.color(-1);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        bp.color(3);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "testInvalidVertex passed\n";
}

int main() {
    std::cout << "Running Bipartite tests...\n";

    testBipartiteLinearGraph();
    testBipartiteCompleteBipartite();
    testNonBipartiteTriangle();
    testNonBipartitePentagon();
    testBipartiteSquare();
    testSingleVertex();
    testDisconnectedBipartite();
    testDisconnectedWithOddCycle();
    testEmptyGraph();
    testColorException();
    testInvalidVertex();

    std::cout << "All tests passed!\n";
    return 0;
}