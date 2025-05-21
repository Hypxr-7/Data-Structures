#include <cassert>
#include <iostream>

#include "DepthFirstDirectedPaths.hpp"
#include "Digraph.hpp"

int main() {
    // Create a digraph with 6 vertices
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 5);

    // Compute paths from source vertex 0
    DepthFirstDirectedPaths paths(g, 0);

    // Test path existence
    assert(paths.hasPathTo(5) == true);  // Path exists from 0 to 5
    assert(paths.hasPathTo(4) == true);  // Path exists from 0 to 4
    assert(paths.hasPathTo(1) == true);  // Path exists from 0 to 1
    assert(paths.hasPathTo(0) == true);  // Path exists from 0 to itself
    assert(paths.hasPathTo(6) == false); // Invalid vertex, should return false

    // Test specific paths
    std::vector<int> pathTo5 = paths.pathTo(5);
    std::vector<int> expectedPathTo5 = {0, 1, 3, 5};
    assert(pathTo5 == expectedPathTo5);

    std::vector<int> pathTo4 = paths.pathTo(4);
    std::vector<int> expectedPathTo4 = {0, 2, 4};
    assert(pathTo4 == expectedPathTo4);

    // Test path to an unreachable vertex (should return an empty vector)
    std::vector<int> pathTo6 = paths.pathTo(6);
    assert(pathTo6.empty());

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
