#include <cassert>
#include <iostream>

#include "../data_structures/DepthFirstDirectedPaths.hpp"
#include "../data_structures/Digraph.hpp"

void testBasicDirectedPath() {
    // Create directed path: 0->1->2->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    DepthFirstDirectedPaths dfs(g, 0);

    // Test reachability
    assert(dfs.hasPathTo(0) == true);
    assert(dfs.hasPathTo(1) == true);
    assert(dfs.hasPathTo(2) == true);
    assert(dfs.hasPathTo(3) == true);

    // Test path from 0 to 3
    std::vector<int> path = dfs.pathTo(3);
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

    DepthFirstDirectedPaths dfs(g, 0);

    // All vertices reachable from 0
    for (int i = 0; i < 5; i++) {
        assert(dfs.hasPathTo(i) == true);
    }

    // Test specific paths
    std::vector<int> pathTo3 = dfs.pathTo(3);
    assert(pathTo3.size() == 3);
    assert(pathTo3[0] == 0);
    assert(pathTo3[1] == 1);
    assert(pathTo3[2] == 3);

    std::vector<int> pathTo4 = dfs.pathTo(4);
    assert(pathTo4.size() == 3);
    assert(pathTo4[0] == 0);
    assert(pathTo4[1] == 2);
    assert(pathTo4[2] == 4);

    std::cout << "testDirectedTree passed\n";
}

void testDisconnectedDigraph() {
    // Create disconnected digraph: 0->1  2->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(2, 3);

    DepthFirstDirectedPaths dfs(g, 0);

    // Test reachability from vertex 0
    assert(dfs.hasPathTo(0) == true);
    assert(dfs.hasPathTo(1) == true);
    assert(dfs.hasPathTo(2) == false);  // Not reachable via directed edges
    assert(dfs.hasPathTo(3) == false);

    // Test paths
    assert(!dfs.pathTo(1).empty());
    assert(dfs.pathTo(2).empty());
    assert(dfs.pathTo(3).empty());

    std::cout << "testDisconnectedDigraph passed\n";
}

void testDirectedCycle() {
    // Create directed cycle: 0->1->2->0
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    DepthFirstDirectedPaths dfs(g, 0);

    // All vertices reachable in a strongly connected component
    assert(dfs.hasPathTo(0) == true);
    assert(dfs.hasPathTo(1) == true);
    assert(dfs.hasPathTo(2) == true);

    // Test path to vertex 2
    std::vector<int> pathTo2 = dfs.pathTo(2);
    assert(pathTo2.size() == 3);
    assert(pathTo2[0] == 0);
    assert(pathTo2[1] == 1);
    assert(pathTo2[2] == 2);

    std::cout << "testDirectedCycle passed\n";
}

void testSingleVertex() {
    // Single vertex digraph
    Digraph g(1);
    DepthFirstDirectedPaths dfs(g, 0);

    assert(dfs.hasPathTo(0) == true);

    std::vector<int> path = dfs.pathTo(0);
    assert(path.size() == 1);
    assert(path[0] == 0);

    std::cout << "testSingleVertex passed\n";
}

void testDirectionalityMatters() {
    // Show that direction matters: 0->1, 2->1 (1 is reachable from 0 but not vice versa)
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(2, 1);

    // From vertex 0
    DepthFirstDirectedPaths dfs0(g, 0);
    assert(dfs0.hasPathTo(0) == true);
    assert(dfs0.hasPathTo(1) == true);  // 0->1
    assert(dfs0.hasPathTo(2) == false); // No path 0->2

    // From vertex 1
    DepthFirstDirectedPaths dfs1(g, 1);
    assert(dfs1.hasPathTo(0) == false); // No path 1->0
    assert(dfs1.hasPathTo(1) == true);
    assert(dfs1.hasPathTo(2) == false); // No path 1->2

    // From vertex 2
    DepthFirstDirectedPaths dfs2(g, 2);
    assert(dfs2.hasPathTo(0) == false); // No path 2->0
    assert(dfs2.hasPathTo(1) == true);  // 2->1
    assert(dfs2.hasPathTo(2) == true);

    std::cout << "testDirectionalityMatters passed\n";
}

void testComplexDigraph() {
    // More complex directed graph
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(5, 4); // 5 is disconnected from 0's component

    DepthFirstDirectedPaths dfs(g, 0);

    // Reachable from 0
    assert(dfs.hasPathTo(0) == true);
    assert(dfs.hasPathTo(1) == true);
    assert(dfs.hasPathTo(2) == true);
    assert(dfs.hasPathTo(3) == true);
    assert(dfs.hasPathTo(4) == true);
    assert(dfs.hasPathTo(5) == false); // Not reachable

    // Test multiple paths to same vertex (DFS finds one)
    std::vector<int> pathTo4 = dfs.pathTo(4);
    assert(pathTo4[0] == 0);
    assert(pathTo4[pathTo4.size()-1] == 4);
    assert(pathTo4.size() >= 3); // At least 0->?->4

    std::cout << "testComplexDigraph passed\n";
}

void testEmptyDigraph() {
    // Graph with vertices but no edges
    Digraph g(3);
    DepthFirstDirectedPaths dfs(g, 0);

    // Only source is reachable
    assert(dfs.hasPathTo(0) == true);
    assert(dfs.hasPathTo(1) == false);
    assert(dfs.hasPathTo(2) == false);

    assert(dfs.pathTo(0).size() == 1);
    assert(dfs.pathTo(1).empty());
    assert(dfs.pathTo(2).empty());

    std::cout << "testEmptyDigraph passed\n";
}

void testInvalidVertex() {
    Digraph g(3);
    g.addEdge(0, 1);
    DepthFirstDirectedPaths dfs(g, 0);

    // Test invalid vertex indices
    try {
        dfs.hasPathTo(-1);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        dfs.pathTo(3);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "testInvalidVertex passed\n";
}

void testInvalidSource() {
    Digraph g(3);

    try {
        DepthFirstDirectedPaths dfs(g, -1);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        DepthFirstDirectedPaths dfs(g, 3);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "testInvalidSource passed\n";
}

int main() {
    std::cout << "Running DepthFirstDirectedPaths tests...\n";

    testBasicDirectedPath();
    testDirectedTree();
    testDisconnectedDigraph();
    testDirectedCycle();
    testSingleVertex();
    testDirectionalityMatters();
    testComplexDigraph();
    testEmptyDigraph();
    testInvalidVertex();
    testInvalidSource();

    std::cout << "All tests passed!\n";
    return 0;
}