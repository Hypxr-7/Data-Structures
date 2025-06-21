#include <cassert>
#include <iostream>

#include "../data_structures/Digraph.hpp"
#include "../data_structures/Topological.hpp"

void testSimpleDAG() {
    // Create simple DAG: 0->1->2, 0->2
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(0, 2);

    Topological topo(g);

    assert(topo.hasOrder() == true);

    std::vector<int> order = topo.order();
    assert(order.size() == 3);

    // Check topological property: for each edge u->v, u comes before v
    // 0 should come before 1 and 2
    // 1 should come before 2
    assert(topo.rank(0) < topo.rank(1));
    assert(topo.rank(0) < topo.rank(2));
    assert(topo.rank(1) < topo.rank(2));

    // Check that ranks are consecutive starting from 0
    assert(topo.rank(order[0]) == 0);
    assert(topo.rank(order[1]) == 1);
    assert(topo.rank(order[2]) == 2);

    std::cout << "testSimpleDAG passed\n";
}

void testLinearDAG() {
    // Create linear DAG: 0->1->2->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    Topological topo(g);

    assert(topo.hasOrder() == true);

    std::vector<int> order = topo.order();
    assert(order.size() == 4);

    // In linear chain, topological order should be 0, 1, 2, 3
    assert(order[0] == 0);
    assert(order[1] == 1);
    assert(order[2] == 2);
    assert(order[3] == 3);

    // Check ranks
    for (int i = 0; i < 4; i++) {
        assert(topo.rank(i) == i);
    }

    std::cout << "testLinearDAG passed\n";
}

void testDirectedCycle() {
    // Create directed cycle: 0->1->2->0
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    Topological topo(g);

    // Graph has cycle, so no topological ordering
    assert(topo.hasOrder() == false);
    assert(topo.order().empty());

    // rank() should return -1 for all vertices when no ordering exists
    for (int v = 0; v < 3; v++) {
        assert(topo.rank(v) == -1);
    }

    std::cout << "testDirectedCycle passed\n";
}

void testTreeDAG() {
    // Create tree DAG: 0->1, 0->2, 1->3, 1->4, 2->5
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    Topological topo(g);

    assert(topo.hasOrder() == true);

    std::vector<int> order = topo.order();
    assert(order.size() == 6);

    // Check topological property for all edges
    assert(topo.rank(0) < topo.rank(1));
    assert(topo.rank(0) < topo.rank(2));
    assert(topo.rank(1) < topo.rank(3));
    assert(topo.rank(1) < topo.rank(4));
    assert(topo.rank(2) < topo.rank(5));

    // Root should be first
    assert(topo.rank(0) == 0);

    std::cout << "testTreeDAG passed\n";
}

void testDisconnectedDAG() {
    // Create disconnected DAG: 0->1->2 and 3->4
    Digraph g(5);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(3, 4);

    Topological topo(g);

    assert(topo.hasOrder() == true);

    std::vector<int> order = topo.order();
    assert(order.size() == 5);

    // Check topological property within each component
    assert(topo.rank(0) < topo.rank(1));
    assert(topo.rank(1) < topo.rank(2));
    assert(topo.rank(3) < topo.rank(4));

    // All ranks should be unique and in range [0, 4]
    std::vector<bool> used(5, false);
    for (int v = 0; v < 5; v++) {
        int r = topo.rank(v);
        assert(r >= 0 && r < 5);
        assert(!used[r]);
        used[r] = true;
    }

    std::cout << "testDisconnectedDAG passed\n";
}

void testSingleVertex() {
    // Single vertex with no edges
    Digraph g(1);

    Topological topo(g);

    assert(topo.hasOrder() == true);

    std::vector<int> order = topo.order();
    assert(order.size() == 1);
    assert(order[0] == 0);
    assert(topo.rank(0) == 0);

    std::cout << "testSingleVertex passed\n";
}

void testEmptyDAG() {
    // Multiple vertices with no edges
    Digraph g(4);

    Topological topo(g);

    assert(topo.hasOrder() == true);

    std::vector<int> order = topo.order();
    assert(order.size() == 4);

    // All vertices should have unique ranks
    std::vector<bool> used(4, false);
    for (int v = 0; v < 4; v++) {
        int r = topo.rank(v);
        assert(r >= 0 && r < 4);
        assert(!used[r]);
        used[r] = true;
    }

    std::cout << "testEmptyDAG passed\n";
}

void testComplexDAG() {
    // Complex DAG with multiple valid topological orders
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(3, 5);

    Topological topo(g);

    assert(topo.hasOrder() == true);

    std::vector<int> order = topo.order();
    assert(order.size() == 6);

    // Check all edge constraints
    assert(topo.rank(0) < topo.rank(1));
    assert(topo.rank(0) < topo.rank(2));
    assert(topo.rank(1) < topo.rank(3));
    assert(topo.rank(2) < topo.rank(3));
    assert(topo.rank(3) < topo.rank(4));
    assert(topo.rank(3) < topo.rank(5));

    // Vertex 0 should be first (no incoming edges)
    assert(topo.rank(0) == 0);

    std::cout << "testComplexDAG passed\n";
}

void testSelfLoop() {
    // Graph with self-loop: 0->0
    Digraph g(2);
    g.addEdge(0, 0);  // Self-loop creates cycle
    g.addEdge(0, 1);

    Topological topo(g);

    // Self-loop creates cycle, so no topological ordering
    assert(topo.hasOrder() == false);
    assert(topo.order().empty());

    for (int v = 0; v < 2; v++) {
        assert(topo.rank(v) == -1);
    }

    std::cout << "testSelfLoop passed\n";
}

void testLargeLinearDAG() {
    // Large linear DAG: 0->1->2->...->9
    Digraph g(10);
    for (int i = 0; i < 9; i++) {
        g.addEdge(i, i + 1);
    }

    Topological topo(g);

    assert(topo.hasOrder() == true);

    std::vector<int> order = topo.order();
    assert(order.size() == 10);

    // Should be in exact order 0, 1, 2, ..., 9
    for (int i = 0; i < 10; i++) {
        assert(order[i] == i);
        assert(topo.rank(i) == i);
    }

    std::cout << "testLargeLinearDAG passed\n";
}

void testTaskSchedulingExample() {
    // Task scheduling example: tasks with dependencies
    // Tasks: 0=compile, 1=test, 2=package, 3=deploy, 4=document
    // Dependencies: compile->test->deploy, compile->package->deploy, compile->document
    Digraph g(5);
    g.addEdge(0, 1);  // compile -> test
    g.addEdge(1, 3);  // test -> deploy
    g.addEdge(0, 2);  // compile -> package
    g.addEdge(2, 3);  // package -> deploy
    g.addEdge(0, 4);  // compile -> document

    Topological topo(g);

    assert(topo.hasOrder() == true);

    // Compile must be first
    assert(topo.rank(0) == 0);

    // Deploy must be after test and package
    assert(topo.rank(1) < topo.rank(3));
    assert(topo.rank(2) < topo.rank(3));

    // Test, package, and document must be after compile
    assert(topo.rank(0) < topo.rank(1));
    assert(topo.rank(0) < topo.rank(2));
    assert(topo.rank(0) < topo.rank(4));

    std::cout << "testTaskSchedulingExample passed\n";
}

void testDiamondDAG() {
    // Diamond-shaped DAG: 0->1, 0->2, 1->3, 2->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);

    Topological topo(g);

    assert(topo.hasOrder() == true);

    std::vector<int> order = topo.order();
    assert(order.size() == 4);

    // Check diamond constraints
    assert(topo.rank(0) < topo.rank(1));
    assert(topo.rank(0) < topo.rank(2));
    assert(topo.rank(1) < topo.rank(3));
    assert(topo.rank(2) < topo.rank(3));

    // 0 should be first, 3 should be last
    assert(topo.rank(0) == 0);
    assert(topo.rank(3) == 3);

    std::cout << "testDiamondDAG passed\n";
}

void testInvalidVertex() {
    Digraph g(3);
    g.addEdge(0, 1);

    Topological topo(g);

    // Test invalid vertex indices
    try {
        topo.rank(-1);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        topo.rank(3);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "testInvalidVertex passed\n";
}

int main() {
    std::cout << "Running Topological tests...\n";

    testSimpleDAG();
    testLinearDAG();
    testDirectedCycle();
    testTreeDAG();
    testDisconnectedDAG();
    testSingleVertex();
    testEmptyDAG();
    testComplexDAG();
    testSelfLoop();
    testLargeLinearDAG();
    testTaskSchedulingExample();
    testDiamondDAG();
    testInvalidVertex();

    std::cout << "All tests passed!\n";
    return 0;
}