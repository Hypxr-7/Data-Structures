#include <cassert>
#include <iostream>

#include "../data_structures/Digraph.hpp"
#include "../data_structures/DirectedEulerianCycle.hpp"


void testSimpleDirectedCycle() {
    // Create simple directed cycle: 0->1->2->0
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    DirectedEulerianCycle dec(g);

    assert(dec.hasEulerianCycle() == true);

    std::vector<int> cycle = dec.cycle();
    assert(cycle.size() == 4);  // 3 edges + 1 (return to start)
    assert(cycle.front() == cycle.back());  // Forms a cycle

    std::cout << "testSimpleDirectedCycle passed\n";
}

void testSelfLoop() {
    // Single vertex with self-loop: 0->0
    Digraph g(1);
    g.addEdge(0, 0);

    DirectedEulerianCycle dec(g);

    assert(dec.hasEulerianCycle() == true);

    std::vector<int> cycle = dec.cycle();
    assert(cycle.size() == 2);  // 1 edge + 1
    assert(cycle[0] == 0);
    assert(cycle[1] == 0);

    std::cout << "testSelfLoop passed\n";
}

void testNoEulerianCycleUnequalDegrees() {
    // Directed path: 0->1->2 (no cycle, unequal in/out degrees)
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);

    DirectedEulerianCycle dec(g);

    // Vertex 0: outdegree=1, indegree=0
    // Vertex 2: outdegree=0, indegree=1
    assert(dec.hasEulerianCycle() == false);
    assert(dec.cycle().empty());

    std::cout << "testNoEulerianCycleUnequalDegrees passed\n";
}

void testComplexDirectedCycle() {
    // More complex cycle: 0->1->2->3->0, with additional edges
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 0);
    g.addEdge(0, 2);  // Additional edge
    g.addEdge(2, 0);  // Return edge to balance degrees

    DirectedEulerianCycle dec(g);

    assert(dec.hasEulerianCycle() == true);

    std::vector<int> cycle = dec.cycle();
    assert(cycle.size() == 7);  // 6 edges + 1
    assert(cycle.front() == cycle.back());

    std::cout << "testComplexDirectedCycle passed\n";
}

void testDisconnectedDigraph() {
    // Two disconnected cycles: 0->1->0 and 2->3->2
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 2);

    DirectedEulerianCycle dec(g);

    // Even though each component has Eulerian cycle,
    // graph is disconnected so no overall Eulerian cycle
    assert(dec.hasEulerianCycle() == false);
    assert(dec.cycle().empty());

    std::cout << "testDisconnectedDigraph passed\n";
}

void testConnectedEulerianDigraph() {
    // Connected digraph with Eulerian cycle
    // 0->1->2->0, 0->3->0 (connected via vertex 0)
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(0, 3);
    g.addEdge(3, 0);

    DirectedEulerianCycle dec(g);

    assert(dec.hasEulerianCycle() == true);

    std::vector<int> cycle = dec.cycle();
    assert(cycle.size() == 6);  // 5 edges + 1
    assert(cycle.front() == cycle.back());

    std::cout << "testConnectedEulerianDigraph passed\n";
}

void testEmptyDigraph() {
    // Digraph with vertices but no edges
    Digraph g(3);

    DirectedEulerianCycle dec(g);

    assert(dec.hasEulerianCycle() == false);
    assert(dec.cycle().empty());

    std::cout << "testEmptyDigraph passed\n";
}

void testSingleVertex() {
    // Single vertex with no edges
    Digraph g(1);

    DirectedEulerianCycle dec(g);

    assert(dec.hasEulerianCycle() == false);
    assert(dec.cycle().empty());

    std::cout << "testSingleVertex passed\n";
}

void testMultipleSelfLoops() {
    // Vertex with multiple self-loops: 0->0, 0->0
    Digraph g(1);
    g.addEdge(0, 0);
    g.addEdge(0, 0);

    DirectedEulerianCycle dec(g);

    assert(dec.hasEulerianCycle() == true);

    std::vector<int> cycle = dec.cycle();
    assert(cycle.size() == 3);  // 2 edges + 1
    assert(cycle[0] == 0);
    assert(cycle[1] == 0);
    assert(cycle[2] == 0);

    std::cout << "testMultipleSelfLoops passed\n";
}

void testStronglyConnectedDigraph() {
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 0);
    g.addEdge(1, 2);
    g.addEdge(2, 1);
    g.addEdge(2, 0);
    g.addEdge(0, 2);

    DirectedEulerianCycle dec(g);

    assert(dec.hasEulerianCycle() == true);

    std::vector<int> cycle = dec.cycle();
    assert(cycle.size() == 7);  // 6 edges + 1
    assert(cycle.front() == cycle.back());

    std::cout << "testStronglyConnectedDigraph passed\n";
}

void testDirectionMatters() {
    // Show direction matters: same edges as undirected Eulerian graph
    // but different directions
    Digraph g1(3);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(0, 2);  // No return edges - not Eulerian

    DirectedEulerianCycle dec1(g1);
    assert(dec1.hasEulerianCycle() == false);

    // Add return edges to make it Eulerian
    Digraph g2(3);
    g2.addEdge(0, 1);
    g2.addEdge(1, 0);  // Bidirectional
    g2.addEdge(1, 2);
    g2.addEdge(2, 1);  // Bidirectional
    g2.addEdge(0, 2);
    g2.addEdge(2, 0);  // Bidirectional

    DirectedEulerianCycle dec2(g2);
    assert(dec2.hasEulerianCycle() == true);

    std::cout << "testDirectionMatters passed\n";
}

void testLargeDirectedCycle() {
    // Large directed cycle: 0->1->2->3->4->5->0
    Digraph g(6);
    for (int i = 0; i < 5; i++) {
        g.addEdge(i, i + 1);
    }
    g.addEdge(5, 0);  // Complete the cycle

    DirectedEulerianCycle dec(g);

    assert(dec.hasEulerianCycle() == true);

    std::vector<int> cycle = dec.cycle();
    assert(cycle.size() == 7);  // 6 edges + 1
    assert(cycle.front() == cycle.back());

    std::cout << "testLargeDirectedCycle passed\n";
}

void testTournamentNoEulerian() {
    // Tournament (complete directed graph) - typically no Eulerian cycle
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);  // This is actually Eulerian!

    DirectedEulerianCycle dec(g);
    assert(dec.hasEulerianCycle() == true);

    // Try non-Eulerian tournament
    Digraph g2(3);
    g2.addEdge(0, 1);
    g2.addEdge(0, 2);
    g2.addEdge(1, 2);  // Vertex 0: out=2,in=0; Vertex 2: out=0,in=2

    DirectedEulerianCycle dec2(g2);
    assert(dec2.hasEulerianCycle() == false);

    std::cout << "testTournamentNoEulerian passed\n";
}


void testParallelEdges() {
    // Multiple edges between same vertices
    Digraph g(2);
    g.addEdge(0, 1);
    g.addEdge(1, 0);
    g.addEdge(0, 1);  // Parallel edge
    g.addEdge(1, 0);  // Parallel edge

    DirectedEulerianCycle dec(g);

    assert(dec.hasEulerianCycle() == true);

    std::vector<int> cycle = dec.cycle();
    assert(cycle.size() == 5);  // 4 edges + 1
    assert(cycle.front() == cycle.back());

    std::cout << "testParallelEdges passed\n";
}

int main() {
    std::cout << "Running DirectedEulerianCycle tests...\n";

    testSimpleDirectedCycle();
    testSelfLoop();
    testNoEulerianCycleUnequalDegrees();
    testComplexDirectedCycle();
    testDisconnectedDigraph();
    testConnectedEulerianDigraph();
    testEmptyDigraph();
    testSingleVertex();
    testMultipleSelfLoops();
    testStronglyConnectedDigraph();
    testDirectionMatters();
    testLargeDirectedCycle();
    testTournamentNoEulerian();
    testParallelEdges();

    std::cout << "All tests passed!\n";
    return 0;
}