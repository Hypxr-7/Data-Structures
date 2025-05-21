#include <cassert>
#include <iostream>

#include "BreadthFirstDirectedPaths.hpp"
#include "Digraph.hpp"

int main() {
    // Create a digraph with 6 vertices
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 5);

    // Compute shortest paths from source vertex 0
    BreadthFirstDirectedPaths bfsPaths(g, 0);

    // Test path existence
    assert(bfsPaths.hasPathTo(5) == true);  // Path exists from 0 to 5
    assert(bfsPaths.hasPathTo(4) == true);  // Path exists from 0 to 4
    assert(bfsPaths.hasPathTo(1) == true);  // Path exists from 0 to 1
    assert(bfsPaths.hasPathTo(0) == true);  // Path exists from 0 to itself
    assert(bfsPaths.hasPathTo(6) == false); // Invalid vertex, should return false

    // Test distances
    assert(bfsPaths.distTo(5) == 3); // Shortest distance from 0 to 5 is 3
    assert(bfsPaths.distTo(4) == 2); // Shortest distance from 0 to 4 is 2
    assert(bfsPaths.distTo(0) == 0); // Distance to itself is 0

    // Test specific paths
    std::vector<int> pathTo5 = bfsPaths.pathTo(5);
    std::vector<int> expectedPathTo5 = {0, 2, 4, 5}; // BFS will discover this shortest path
    assert(pathTo5 == expectedPathTo5);

    std::vector<int> pathTo4 = bfsPaths.pathTo(4);
    std::vector<int> expectedPathTo4 = {0, 2, 4};
    assert(pathTo4 == expectedPathTo4);

    // Test path to an unreachable vertex (should return an empty vector)
    std::vector<int> pathTo6 = bfsPaths.pathTo(6);
    assert(pathTo6.empty());

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
