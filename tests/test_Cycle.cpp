#include <cassert>
#include <iostream>

#include "../data_structures/Cycle.hpp"
#include "../data_structures/Graph.hpp"

void testNoCycle() {
    // Create tree: 0-1-2-3 (no cycle)
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    Cycle cycle(g);

    assert(cycle.hasCycle() == false);
    assert(cycle.cycle().empty());

    std::cout << "testNoCycle passed\n";
}

void testSimpleCycle() {
    // Create triangle: 0-1-2-0
    Graph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    Cycle cycle(g);

    assert(cycle.hasCycle() == true);

    std::vector<int> cycleVertices = cycle.cycle();
    assert(!cycleVertices.empty());
    assert(cycleVertices.size() >= 3);  // Triangle has at least 3 vertices

    std::cout << "testSimpleCycle passed\n";
}

void testSquareCycle() {
    // Create square: 0-1-2-3-0
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 0);

    Cycle cycle(g);

    assert(cycle.hasCycle() == true);

    std::vector<int> cycleVertices = cycle.cycle();
    assert(!cycleVertices.empty());
    assert(cycleVertices.size() >= 4);  // Square has at least 4 vertices

    std::cout << "testSquareCycle passed\n";
}

void testSelfLoop() {
    // Create self-loop: vertex 0 connected to itself
    Graph g(2);
    g.addEdge(0, 0);  // Self-loop
    g.addEdge(0, 1);

    Cycle cycle(g);

    assert(cycle.hasCycle() == true);

    std::vector<int> cycleVertices = cycle.cycle();
    assert(!cycleVertices.empty());
    assert(cycleVertices.size() == 2);  // Self-loop represented as [v, v]
    assert(cycleVertices[0] == cycleVertices[1]);  // Same vertex twice

    std::cout << "testSelfLoop passed\n";
}

void testParallelEdges() {
    // Create parallel edges between vertices 0 and 1
    Graph g(3);
    g.addEdge(0, 1);
    g.addEdge(0, 1);  // Parallel edge
    g.addEdge(1, 2);

    Cycle cycle(g);

    assert(cycle.hasCycle() == true);

    std::vector<int> cycleVertices = cycle.cycle();
    assert(!cycleVertices.empty());
    assert(cycleVertices.size() == 3);  // Parallel edge: [u, v, u]

    std::cout << "testParallelEdges passed\n";
}

void testSingleVertex() {
    // Single vertex with no edges
    Graph g(1);

    Cycle cycle(g);

    assert(cycle.hasCycle() == false);
    assert(cycle.cycle().empty());

    std::cout << "testSingleVertex passed\n";
}

void testDisconnectedNoCycle() {
    // Two disconnected trees: 0-1 and 2-3
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(2, 3);

    Cycle cycle(g);

    assert(cycle.hasCycle() == false);
    assert(cycle.cycle().empty());

    std::cout << "testDisconnectedNoCycle passed\n";
}

void testDisconnectedWithCycle() {
    // Disconnected: tree 0-1 and triangle 2-3-4-2
    Graph g(5);
    g.addEdge(0, 1);    // Tree component
    g.addEdge(2, 3);    // Triangle component (has cycle)
    g.addEdge(3, 4);
    g.addEdge(4, 2);

    Cycle cycle(g);

    assert(cycle.hasCycle() == true);

    std::vector<int> cycleVertices = cycle.cycle();
    assert(!cycleVertices.empty());
    // Cycle should be from the triangle component

    std::cout << "testDisconnectedWithCycle passed\n";
}

void testStar() {
    // Star graph: center 0 connected to 1,2,3 (no cycle)
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);

    Cycle cycle(g);

    assert(cycle.hasCycle() == false);
    assert(cycle.cycle().empty());

    std::cout << "testStar passed\n";
}

void testCompleteGraph() {
    // Complete graph K4 (has many cycles)
    Graph g(4);
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            g.addEdge(i, j);
        }
    }

    Cycle cycle(g);

    assert(cycle.hasCycle() == true);
    assert(!cycle.cycle().empty());

    std::cout << "testCompleteGraph passed\n";
}

void testLargeCycle() {
    // Create large cycle: 0-1-2-3-4-5-0
    Graph g(6);
    for (int i = 0; i < 5; i++) {
        g.addEdge(i, i + 1);
    }
    g.addEdge(5, 0);  // Complete the cycle

    Cycle cycle(g);

    assert(cycle.hasCycle() == true);

    std::vector<int> cycleVertices = cycle.cycle();
    assert(!cycleVertices.empty());
    assert(cycleVertices.size() >= 6);  // Should detect the 6-cycle

    std::cout << "testLargeCycle passed\n";
}

void testEmptyGraph() {
    // Graph with vertices but no edges
    Graph g(3);

    Cycle cycle(g);

    assert(cycle.hasCycle() == false);
    assert(cycle.cycle().empty());

    std::cout << "testEmptyGraph passed\n";
}

int main() {
    std::cout << "Running Cycle tests...\n";

    testNoCycle();
    testSimpleCycle();
    testSquareCycle();
    testSelfLoop();
    testParallelEdges();
    testSingleVertex();
    testDisconnectedNoCycle();
    testDisconnectedWithCycle();
    testStar();
    testCompleteGraph();
    testLargeCycle();
    testEmptyGraph();

    std::cout << "All tests passed!\n";
    return 0;
}