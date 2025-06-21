
#include <cassert>
#include <iostream>
#include <set>
#include <vector>

#include "../data_structures/Digraph.hpp"
#include "../data_structures/KosarajuSharirSCC.hpp"

void testSingleSCC() {
    // Create strongly connected graph: 0->1->2->0
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    KosarajuSharirSCC scc(g);

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

    KosarajuSharirSCC scc(g);

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

    KosarajuSharirSCC scc(g);

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

void testSingleVertex() {
    // Single vertex (trivially strongly connected to itself)
    Digraph g(1);

    KosarajuSharirSCC scc(g);

    assert(scc.count() == 1);
    assert(scc.stronglyConnected(0, 0) == true);
    assert(scc.id(0) == 0);

    std::cout << "testSingleVertex passed\n";
}

void testSingleVertexWithSelfLoop() {
    // Single vertex with self-loop
    Digraph g(1);
    g.addEdge(0, 0);

    KosarajuSharirSCC scc(g);

    assert(scc.count() == 1);
    assert(scc.stronglyConnected(0, 0) == true);
    assert(scc.id(0) == 0);

    std::cout << "testSingleVertexWithSelfLoop passed\n";
}

void testDisconnectedComponents() {
    // Disconnected components: 0->1->0 and 2->3->2
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 0);  // SCC: {0, 1}
    g.addEdge(2, 3);
    g.addEdge(3, 2);  // SCC: {2, 3}

    KosarajuSharirSCC scc(g);

    // Should have 2 SCCs
    assert(scc.count() == 2);

    // Within components
    assert(scc.stronglyConnected(0, 1) == true);
    assert(scc.stronglyConnected(2, 3) == true);

    // Between components
    assert(scc.stronglyConnected(0, 2) == false);
    assert(scc.stronglyConnected(0, 3) == false);
    assert(scc.stronglyConnected(1, 2) == false);
    assert(scc.stronglyConnected(1, 3) == false);

    std::cout << "testDisconnectedComponents passed\n";
}

void testComplexSCCs() {
    // Complex graph with nested SCCs: 0->1->2->3->1, 2->4->5->4
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 1);  // SCC: {1, 2, 3}
    g.addEdge(2, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 4);  // SCC: {4, 5}

    KosarajuSharirSCC scc(g);

    // Should have 3 SCCs: {0}, {1,2,3}, {4,5}
    assert(scc.count() == 3);

    // Vertex 0 is its own SCC
    assert(scc.stronglyConnected(0, 1) == false);
    assert(scc.stronglyConnected(0, 2) == false);
    assert(scc.stronglyConnected(0, 3) == false);

    // Vertices 1, 2, 3 form an SCC
    assert(scc.stronglyConnected(1, 2) == true);
    assert(scc.stronglyConnected(1, 3) == true);
    assert(scc.stronglyConnected(2, 3) == true);

    // Vertices 4, 5 form an SCC
    assert(scc.stronglyConnected(4, 5) == true);

    // Different SCCs are not strongly connected
    assert(scc.stronglyConnected(1, 4) == false);
    assert(scc.stronglyConnected(2, 5) == false);

    std::cout << "testComplexSCCs passed\n";
}

void testLargeSCC() {
    // Large strongly connected component: 0->1->2->3->4->0
    Digraph g(5);
    for (int i = 0; i < 4; i++) {
        g.addEdge(i, i + 1);
    }
    g.addEdge(4, 0);  // Complete the cycle

    KosarajuSharirSCC scc(g);

    // Should have 1 SCC
    assert(scc.count() == 1);

    // All vertices should be strongly connected
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            assert(scc.stronglyConnected(i, j) == true);
        }
    }

    // All should have same id
    int commonId = scc.id(0);
    for (int i = 1; i < 5; i++) {
        assert(scc.id(i) == commonId);
    }

    std::cout << "testLargeSCC passed\n";
}

void testEmptyDigraph() {
    // Graph with vertices but no edges
    Digraph g(4);

    KosarajuSharirSCC scc(g);

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

    // All different ids
    std::set<int> ids;
    for (int i = 0; i < 4; i++) {
        ids.insert(scc.id(i));
    }
    assert(ids.size() == 4);

    std::cout << "testEmptyDigraph passed\n";
}

void testLinearChain() {
    // Linear chain: 0->1->2->3 (no cycles)
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    KosarajuSharirSCC scc(g);

    // Each vertex is its own SCC
    assert(scc.count() == 4);

    // No strong connections except self
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                assert(scc.stronglyConnected(i, j) == true);
            } else {
                assert(scc.stronglyConnected(i, j) == false);
            }
        }
    }

    std::cout << "testLinearChain passed\n";
}

void testCondensationDAG() {
    // Graph that forms a DAG when condensed: SCC1->SCC2->SCC3
    Digraph g(6);

    // SCC1: {0, 1}
    g.addEdge(0, 1);
    g.addEdge(1, 0);

    // SCC2: {2, 3}
    g.addEdge(2, 3);
    g.addEdge(3, 2);

    // SCC3: {4, 5}
    g.addEdge(4, 5);
    g.addEdge(5, 4);

    // Connections between SCCs
    g.addEdge(1, 2);  // SCC1 -> SCC2
    g.addEdge(3, 4);  // SCC2 -> SCC3

    KosarajuSharirSCC scc(g);

    assert(scc.count() == 3);

    // Within SCCs
    assert(scc.stronglyConnected(0, 1) == true);
    assert(scc.stronglyConnected(2, 3) == true);
    assert(scc.stronglyConnected(4, 5) == true);

    // Between SCCs
    assert(scc.stronglyConnected(0, 2) == false);
    assert(scc.stronglyConnected(2, 4) == false);
    assert(scc.stronglyConnected(0, 4) == false);

    std::cout << "testCondensationDAG passed\n";
}

void testSelfLoopsInSCCs() {
    // SCCs with self-loops: 0->0, 1->2->1, 3->3
    Digraph g(4);
    g.addEdge(0, 0);  // Self-loop SCC
    g.addEdge(1, 2);
    g.addEdge(2, 1);  // Cycle SCC
    g.addEdge(3, 3);  // Self-loop SCC

    KosarajuSharirSCC scc(g);

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

    std::cout << "testSelfLoopsInSCCs passed\n";
}

void testInvalidVertex() {
    Digraph g(3);
    g.addEdge(0, 1);

    KosarajuSharirSCC scc(g);

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

void testSCCProperties() {
    // Test fundamental SCC properties
    Digraph g(5);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);  // SCC: {0, 1, 2}
    g.addEdge(2, 3);
    g.addEdge(3, 4);  // Chain: 3->4

    KosarajuSharirSCC scc(g);

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

    std::cout << "testSCCProperties passed\n";
}

int main() {
    std::cout << "Running KosarajuSharirSCC tests...\n";

    testSingleSCC();
    testMultipleSCCs();
    testDAG();
    testSingleVertex();
    testSingleVertexWithSelfLoop();
    testDisconnectedComponents();
    testComplexSCCs();
    testLargeSCC();
    testEmptyDigraph();
    testLinearChain();
    testCondensationDAG();
    testSelfLoopsInSCCs();
    testInvalidVertex();
    testSCCProperties();

    std::cout << "All tests passed!\n";
    return 0;
}