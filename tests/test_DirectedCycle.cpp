#include <cassert>
#include <iostream>

#include "../data_structures/Digraph.hpp"
#include "../data_structures/DirectedCycle.hpp"

void testNoCycle() {
    // Create DAG (directed acyclic graph): 0->1->2->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    DirectedCycle dc(g);

    assert(dc.hasCycle() == false);
    assert(dc.cycle().empty());

    std::cout << "testNoCycle passed\n";
}

void testSimpleDirectedCycle() {
    // Create simple directed cycle: 0->1->2->0
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    DirectedCycle dc(g);

    assert(dc.hasCycle() == true);

    std::vector<int> cycle = dc.cycle();
    assert(!cycle.empty());
    assert(cycle.size() >= 3);  // At least 3 vertices in cycle
    assert(cycle.front() == cycle.back());  // First and last should be same

    std::cout << "testSimpleDirectedCycle passed\n";
}

void testSelfLoop() {
    // Create self-loop: 0->0
    Digraph g(2);
    g.addEdge(0, 0);  // Self-loop
    g.addEdge(0, 1);

    DirectedCycle dc(g);

    assert(dc.hasCycle() == true);

    std::vector<int> cycle = dc.cycle();
    assert(!cycle.empty());
    assert(cycle.front() == cycle.back());  // Forms a cycle

    std::cout << "testSelfLoop passed\n";
}

void testDAGTree() {
    // Create directed tree (DAG): 0->1, 0->2, 1->3, 2->4
    Digraph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);

    DirectedCycle dc(g);

    assert(dc.hasCycle() == false);
    assert(dc.cycle().empty());

    std::cout << "testDAGTree passed\n";
}

void testComplexCycle() {
    // Create more complex cycle: 0->1->2->3->1 (cycle involves 1,2,3)
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 1);  // Back to 1, creating cycle

    DirectedCycle dc(g);

    assert(dc.hasCycle() == true);

    std::vector<int> cycle = dc.cycle();
    assert(!cycle.empty());
    assert(cycle.front() == cycle.back());

    std::cout << "testComplexCycle passed\n";
}

void testDisconnectedWithCycle() {
    // Disconnected graph: 0->1 (no cycle) and 2->3->2 (cycle)
    Digraph g(4);
    g.addEdge(0, 1);    // Component 1: no cycle
    g.addEdge(2, 3);    // Component 2: has cycle
    g.addEdge(3, 2);

    DirectedCycle dc(g);

    assert(dc.hasCycle() == true);  // Should find cycle in second component

    std::vector<int> cycle = dc.cycle();
    assert(!cycle.empty());
    assert(cycle.front() == cycle.back());

    std::cout << "testDisconnectedWithCycle passed\n";
}

void testDisconnectedNoCycle() {
    // Disconnected DAGs: 0->1->2 and 3->4
    Digraph g(5);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(3, 4);

    DirectedCycle dc(g);

    assert(dc.hasCycle() == false);
    assert(dc.cycle().empty());

    std::cout << "testDisconnectedNoCycle passed\n";
}

void testDirectionMatters() {
    // Show that direction matters: 0->1->2, 2->0 vs undirected would have cycle
    Digraph g1(3);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(2, 0);  // Directed cycle

    DirectedCycle dc1(g1);
    assert(dc1.hasCycle() == true);

    // Same vertices but no cycle due to direction
    Digraph g2(3);
    g2.addEdge(0, 1);
    g2.addEdge(1, 2);
    g2.addEdge(0, 2);  // No back edge, just DAG

    DirectedCycle dc2(g2);
    assert(dc2.hasCycle() == false);

    std::cout << "testDirectionMatters passed\n";
}

void testLargeCycle() {
    // Create large directed cycle: 0->1->2->3->4->0
    Digraph g(5);
    for (int i = 0; i < 4; i++) {
        g.addEdge(i, i + 1);
    }
    g.addEdge(4, 0);  // Complete the cycle

    DirectedCycle dc(g);

    assert(dc.hasCycle() == true);

    std::vector<int> cycle = dc.cycle();
    assert(!cycle.empty());
    assert(cycle.size() >= 5);  // Should contain all 5 vertices plus repetition
    assert(cycle.front() == cycle.back());

    std::cout << "testLargeCycle passed\n";
}

void testMultipleCycles() {
    // Graph with multiple cycles - should find one
    Digraph g(6);
    // Cycle 1: 0->1->0
    g.addEdge(0, 1);
    g.addEdge(1, 0);
    // Cycle 2: 2->3->4->2
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 2);
    // Connection between components
    g.addEdge(1, 2);

    DirectedCycle dc(g);

    assert(dc.hasCycle() == true);  // Should find at least one cycle

    std::vector<int> cycle = dc.cycle();
    assert(!cycle.empty());
    assert(cycle.front() == cycle.back());

    std::cout << "testMultipleCycles passed\n";
}

void testSingleVertex() {
    // Single vertex with no edges
    Digraph g(1);

    DirectedCycle dc(g);

    assert(dc.hasCycle() == false);
    assert(dc.cycle().empty());

    std::cout << "testSingleVertex passed\n";
}

void testEmptyDigraph() {
    // Multiple vertices but no edges
    Digraph g(3);

    DirectedCycle dc(g);

    assert(dc.hasCycle() == false);
    assert(dc.cycle().empty());

    std::cout << "testEmptyDigraph passed\n";
}

void testCompleteDAG() {
    // Complete DAG: all edges go from lower to higher numbered vertices
    Digraph g(4);
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            g.addEdge(i, j);
        }
    }

    DirectedCycle dc(g);

    assert(dc.hasCycle() == false);  // No back edges possible
    assert(dc.cycle().empty());

    std::cout << "testCompleteDAG passed\n";
}

void testTournament() {
    // Tournament with cycle: 0->1->2->0
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    DirectedCycle dc(g);

    assert(dc.hasCycle() == true);

    std::cout << "testTournament passed\n";
}

int main() {
    std::cout << "Running DirectedCycle tests...\n";

    testNoCycle();
    testSimpleDirectedCycle();
    testSelfLoop();
    testDAGTree();
    testComplexCycle();
    testDisconnectedWithCycle();
    testDisconnectedNoCycle();
    testDirectionMatters();
    testLargeCycle();
    testMultipleCycles();
    testSingleVertex();
    testEmptyDigraph();
    testCompleteDAG();
    testTournament();

    std::cout << "All tests passed!\n";
    return 0;
}