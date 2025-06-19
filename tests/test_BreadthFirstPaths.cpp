#include <cassert>
#include <iostream>

#include "../data_structures/BreadthFirstPaths.hpp"
#include "../data_structures/Graph.hpp"

void testBasicPath() {
    // Create simple path graph: 0-1-2-3
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    BreadthFirstPaths bfs(g, 0);

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

    std::cout << "testBasicPath passed\n";
}

void testShortestPath() {
    // Create graph with multiple paths: 0-1-3 and 0-2-3
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);

    BreadthFirstPaths bfs(g, 0);

    // Test shortest distances
    assert(bfs.distTo(0) == 0);
    assert(bfs.distTo(1) == 1);
    assert(bfs.distTo(2) == 1);
    assert(bfs.distTo(3) == 2);  // Shortest path is 0-1-3 or 0-2-3

    // Verify path to 3 has length 2
    std::vector<int> path = bfs.pathTo(3);
    assert(path.size() == 3);
    assert(path[0] == 0);
    assert(path[2] == 3);

    std::cout << "testShortestPath passed\n";
}

void testDisconnectedGraph() {
    // Create disconnected graph: 0-1  2-3
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(2, 3);

    BreadthFirstPaths bfs(g, 0);

    // Test reachability
    assert(bfs.hasPathTo(0) == true);
    assert(bfs.hasPathTo(1) == true);
    assert(bfs.hasPathTo(2) == false);
    assert(bfs.hasPathTo(3) == false);

    // Test distances
    assert(bfs.distTo(0) == 0);
    assert(bfs.distTo(1) == 1);
    assert(bfs.distTo(2) == std::numeric_limits<int>::max());
    assert(bfs.distTo(3) == std::numeric_limits<int>::max());

    // Test empty paths to unreachable vertices
    assert(bfs.pathTo(2).empty());
    assert(bfs.pathTo(3).empty());

    std::cout << "testDisconnectedGraph passed\n";
}

void testSingleVertex() {
    // Graph with single vertex
    Graph g(1);
    BreadthFirstPaths bfs(g, 0);

    assert(bfs.hasPathTo(0) == true);
    assert(bfs.distTo(0) == 0);

    // Path from vertex to itself
    std::vector<int> path = bfs.pathTo(0);
    assert(path.size() == 1);
    assert(path[0] == 0);

    std::cout << "testSingleVertex passed\n";
}

void testCycle() {
    // Create cycle: 0-1-2-0
    Graph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    BreadthFirstPaths bfs(g, 0);

    // All vertices reachable
    assert(bfs.hasPathTo(0) == true);
    assert(bfs.hasPathTo(1) == true);
    assert(bfs.hasPathTo(2) == true);

    // Test shortest distances
    assert(bfs.distTo(0) == 0);
    assert(bfs.distTo(1) == 1);
    assert(bfs.distTo(2) == 1);  // Direct edge 0-2, not 0-1-2

    std::cout << "testCycle passed\n";
}

void testStar() {
    // Star graph: center vertex 0 connected to 1,2,3
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);

    BreadthFirstPaths bfs(g, 0);

    // All vertices at distance 1 from center
    assert(bfs.distTo(0) == 0);
    assert(bfs.distTo(1) == 1);
    assert(bfs.distTo(2) == 1);
    assert(bfs.distTo(3) == 1);

    // Test from peripheral vertex
    BreadthFirstPaths bfs2(g, 1);
    assert(bfs2.distTo(0) == 1);
    assert(bfs2.distTo(1) == 0);
    assert(bfs2.distTo(2) == 2);  // 1-0-2
    assert(bfs2.distTo(3) == 2);  // 1-0-3

    std::cout << "testStar passed\n";
}

void testCompleteGraph() {
    // Complete graph with 4 vertices
    Graph g(4);
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            g.addEdge(i, j);
        }
    }

    BreadthFirstPaths bfs(g, 0);

    // All vertices at distance 1 from any vertex
    assert(bfs.distTo(0) == 0);
    assert(bfs.distTo(1) == 1);
    assert(bfs.distTo(2) == 1);
    assert(bfs.distTo(3) == 1);

    std::cout << "testCompleteGraph passed\n";
}

void testInvalidVertex() {
    Graph g(3);
    g.addEdge(0, 1);

    BreadthFirstPaths bfs(g, 0);

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
        bfs.pathTo(-1);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "testInvalidVertex passed\n";
}

void testInvalidSource() {
    Graph g(3);

    try {
        BreadthFirstPaths bfs(g, -1);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        BreadthFirstPaths bfs(g, 3);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "testInvalidSource passed\n";
}

int main() {
    std::cout << "Running BreadthFirstPaths tests...\n";

    testBasicPath();
    testShortestPath();
    testDisconnectedGraph();
    testSingleVertex();
    testCycle();
    testStar();
    testCompleteGraph();
    testInvalidVertex();
    testInvalidSource();

    std::cout << "All tests passed!\n";
    return 0;
}