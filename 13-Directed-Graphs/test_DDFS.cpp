#include <cassert>
#include <iostream>

#include "Digraph.hpp"
#include "DirectedDFS.hpp"

int main() {
    // Create a digraph with 6 vertices
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(4, 5);

    // Perform DFS starting from vertex 0
    DirectedDFS dfs(g, 0);

    // Test if certain vertices are reachable from 0
    assert(dfs.marked(0) == true);
    assert(dfs.marked(1) == true);
    assert(dfs.marked(2) == true);
    assert(dfs.marked(3) == true);
    assert(dfs.marked(4) == false); // Vertex 4 is not reachable
    assert(dfs.marked(5) == false); // Vertex 5 is not reachable

    // Test the count of reachable vertices
    assert(dfs.count() == 4); // Only vertices 0, 1, 2, and 3 are reachable

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
