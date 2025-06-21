#include <cassert>
#include <iostream>
#include <set>
#include <vector>

#include "../data_structures/Digraph.hpp"
#include "../data_structures/GabowSCC.hpp"

void testSingleSCC() {
    // Create strongly connected graph: 0->1->2->0
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    GabowSCC scc(g);

    // Should have exactly 1 SCC
    assert(scc.count() == 1);

    // All vertices should be strongly connected
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            assert(scc.stronglyConnected(i, j) == true);
        }
    }

    // All vertices should have same SCC id
    int commonId = scc.id(0);
    for (int i = 1; i < 3; i++) {
        assert(scc.id(i) == commonId);
    }

    std::cout << "testSingleSCC passed\n";
}

void testMultipleSCCs() {
    // Create graph with multiple SCCs: 0->1->2->0, 2->3->4->3
    Digraph g(5);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);  // SCC: {0, 1, 2}
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 3);  // SCC: {3, 4}

    GabowSCC scc(g);

    // Should have exactly 2 SCCs
    assert(scc.count() == 2);

    // Vertices 0, 1, 2 should be strongly connected
    assert(scc.stronglyConnected(0, 1) == true);
    assert(scc.stronglyConnected(0, 2) == true);
    assert(scc.stronglyConnected(1, 2) == true);

    // Vertices 3, 4 should be strongly connected
    assert(scc.stronglyConnected(3, 4) == true);

    // Different SCCs should not be strongly connected
    assert(scc.stronglyConnected(0, 3) == false);
    assert(scc.stronglyConnected(0, 4) == false);
    assert(scc.stronglyConnected(1, 3) == false);
    assert(scc.stronglyConnected(1, 4) == false);
    assert(scc.stronglyConnected(2, 3) == false);
    assert(scc.stronglyConnected(2, 4) == false);

    // Check SCC ids
    assert(scc.id(0) == scc.id(1));
    assert(scc.id(1) == scc.id(2));
    assert(scc.id(3) == scc.id(4));
    assert(scc.id(0) != scc.id(3));

    std::cout << "testMultipleSCCs passed\n";
}

void testDAG() {
    // Create DAG: 0->1->2, 0->2 (no cycles, each vertex is its own SCC)
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(0, 2);

    GabowSCC scc(g);

    // Should have 3 SCCs (one per vertex)
    assert(scc.count() == 3);

    // No vertices should be strongly connected (except to themselves)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j) {
                assert(scc.stronglyConnected(i, j) == true);
            } else {
                assert(scc.stronglyConnected(i, j) == false);
            }
        }
    }

    // All vertices should have different SCC ids
    std::set<int> ids;
    for (int i = 0; i < 3; i++) {
        ids.insert(scc.id(i));
    }
    assert(ids.size() == 3);

    std::cout << "testDAG passed\n";
}

void testGabowTwoStackBehavior() {
    // Test case specifically designed to test Gabow's two-stack behavior
    // Complex structure: 0->1->2->3->1, 2->4, 4->5->4
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 1);  // Back edge creating SCC {1, 2, 3}
    g.addEdge(2, 4);  // Cross edge to different component
    g.addEdge(4, 5);
    g.addEdge(5, 4);  // SCC {4, 5}

    GabowSCC scc(g);

    // Should have 3 SCCs: {0}, {1,2,3}, {4,5}
    assert(scc.count() == 3);

    // Vertex 0 is its own SCC
    assert(scc.stronglyConnected(0, 1) == false);
    assert(scc.stronglyConnected(0, 2) == false);

    // Vertices 1, 2, 3 form an SCC
    assert(scc.stronglyConnected(1, 2) == true);
    assert(scc.stronglyConnected(1, 3) == true);
    assert(scc.stronglyConnected(2, 3) == true);

    // Vertices 4, 5 form an SCC
    assert(scc.stronglyConnected(4, 5) == true);

    std::cout << "testGabowTwoStackBehavior passed\n";
}

void testNestedCycles() {
    // Nested cycles that should merge: 0->1->2->0, 1->3->4->1
    Digraph g(5);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);  // Outer cycle
    g.addEdge(1, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 1);  // Inner cycle that shares vertex 1

    GabowSCC scc(g);

    // All vertices should be in one SCC due to shared vertex
    assert(scc.count() == 1);

    // All vertices should be strongly connected
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            assert(scc.stronglyConnected(i, j) == true);
        }
    }

    std::cout << "testNestedCycles passed\n";
}

void testBackEdgeStackBehavior() {
    // Test back edge handling in stack2: 0->1->2, 2->0, 1->3->4
    Digraph g(5);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);  // Back edge creates SCC {0, 1, 2}
    g.addEdge(1, 3);  // Tree edge to separate component
    g.addEdge(3, 4);  // Tree edge

    GabowSCC scc(g);

    // Should have 3 SCCs: {0,1,2}, {3}, {4}
    assert(scc.count() == 3);

    // Vertices 0, 1, 2 in same SCC
    assert(scc.stronglyConnected(0, 1) == true);
    assert(scc.stronglyConnected(0, 2) == true);
    assert(scc.stronglyConnected(1, 2) == true);

    // Vertices 3, 4 in separate SCCs
    assert(scc.stronglyConnected(3, 4) == false);
    assert(scc.stronglyConnected(0, 3) == false);
    assert(scc.stronglyConnected(0, 4) == false);

    std::cout << "testBackEdgeStackBehavior passed\n";
}

void testSelfLoops() {
    // Graph with self-loops: 0->0, 1->2->1, 3->3
    Digraph g(4);
    g.addEdge(0, 0);  // Self-loop SCC
    g.addEdge(1, 2);
    g.addEdge(2, 1);  // Cycle SCC
    g.addEdge(3, 3);  // Self-loop SCC

    GabowSCC scc(g);

    assert(scc.count() == 3);

    // Self-loops form SCCs
    assert(scc.stronglyConnected(0, 0) == true);
    assert(scc.stronglyConnected(3, 3) == true);

    // Cycle forms SCC
    assert(scc.stronglyConnected(1, 2) == true);

    // Different SCCs
    assert(scc.stronglyConnected(0, 1) == false);
    assert(scc.stronglyConnected(0, 3) == false);
    assert(scc.stronglyConnected(1, 3) == false);

    std::cout << "testSelfLoops passed\n";
}

void testDisconnectedComponents() {
    // Multiple disconnected SCCs: 0->1->0, 2->3->4->2, 5
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(1, 0);  // SCC: {0, 1}
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 2);  // SCC: {2, 3, 4}
    // Vertex 5 is isolated: SCC: {5}

    GabowSCC scc(g);

    assert(scc.count() == 3);

    // Within SCCs
    assert(scc.stronglyConnected(0, 1) == true);
    assert(scc.stronglyConnected(2, 3) == true);
    assert(scc.stronglyConnected(2, 4) == true);
    assert(scc.stronglyConnected(3, 4) == true);

    // Between SCCs
    assert(scc.stronglyConnected(0, 2) == false);
    assert(scc.stronglyConnected(0, 5) == false);
    assert(scc.stronglyConnected(2, 5) == false);

    // Isolated vertex
    assert(scc.stronglyConnected(5, 5) == true);

    std::cout << "testDisconnectedComponents passed\n";
}

void testComplexStackInteraction() {
    // Complex case to test stack1 and stack2 interaction
    // Tree with multiple back edges: 0->1->2->3, 3->1, 2->4->5, 5->2
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 1);  // Back edge: SCC {1, 2, 3}
    g.addEdge(2, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 2);  // Back edge merges with previous SCC: {1, 2, 3, 4, 5}

    GabowSCC scc(g);

    // Should have 2 SCCs: {0}, {1,2,3,4,5}
    assert(scc.count() == 2);

    // Vertex 0 separate
    assert(scc.stronglyConnected(0, 1) == false);

    // Large SCC
    assert(scc.stronglyConnected(1, 2) == true);
    assert(scc.stronglyConnected(1, 3) == true);
    assert(scc.stronglyConnected(2, 4) == true);
    assert(scc.stronglyConnected(2, 5) == true);
    assert(scc.stronglyConnected(4, 5) == true);
    assert(scc.stronglyConnected(1, 5) == true);

    std::cout << "testComplexStackInteraction passed\n";
}

void testLargeCycle() {
    // Large cycle: 0->1->2->3->4->5->0
    Digraph g(6);
    for (int i = 0; i < 5; i++) {
        g.addEdge(i, i + 1);
    }
    g.addEdge(5, 0);  // Complete the cycle

    GabowSCC scc(g);

    // Should have 1 SCC
    assert(scc.count() == 1);

    // All vertices should be strongly connected
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            assert(scc.stronglyConnected(i, j) == true);
        }
    }

    std::cout << "testLargeCycle passed\n";
}

void testSingleVertex() {
    // Single vertex
    Digraph g(1);

    GabowSCC scc(g);

    assert(scc.count() == 1);
    assert(scc.stronglyConnected(0, 0) == true);
    assert(scc.id(0) == 0);

    std::cout << "testSingleVertex passed\n";
}

void testEmptyDigraph() {
    // Multiple vertices with no edges
    Digraph g(4);

    GabowSCC scc(g);

    // Each vertex is its own SCC
    assert(scc.count() == 4);

    // Only self-connections
    for (int i = 0; i < 4; i++) {
        assert(scc.stronglyConnected(i, i) == true);
        for (int j = 0; j < 4; j++) {
            if (i != j) {
                assert(scc.stronglyConnected(i, j) == false);
            }
        }
    }

    std::cout << "testEmptyDigraph passed\n";
}

void testGabowVsTarjanEquivalence() {
    // Test case where Gabow should produce same results as Tarjan
    // Complex graph with various SCC patterns
    Digraph g(8);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);  // SCC: {0, 1, 2}
    g.addEdge(1, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 3);  // SCC: {3, 4}
    g.addEdge(4, 5);  // Tree edge
    g.addEdge(5, 6);
    g.addEdge(6, 7);
    g.addEdge(7, 5);  // SCC: {5, 6, 7}

    GabowSCC scc(g);

    assert(scc.count() == 3);

    // Verify SCC structure
    assert(scc.stronglyConnected(0, 1) == true);
    assert(scc.stronglyConnected(0, 2) == true);
    assert(scc.stronglyConnected(1, 2) == true);

    assert(scc.stronglyConnected(3, 4) == true);

    assert(scc.stronglyConnected(5, 6) == true);
    assert(scc.stronglyConnected(5, 7) == true);
    assert(scc.stronglyConnected(6, 7) == true);

    // Between SCCs
    assert(scc.stronglyConnected(0, 3) == false);
    assert(scc.stronglyConnected(3, 5) == false);

    std::cout << "testGabowVsTarjanEquivalence passed\n";
}

void testCrossEdgeHandling() {
    // Test cross edges that don't affect SCC structure
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);  // Cross edge (if DFS visits 1 before 2)
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    // No back edges, so each vertex is its own SCC

    GabowSCC scc(g);

    assert(scc.count() == 6);

    // No strong connections except self
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (i == j) {
                assert(scc.stronglyConnected(i, j) == true);
            } else {
                assert(scc.stronglyConnected(i, j) == false);
            }
        }
    }

    std::cout << "testCrossEdgeHandling passed\n";
}

void testGabowProperties() {
    // Test fundamental SCC properties with Gabow's algorithm
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);  // SCC: {0, 1, 2}
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 3);  // SCC: {3, 4, 5}

    GabowSCC scc(g);

    // Test reflexivity: every vertex is strongly connected to itself
    for (int v = 0; v < g.V(); v++) {
        assert(scc.stronglyConnected(v, v) == true);
    }

    // Test symmetry: if u and v are strongly connected, so are v and u
    for (int u = 0; u < g.V(); u++) {
        for (int v = 0; v < g.V(); v++) {
            assert(scc.stronglyConnected(u, v) == scc.stronglyConnected(v, u));
        }
    }

    // Test transitivity: if u-v and v-w are strongly connected, so are u-w
    for (int u = 0; u < g.V(); u++) {
        for (int v = 0; v < g.V(); v++) {
            for (int w = 0; w < g.V(); w++) {
                if (scc.stronglyConnected(u, v) && scc.stronglyConnected(v, w)) {
                    assert(scc.stronglyConnected(u, w) == true);
                }
            }
        }
    }

    std::cout << "testGabowProperties passed\n";
}

void testInvalidVertex() {
    Digraph g(3);
    g.addEdge(0, 1);

    GabowSCC scc(g);

    // Test invalid vertex indices
    try {
        scc.stronglyConnected(-1, 0);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        scc.id(3);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        scc.stronglyConnected(0, 3);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "testInvalidVertex passed\n";
}

int main() {
    std::cout << "Running GabowSCC tests...\n";

    testSingleSCC();
    testMultipleSCCs();
    testDAG();
    testGabowTwoStackBehavior();
    testNestedCycles();
    testBackEdgeStackBehavior();
    testSelfLoops();
    testDisconnectedComponents();
    testComplexStackInteraction();
    testLargeCycle();
    testSingleVertex();
    testEmptyDigraph();
    testGabowVsTarjanEquivalence();
    testCrossEdgeHandling();
    testGabowProperties();
    testInvalidVertex();

    std::cout << "All tests passed!\n";
    return 0;
}