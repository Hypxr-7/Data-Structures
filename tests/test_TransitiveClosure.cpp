#include <cassert>
#include <iostream>

#include "../data_structures/Digraph.hpp"
#include "../data_structures/TransitiveClosure.hpp"

void testSimpleDAG() {
    // Create simple DAG: 0->1->2, 0->2
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(0, 2);

    TransitiveClosure tc(g);

    // Direct edges should be reachable
    assert(tc.reachable(0, 1) == true);
    assert(tc.reachable(1, 2) == true);
    assert(tc.reachable(0, 2) == true);

    // Transitive reachability: 0 can reach 2 via 1
    assert(tc.reachable(0, 2) == true);

    // Reverse directions should not be reachable
    assert(tc.reachable(1, 0) == false);
    assert(tc.reachable(2, 1) == false);
    assert(tc.reachable(2, 0) == false);

    // Self-reachability (no self-loops)
    assert(tc.reachable(0, 0) == true);  // Every vertex reaches itself
    assert(tc.reachable(1, 1) == true);
    assert(tc.reachable(2, 2) == true);

    std::cout << "testSimpleDAG passed\n";
}

void testLinearChain() {
    // Create linear chain: 0->1->2->3->4
    Digraph g(5);
    for (int i = 0; i < 4; i++) {
        g.addEdge(i, i + 1);
    }

    TransitiveClosure tc(g);

    // Test transitive reachability along chain
    for (int i = 0; i < 5; i++) {
        for (int j = i; j < 5; j++) {
            assert(tc.reachable(i, j) == true);  // Can reach forward
        }
        for (int j = 0; j < i; j++) {
            assert(tc.reachable(i, j) == false); // Cannot reach backward
        }
    }

    // Specific cases
    assert(tc.reachable(0, 4) == true);  // 0 can reach end
    assert(tc.reachable(4, 0) == false); // End cannot reach start
    assert(tc.reachable(2, 4) == true);  // Middle can reach end
    assert(tc.reachable(4, 2) == false); // End cannot reach middle

    std::cout << "testLinearChain passed\n";
}

void testDirectedCycle() {
    // Create directed cycle: 0->1->2->0
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    TransitiveClosure tc(g);

    // In a cycle, every vertex can reach every other vertex
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            assert(tc.reachable(i, j) == true);
        }
    }

    std::cout << "testDirectedCycle passed\n";
}

void testDisconnectedComponents() {
    // Create disconnected components: 0->1 and 2->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(2, 3);

    TransitiveClosure tc(g);

    // Within component 1: 0->1
    assert(tc.reachable(0, 1) == true);
    assert(tc.reachable(1, 0) == false);

    // Within component 2: 2->3
    assert(tc.reachable(2, 3) == true);
    assert(tc.reachable(3, 2) == false);

    // Between components: no reachability
    assert(tc.reachable(0, 2) == false);
    assert(tc.reachable(0, 3) == false);
    assert(tc.reachable(1, 2) == false);
    assert(tc.reachable(1, 3) == false);
    assert(tc.reachable(2, 0) == false);
    assert(tc.reachable(2, 1) == false);
    assert(tc.reachable(3, 0) == false);
    assert(tc.reachable(3, 1) == false);

    // Self-reachability
    for (int i = 0; i < 4; i++) {
        assert(tc.reachable(i, i) == true);
    }

    std::cout << "testDisconnectedComponents passed\n";
}

void testTreeStructure() {
    // Create tree: 0->1, 0->2, 1->3, 1->4, 2->5
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    TransitiveClosure tc(g);

    // Root can reach all vertices
    for (int i = 1; i < 6; i++) {
        assert(tc.reachable(0, i) == true);
    }

    // Subtree reachability
    assert(tc.reachable(1, 3) == true);
    assert(tc.reachable(1, 4) == true);
    assert(tc.reachable(2, 5) == true);

    // Leaves cannot reach anything except themselves
    assert(tc.reachable(3, 0) == false);
    assert(tc.reachable(3, 1) == false);
    assert(tc.reachable(4, 1) == false);
    assert(tc.reachable(5, 2) == false);

    // Cross-subtree: no reachability
    assert(tc.reachable(1, 5) == false);
    assert(tc.reachable(2, 3) == false);
    assert(tc.reachable(3, 5) == false);

    std::cout << "testTreeStructure passed\n";
}

void testComplexDAG() {
    // Complex DAG with multiple paths: 0->1, 0->2, 1->3, 2->3, 3->4
    Digraph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(3, 4);

    TransitiveClosure tc(g);

    // Source vertex can reach all
    for (int i = 1; i < 5; i++) {
        assert(tc.reachable(0, i) == true);
    }

    // Multiple paths to vertex 3
    assert(tc.reachable(0, 3) == true);  // via 1 or 2
    assert(tc.reachable(1, 3) == true);  // direct
    assert(tc.reachable(2, 3) == true);  // direct

    // Paths to sink vertex 4
    assert(tc.reachable(0, 4) == true);  // 0->1->3->4 or 0->2->3->4
    assert(tc.reachable(1, 4) == true);  // 1->3->4
    assert(tc.reachable(2, 4) == true);  // 2->3->4
    assert(tc.reachable(3, 4) == true);  // 3->4

    // Backward reachability should be false
    assert(tc.reachable(4, 0) == false);
    assert(tc.reachable(3, 1) == false);
    assert(tc.reachable(3, 2) == false);

    std::cout << "testComplexDAG passed\n";
}

void testSelfLoops() {
    // Graph with self-loops: 0->0, 1->2, 2->2
    Digraph g(3);
    g.addEdge(0, 0);  // Self-loop
    g.addEdge(1, 2);
    g.addEdge(2, 2);  // Self-loop

    TransitiveClosure tc(g);

    // Self-loops ensure self-reachability
    assert(tc.reachable(0, 0) == true);
    assert(tc.reachable(1, 1) == true);
    assert(tc.reachable(2, 2) == true);

    // Other reachability
    assert(tc.reachable(1, 2) == true);
    assert(tc.reachable(2, 1) == false);
    assert(tc.reachable(0, 1) == false);
    assert(tc.reachable(0, 2) == false);

    std::cout << "testSelfLoops passed\n";
}

void testStronglyConnectedComponent() {
    // Strongly connected: 0->1->2->0, plus 3->0
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);  // Creates SCC {0,1,2}
    g.addEdge(3, 0);  // 3 can reach SCC but not vice versa

    TransitiveClosure tc(g);

    // Within SCC: all can reach all
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            assert(tc.reachable(i, j) == true);
        }
    }

    // From vertex 3: can reach all (via 3->0->SCC)
    for (int i = 0; i < 3; i++) {
        assert(tc.reachable(3, i) == true);
    }

    // To vertex 3: none can reach it
    for (int i = 0; i < 3; i++) {
        assert(tc.reachable(i, 3) == false);
    }

    assert(tc.reachable(3, 3) == true);  // Self-reachable

    std::cout << "testStronglyConnectedComponent passed\n";
}

void testEmptyDigraph() {
    // Graph with vertices but no edges
    Digraph g(4);

    TransitiveClosure tc(g);

    // Only self-reachability should exist
    for (int i = 0; i < 4; i++) {
        assert(tc.reachable(i, i) == true);
        for (int j = 0; j < 4; j++) {
            if (i != j) {
                assert(tc.reachable(i, j) == false);
            }
        }
    }

    std::cout << "testEmptyDigraph passed\n";
}

void testSingleVertex() {
    // Single vertex
    Digraph g(1);

    TransitiveClosure tc(g);

    assert(tc.reachable(0, 0) == true);

    std::cout << "testSingleVertex passed\n";
}

void testCompleteDigraph() {
    // Complete digraph: every vertex connects to every other
    Digraph g(4);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i != j) {
                g.addEdge(i, j);
            }
        }
    }

    TransitiveClosure tc(g);

    // Every vertex should reach every vertex
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            assert(tc.reachable(i, j) == true);
        }
    }

    std::cout << "testCompleteDigraph passed\n";
}

void testLargeChain() {
    // Large chain to test scalability
    const int n = 100;
    Digraph g(n);
    for (int i = 0; i < n - 1; i++) {
        g.addEdge(i, i + 1);
    }

    TransitiveClosure tc(g);

    // Test some reachability properties
    assert(tc.reachable(0, n - 1) == true);    // First to last
    assert(tc.reachable(n - 1, 0) == false);   // Last to first
    assert(tc.reachable(n / 2, n - 1) == true); // Middle to last
    assert(tc.reachable(n - 1, n / 2) == false); // Last to middle

    // Test a few random pairs
    assert(tc.reachable(10, 50) == true);
    assert(tc.reachable(50, 10) == false);

    std::cout << "testLargeChain passed\n";
}

void testInvalidVertex() {
    Digraph g(3);
    g.addEdge(0, 1);

    TransitiveClosure tc(g);

    // Test invalid vertex indices
    try {
        tc.reachable(-1, 0);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        tc.reachable(0, 3);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        tc.reachable(3, 0);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "testInvalidVertex passed\n";
}

void testDiamondPattern() {
    // Diamond pattern: 0->1, 0->2, 1->3, 2->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);

    TransitiveClosure tc(g);

    // Source can reach all
    assert(tc.reachable(0, 1) == true);
    assert(tc.reachable(0, 2) == true);
    assert(tc.reachable(0, 3) == true);

    // Multiple paths to sink
    assert(tc.reachable(1, 3) == true);
    assert(tc.reachable(2, 3) == true);

    // No cross-edges between parallel paths
    assert(tc.reachable(1, 2) == false);
    assert(tc.reachable(2, 1) == false);

    // No backward reachability
    assert(tc.reachable(3, 0) == false);
    assert(tc.reachable(3, 1) == false);
    assert(tc.reachable(3, 2) == false);

    std::cout << "testDiamondPattern passed\n";
}

int main() {
    std::cout << "Running TransitiveClosure tests...\n";

    testSimpleDAG();
    testLinearChain();
    testDirectedCycle();
    testDisconnectedComponents();
    testTreeStructure();
    testComplexDAG();
    testSelfLoops();
    testStronglyConnectedComponent();
    testEmptyDigraph();
    testSingleVertex();
    testCompleteDigraph();
    testLargeChain();
    testInvalidVertex();
    testDiamondPattern();

    std::cout << "All tests passed!\n";
    return 0;
}