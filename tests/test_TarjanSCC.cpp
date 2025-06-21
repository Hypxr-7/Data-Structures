#include <cassert>
#include <iostream>
#include <set>
#include <vector>

#include "../data_structures/Digraph.hpp"
#include "../data_structures/TarjanSCC.hpp"

void testSingleSCC() {
    // Create strongly connected graph: 0->1->2->0
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    TarjanSCC scc(g);

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

    TarjanSCC scc(g);

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

    TarjanSCC scc(g);

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

void testComplexSCCs() {
    // Complex graph: 0->1->2->3->1, 2->4->5->4, 0->6
    Digraph g(7);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 1);  // SCC: {1, 2, 3}
    g.addEdge(2, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 4);  // SCC: {4, 5}
    g.addEdge(0, 6);  // SCC: {0}, {6}

    TarjanSCC scc(g);

    // Should have 4 SCCs: {0}, {1,2,3}, {4,5}, {6}
    assert(scc.count() == 4);

    // Vertices 1, 2, 3 form an SCC
    assert(scc.stronglyConnected(1, 2) == true);
    assert(scc.stronglyConnected(1, 3) == true);
    assert(scc.stronglyConnected(2, 3) == true);

    // Vertices 4, 5 form an SCC
    assert(scc.stronglyConnected(4, 5) == true);

    // Isolated vertices
    assert(scc.stronglyConnected(0, 1) == false);
    assert(scc.stronglyConnected(0, 6) == false);

    std::cout << "testComplexSCCs passed\n";
}

void testNestedSCCs() {
    // Nested structure with back edges: 0->1->2->3->0, 1->4->5->1
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 0);  // Outer cycle: {0, 1, 2, 3}
    g.addEdge(1, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 1);  // Inner cycle should merge with outer: {0, 1, 2, 3, 4, 5}

    TarjanSCC scc(g);

    // All vertices should be in one SCC due to connectivity
    assert(scc.count() == 1);

    // All vertices should be strongly connected
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            assert(scc.stronglyConnected(i, j) == true);
        }
    }

    std::cout << "testNestedSCCs passed\n";
}

void testSelfLoops() {
    // Graph with self-loops: 0->0, 1->2->1, 3->3
    Digraph g(4);
    g.addEdge(0, 0);  // Self-loop SCC
    g.addEdge(1, 2);
    g.addEdge(2, 1);  // Cycle SCC
    g.addEdge(3, 3);  // Self-loop SCC

    TarjanSCC scc(g);

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

void testCrossEdges() {
    // Graph with cross edges that don't create cycles
    Digraph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(1, 4);
    // No back edges, so each vertex is its own SCC

    TarjanSCC scc(g);

    assert(scc.count() == 5);

    // No strong connections except self
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (i == j) {
                assert(scc.stronglyConnected(i, j) == true);
            } else {
                assert(scc.stronglyConnected(i, j) == false);
            }
        }
    }

    std::cout << "testCrossEdges passed\n";
}

void testDisconnectedSCCs() {
    // Multiple disconnected SCCs: 0->1->0, 2->3->4->2, 5
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(1, 0);  // SCC: {0, 1}
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 2);  // SCC: {2, 3, 4}
    // Vertex 5 is isolated: SCC: {5}

    TarjanSCC scc(g);

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

    std::cout << "testDisconnectedSCCs passed\n";
}

void testLargeCycle() {
    // Large cycle: 0->1->2->3->4->5->0
    Digraph g(6);
    for (int i = 0; i < 5; i++) {
        g.addEdge(i, i + 1);
    }
    g.addEdge(5, 0);  // Complete the cycle

    TarjanSCC scc(g);

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

    TarjanSCC scc(g);

    assert(scc.count() == 1);
    assert(scc.stronglyConnected(0, 0) == true);
    assert(scc.id(0) == 0);

    std::cout << "testSingleVertex passed\n";
}

void testEmptyDigraph() {
    // Multiple vertices with no edges
    Digraph g(4);

    TarjanSCC scc(g);

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

    TarjanSCC scc(g);

    // Should have 1 SCC (all vertices strongly connected)
    assert(scc.count() == 1);

    // All vertices should be strongly connected
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            assert(scc.stronglyConnected(i, j) == true);
        }
    }

    std::cout << "testCompleteDigraph passed\n";
}

void testBackEdgeDetection() {
    // Test Tarjan's ability to detect back edges correctly
    // 0->1->2, 2->0 (back edge), 1->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);  // Back edge creates SCC {0, 1, 2}
    g.addEdge(1, 3);  // Forward edge to separate vertex

    TarjanSCC scc(g);

    assert(scc.count() == 2);

    // Vertices 0, 1, 2 in same SCC
    assert(scc.stronglyConnected(0, 1) == true);
    assert(scc.stronglyConnected(0, 2) == true);
    assert(scc.stronglyConnected(1, 2) == true);

    // Vertex 3 in separate SCC
    assert(scc.stronglyConnected(0, 3) == false);
    assert(scc.stronglyConnected(1, 3) == false);
    assert(scc.stronglyConnected(2, 3) == false);

    std::cout << "testBackEdgeDetection passed\n";
}

void testTarjanProperties() {
    // Test fundamental SCC properties with Tarjan's algorithm
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);  // SCC: {0, 1, 2}
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 3);  // SCC: {3, 4, 5}

    TarjanSCC scc(g);

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

    std::cout << "testTarjanProperties passed\n";
}

void testInvalidVertex() {
    Digraph g(3);
    g.addEdge(0, 1);

    TarjanSCC scc(g);

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
    std::cout << "Running TarjanSCC tests...\n";

    testSingleSCC();
    testMultipleSCCs();
    testDAG();
    testComplexSCCs();
    testNestedSCCs();
    testSelfLoops();
    testCrossEdges();
    testDisconnectedSCCs();
    testLargeCycle();
    testSingleVertex();
    testEmptyDigraph();
    testCompleteDigraph();
    testBackEdgeDetection();
    testTarjanProperties();
    testInvalidVertex();

    std::cout << "All tests passed!\n";
    return 0;
}
