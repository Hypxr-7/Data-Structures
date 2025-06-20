#include <cassert>
#include <iostream>

#include "../data_structures/BreadthFirstDirectedPaths.hpp"
#include "../data_structures/Digraph.hpp"

void testBasicDirectedPath() {
    // Create directed path: 0->1->2->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    BreadthFirstDirectedPaths bfs(g, 0);

    // Test reachability
    assert(bfs.hasPathTo(0) == true);
    assert(bfs.hasPathTo(1) == true);
    assert(bfs.hasPathTo(2) == true);
    assert(bfs.hasPathTo(3) == true);

    // Test distances
    assert(bfs.distTo(0) == 0);
    assert(bfs.distTo(1) == 1);
    assert(bfs.distTo(2) == 2);
    assert(bfs.distTo(3) == 3);

    // Test path from 0 to 3
    std::vector<int> path = bfs.pathTo(3);
    assert(path.size() == 4);
    assert(path[0] == 0);
    assert(path[1] == 1);
    assert(path[2] == 2);
    assert(path[3] == 3);

    std::cout << "testBasicDirectedPath passed\n";
}

void testDirectedTree() {
    // Create directed tree: 0->1, 0->2, 1->3, 2->4
    Digraph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);

    BreadthFirstDirectedPaths bfs(g, 0);

    // Test distances (shortest paths)
    assert(bfs.distTo(0) == 0);
    assert(bfs.distTo(1) == 1);
    assert(bfs.distTo(2) == 1);
    assert(bfs.distTo(3) == 2);
    assert(bfs.distTo(4) == 2);

    // Test paths
    std::vector<int> pathTo3 = bfs.pathTo(3);
    assert(pathTo3.size() == 3);
    assert(pathTo3[0] == 0);
    assert(pathTo3[1] == 1);
    assert(pathTo3[2] == 3);

    std::vector<int> pathTo4 = bfs.pathTo(4);
    assert(pathTo4.size() == 3);
    assert(pathTo4[0] == 0);
    assert(pathTo4[1] == 2);
    assert(pathTo4[2] == 4);

    std::cout << "testDirectedTree passed\n";
}

void testShortestPathProperty() {
    // Create digraph with multiple paths: 0->1->3, 0->2->3 (both distance 2)
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);

    BreadthFirstDirectedPaths bfs(g, 0);

    // BFS finds shortest path (distance 2, not longer)
    assert(bfs.distTo(3) == 2);

    // Path should be one of the two shortest paths
    std::vector<int> path = bfs.pathTo(3);
    assert(path.size() == 3);
    assert(path[0] == 0);
    assert(path[2] == 3);
    // path[1] could be either 1 or 2

    std::cout << "testShortestPathProperty passed\n";
}

void testDisconnectedDigraph() {
    // Create disconnected digraph: 0->1  2->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(2, 3);

    BreadthFirstDirectedPaths bfs(g, 0);

    // Test reachability from vertex 0
    assert(bfs.hasPathTo(0) == true);
    assert(bfs.hasPathTo(1) == true);
    assert(bfs.hasPathTo(2) == false);
    assert(bfs.hasPathTo(3) == false);

    // Test distances
    assert(bfs.distTo(0) == 0);
    assert(bfs.distTo(1) == 1);
    assert(bfs.distTo(2) == std::numeric_limits<int>::max());
    assert(bfs.distTo(3) == std::numeric_limits<int>::max());

    // Test paths
    assert(!bfs.pathTo(1).empty());
    assert(bfs.pathTo(2).empty());
    assert(bfs.pathTo(3).empty());

    std::cout << "testDisconnectedDigraph passed\n";
}

void testDirectedCycle() {
    // Create directed cycle: 0->1->2->0
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    BreadthFirstDirectedPaths bfs(g, 0);

    // All vertices reachable in strongly connected component
    assert(bfs.hasPathTo(0) == true);
    assert(bfs.hasPathTo(1) == true);
    assert(bfs.hasPathTo(2) == true);

    // Test shortest distances
    assert(bfs.distTo(0) == 0);
    assert(bfs.distTo(1) == 1);
    assert(bfs.distTo(2) == 2);

    std::cout << "testDirectedCycle passed\n";
}

void testSingleVertex() {
    // Single vertex digraph
    Digraph g(1);
    BreadthFirstDirectedPaths bfs(g, 0);

    assert(bfs.hasPathTo(0) == true);
    assert(bfs.distTo(0) == 0);

    std::vector<int> path = bfs.pathTo(0);
    assert(path.size() == 1);
    assert(path[0] == 0);

    std::cout << "testSingleVertex passed\n";
}

void testDirectionalityMatters() {
    // Show that direction matters: 0->1, 2->1
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(2, 1);

    // From vertex 0
    BreadthFirstDirectedPaths bfs0(g, 0);
    assert(bfs0.hasPathTo(0) == true);
    assert(bfs0.hasPathTo(1) == true);
    assert(bfs0.hasPathTo(2) == false);
    assert(bfs0.distTo(1) == 1);

    // From vertex 2
    BreadthFirstDirectedPaths bfs2(g, 2);
    assert(bfs2.hasPathTo(0) == false);
    assert(bfs2.hasPathTo(1) == true);
    assert(bfs2.hasPathTo(2) == true);
    assert(bfs2.distTo(1) == 1);

    std::cout << "testDirectionalityMatters passed\n";
}

void testComplexDigraph() {
    // More complex directed graph with multiple shortest paths
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(1, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 5);

    BreadthFirstDirectedPaths bfs(g, 0);

    // Test shortest distances
    assert(bfs.distTo(0) == 0);
    assert(bfs.distTo(1) == 1);
    assert(bfs.distTo(2) == 1);
    assert(bfs.distTo(3) == 2);  // Via 0->1->3 or 0->2->3
    assert(bfs.distTo(4) == 2);  // Via 0->1->4
    assert(bfs.distTo(5) == 3);  // Via 0->1->4->5 or 0->?->3->5

    // Test that paths have correct lengths
    assert(bfs.pathTo(3).size() == 3);
    assert(bfs.pathTo(4).size() == 3);
    assert(bfs.pathTo(5).size() == 4);

    std::cout << "testComplexDigraph passed\n";
}

void testEmptyDigraph() {
    // Graph with vertices but no edges
    Digraph g(3);
    BreadthFirstDirectedPaths bfs(g, 0);

    // Only source is reachable
    assert(bfs.hasPathTo(0) == true);
    assert(bfs.hasPathTo(1) == false);
    assert(bfs.hasPathTo(2) == false);

    assert(bfs.distTo(0) == 0);
    assert(bfs.distTo(1) == std::numeric_limits<int>::max());
    assert(bfs.distTo(2) == std::numeric_limits<int>::max());

    assert(bfs.pathTo(0).size() == 1);
    assert(bfs.pathTo(1).empty());
    assert(bfs.pathTo(2).empty());

    std::cout << "testEmptyDigraph passed\n";
}

void testLargerDistances() {
    // Create longer directed path to test larger distances
    Digraph g(7);
    for (int i = 0; i < 6; i++) {
        g.addEdge(i, i + 1);
    }

    BreadthFirstDirectedPaths bfs(g, 0);

    // Test increasing distances
    for (int i = 0; i < 7; i++) {
        assert(bfs.distTo(i) == i);
    }

    // Test path to last vertex
    std::vector<int> pathTo6 = bfs.pathTo(6);
    assert(pathTo6.size() == 7);
    for (int i = 0; i < 7; i++) {
        assert(pathTo6[i] == i);
    }

    std::cout << "testLargerDistances passed\n";
}

void testInvalidVertex() {
    Digraph g(3);
    g.addEdge(0, 1);
    BreadthFirstDirectedPaths bfs(g, 0);

    // Test invalid vertex indices
    try {
        bfs.hasPathTo(-1);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        bfs.distTo(3);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        bfs.pathTo(4);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "testInvalidVertex passed\n";
}

void testInvalidSource() {
    Digraph g(3);

    try {
        BreadthFirstDirectedPaths bfs(g, -1);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        BreadthFirstDirectedPaths bfs(g, 3);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "testInvalidSource passed\n";
}

int main() {
    std::cout << "Running BreadthFirstDirectedPaths tests...\n";

    testBasicDirectedPath();
    testDirectedTree();
    testShortestPathProperty();
    testDisconnectedDigraph();
    testDirectedCycle();
    testSingleVertex();
    testDirectionalityMatters();
    testComplexDigraph();
    testEmptyDigraph();
    testLargerDistances();
    testInvalidVertex();
    testInvalidSource();

    std::cout << "All tests passed!\n";
    return 0;
}