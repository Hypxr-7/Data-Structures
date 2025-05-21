#include <cassert>
#include <iostream>

#include "Digraph.hpp"


void testDigraph() {
    // Test 1: Create a Digraph with 5 vertices
    Digraph g(5);
    assert(g.V() == 5); // Check number of vertices
    assert(g.E() == 0); // Check number of edges initially

    // Test 2: Add edges and verify
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(2, 3);
    g.addEdge(4, 0);

    assert(g.E() == 4); // Check number of edges after adding

    // Verify adjacency lists
    std::forward_list<int> adj0 = g.adj(0);
    std::forward_list<int> expectedAdj0 = {2, 1};
    assert(std::equal(adj0.begin(), adj0.end(), expectedAdj0.begin()));

    std::forward_list<int> adj2 = g.adj(2);
    std::forward_list<int> expectedAdj2 = {3};
    assert(std::equal(adj2.begin(), adj2.end(), expectedAdj2.begin()));

    std::forward_list<int> adj4 = g.adj(4);
    std::forward_list<int> expectedAdj4 = {0};
    assert(std::equal(adj4.begin(), adj4.end(), expectedAdj4.begin()));

    // Test 3: Reverse graph and verify
    Digraph reversedG = g.reverse();
    assert(reversedG.V() == 5); // Number of vertices should remain the same
    assert(reversedG.E() == 4); // Number of edges should remain the same

    // Verify reversed adjacency lists
    std::forward_list<int> revAdj1 = reversedG.adj(1);
    std::forward_list<int> expectedRevAdj1 = {0};
    assert(std::equal(revAdj1.begin(), revAdj1.end(), expectedRevAdj1.begin()));

    std::forward_list<int> revAdj2 = reversedG.adj(2);
    std::forward_list<int> expectedRevAdj2 = {0};
    assert(std::equal(revAdj2.begin(), revAdj2.end(), expectedRevAdj2.begin()));

    std::forward_list<int> revAdj3 = reversedG.adj(3);
    std::forward_list<int> expectedRevAdj3 = {2};
    assert(std::equal(revAdj3.begin(), revAdj3.end(), expectedRevAdj3.begin()));

    std::forward_list<int> revAdj0 = reversedG.adj(0);
    std::forward_list<int> expectedRevAdj0 = {4};
    assert(std::equal(revAdj0.begin(), revAdj0.end(), expectedRevAdj0.begin()));

    std::cout << "All tests passed!\n";
}

int main() {
    testDigraph();
    return 0;
}
