#include <cassert>
#include <iostream>

#include "../data_structures/ConnectedComponents.hpp"
#include "../data_structures/Graph.hpp"

void testSingleComponent() {
    // Create connected graph: 0-1-2-3
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    ConnectedComponents cc(g);

    // Should have 1 component
    assert(cc.count() == 1);

    // All vertices should have same component ID
    int componentId = cc.id(0);
    assert(cc.id(1) == componentId);
    assert(cc.id(2) == componentId);
    assert(cc.id(3) == componentId);

    // All vertices should be connected
    assert(cc.connected(0, 1) == true);
    assert(cc.connected(0, 3) == true);
    assert(cc.connected(1, 3) == true);

    // Component size should be 4
    assert(cc.size(0) == 4);
    assert(cc.size(1) == 4);
    assert(cc.size(2) == 4);
    assert(cc.size(3) == 4);

    std::cout << "testSingleComponent passed\n";
}

void testMultipleComponents() {
    // Create disconnected graph: 0-1  2-3  4
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(2, 3);
    // Vertex 4 is isolated

    ConnectedComponents cc(g);

    // Should have 3 components
    assert(cc.count() == 3);

    // Vertices in same component should have same ID
    assert(cc.id(0) == cc.id(1));
    assert(cc.id(2) == cc.id(3));

    // Vertices in different components should have different IDs
    assert(cc.id(0) != cc.id(2));
    assert(cc.id(0) != cc.id(4));
    assert(cc.id(2) != cc.id(4));

    // Test connectivity
    assert(cc.connected(0, 1) == true);
    assert(cc.connected(2, 3) == true);
    assert(cc.connected(0, 2) == false);
    assert(cc.connected(0, 4) == false);
    assert(cc.connected(2, 4) == false);

    // Test component sizes
    assert(cc.size(0) == 2);  // Component {0, 1}
    assert(cc.size(1) == 2);
    assert(cc.size(2) == 2);  // Component {2, 3}
    assert(cc.size(3) == 2);
    assert(cc.size(4) == 1);  // Component {4}

    std::cout << "testMultipleComponents passed\n";
}

void testSingleVertex() {
    // Graph with single vertex
    Graph g(1);
    ConnectedComponents cc(g);

    assert(cc.count() == 1);
    assert(cc.id(0) == 0);
    assert(cc.size(0) == 1);
    assert(cc.connected(0, 0) == true);

    std::cout << "testSingleVertex passed\n";
}

void testIsolatedVertices() {
    // Graph with no edges - all vertices isolated
    Graph g(4);
    ConnectedComponents cc(g);

    // Should have 4 components
    assert(cc.count() == 4);

    // Each vertex should be in its own component
    for (int i = 0; i < 4; i++) {
        assert(cc.size(i) == 1);
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                assert(cc.connected(i, j) == true);
            } else {
                assert(cc.connected(i, j) == false);
            }
        }
    }

    std::cout << "testIsolatedVertices passed\n";
}

void testCompleteGraph() {
    // Complete graph with 4 vertices
    Graph g(4);
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            g.addEdge(i, j);
        }
    }

    ConnectedComponents cc(g);

    // Should have 1 component
    assert(cc.count() == 1);

    // All vertices should be connected
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            assert(cc.connected(i, j) == true);
        }
        assert(cc.size(i) == 4);
    }

    std::cout << "testCompleteGraph passed\n";
}

void testStar() {
    // Star graph: center vertex 0 connected to 1,2,3
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);

    ConnectedComponents cc(g);

    // Should have 1 component
    assert(cc.count() == 1);

    // All vertices should be in same component
    int componentId = cc.id(0);
    for (int i = 1; i < 4; i++) {
        assert(cc.id(i) == componentId);
        assert(cc.connected(0, i) == true);
        assert(cc.size(i) == 4);
    }

    // Peripheral vertices connected through center
    assert(cc.connected(1, 2) == true);
    assert(cc.connected(1, 3) == true);
    assert(cc.connected(2, 3) == true);

    std::cout << "testStar passed\n";
}

void testLargerDisconnected() {
    // Two separate triangles: 0-1-2-0 and 3-4-5-3
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 3);

    ConnectedComponents cc(g);

    // Should have 2 components
    assert(cc.count() == 2);

    // First triangle
    assert(cc.connected(0, 1) == true);
    assert(cc.connected(1, 2) == true);
    assert(cc.connected(2, 0) == true);
    assert(cc.size(0) == 3);

    // Second triangle
    assert(cc.connected(3, 4) == true);
    assert(cc.connected(4, 5) == true);
    assert(cc.connected(5, 3) == true);
    assert(cc.size(3) == 3);

    // Triangles not connected to each other
    assert(cc.connected(0, 3) == false);
    assert(cc.connected(1, 4) == false);
    assert(cc.connected(2, 5) == false);

    std::cout << "testLargerDisconnected passed\n";
}

void testInvalidVertex() {
    Graph g(3);
    g.addEdge(0, 1);
    ConnectedComponents cc(g);

    // Test invalid vertex indices
    try {
        cc.id(-1);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        cc.size(3);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        cc.connected(-1, 0);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        cc.connected(0, 3);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "testInvalidVertex passed\n";
}

int main() {
    std::cout << "Running ConnectedComponents tests...\n";

    testSingleComponent();
    testMultipleComponents();
    testSingleVertex();
    testIsolatedVertices();
    testCompleteGraph();
    testStar();
    testLargerDisconnected();
    testInvalidVertex();

    std::cout << "All tests passed!\n";
    return 0;
}