#include <iostream>
#include <cassert>

#include "EdgeWeightedGraph.hpp"
#include "Edge.hpp"

void testEdgeWeightedGraph() {
    // Create a graph with 5 vertices
    EdgeWeightedGraph graph(5);

    // Add edges
    Edge e1(0, 1, 0.5);
    Edge e2(0, 2, 1.2);
    Edge e3(1, 3, 2.3);
    Edge e4(2, 3, 1.8);
    Edge e5(3, 4, 0.9);

    graph.addEdge(e1);
    graph.addEdge(e2);
    graph.addEdge(e3);
    graph.addEdge(e4);
    graph.addEdge(e5);

    // Check adjacency list for vertex 0
    auto adj0 = graph.adj(0);
    assert(adj0.size() == 2);
    bool edge01Found = false;
    bool edge02Found = false;
    for (const auto& edge : adj0) {
        if (edge == e1) edge01Found = true;
        if (edge == e2) edge02Found = true;
    }
    assert(edge01Found);
    assert(edge02Found);

    // Check adjacency list for vertex 3
    auto adj3 = graph.adj(3);
    assert(adj3.size() == 3);
    bool edge13Found = false;
    bool edge23Found = false;
    bool edge34Found = false;
    for (const auto& edge : adj3) {
        if (edge == e3) edge13Found = true;
        if (edge == e4) edge23Found = true;
        if (edge == e5) edge34Found = true;
    }
    assert(edge13Found);
    assert(edge23Found);
    assert(edge34Found);

    // Check vertex count
    assert(graph.V() == 5);

    std::cout << "All tests passed for EdgeWeightedGraph!\n";
}

int main() {
    testEdgeWeightedGraph();
    return 0;
}
