#include <cassert>
#include <iostream>

#include "../data_structures/DepthFirstPaths.hpp"
#include "../data_structures/Graph.hpp"

void testBasicPath() {
    // Create simple path graph: 0-1-2-3
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    DepthFirstPaths dfs(g, 0);

    // Test reachability
    assert(dfs.hasPathTo(0) == true);
    assert(dfs.hasPathTo(1) == true);
    assert(dfs.hasPathTo(2) == true);
    assert(dfs.hasPathTo(3) == true);

    // Test path from 0 to 3
    std::vector<int> path = dfs.pathTo(3);
    assert(path.size() == 4);
    assert(path[0] == 0);
    assert(path[3] == 3);

    std::cout << "testBasicPath passed\n";
}

void testDisconnectedGraph() {
    // Create disconnected graph: 0-1  2-3
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(2, 3);

    DepthFirstPaths dfs(g, 0);

    // Test reachability
    assert(dfs.hasPathTo(0) == true);
    assert(dfs.hasPathTo(1) == true);
    assert(dfs.hasPathTo(2) == false);  // Not reachable from 0
    assert(dfs.hasPathTo(3) == false);  // Not reachable from 0

    // Test empty path to unreachable vertex
    std::vector<int> path = dfs.pathTo(2);
    assert(path.empty());

    std::cout << "testDisconnectedGraph passed\n";
}

void testSingleVertex() {
    // Graph with single vertex
    Graph g(1);
    DepthFirstPaths dfs(g, 0);

    assert(dfs.hasPathTo(0) == true);

    // Path from vertex to itself
    std::vector<int> path = dfs.pathTo(0);
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

    DepthFirstPaths dfs(g, 0);

    // All vertices reachable
    assert(dfs.hasPathTo(0) == true);
    assert(dfs.hasPathTo(1) == true);
    assert(dfs.hasPathTo(2) == true);

    // Path exists to all vertices
    assert(!dfs.pathTo(1).empty());
    assert(!dfs.pathTo(2).empty());

    std::cout << "testCycle passed\n";
}

void testCompleteGraph() {
    // Complete graph with 4 vertices
    Graph g(4);
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            g.addEdge(i, j);
        }
    }

    DepthFirstPaths dfs(g, 0);

    // All vertices reachable from any source
    for (int i = 0; i < 4; i++) {
        assert(dfs.hasPathTo(i) == true);
        assert(!dfs.pathTo(i).empty());
    }

    std::cout << "testCompleteGraph passed\n";
}

void testInvalidVertex() {
    Graph g(3);
    g.addEdge(0, 1);

    DepthFirstPaths dfs(g, 0);

    // Test invalid vertex indices
    try {
        dfs.hasPathTo(-1);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        // Expected exception
    }

    try {
        dfs.hasPathTo(3); // Out of bounds
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        // Expected exception
    }

    try {
        dfs.pathTo(-1);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        // Expected exception
    }

    std::cout << "testInvalidVertex passed\n";
}

void testInvalidSource() {
    Graph g(3);

    // Test invalid source vertex
    try {
        DepthFirstPaths dfs(g, -1);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        // Expected exception
    }

    try {
        DepthFirstPaths dfs(g, 3); // Out of bounds
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        // Expected exception
    }

    std::cout << "testInvalidSource passed\n";
}

int main() {
    std::cout << "Running DepthFirstPaths tests...\n";

    testBasicPath();
    testDisconnectedGraph();
    testSingleVertex();
    testCycle();
    testCompleteGraph();
    testInvalidVertex();
    testInvalidSource();

    std::cout << "All tests passed!\n";
    return 0;
}