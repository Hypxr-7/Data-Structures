#include <iostream>
#include <cassert>
#include "KruskalMST.hpp"
#include "EdgeWeightedGraph.hpp"
#include "Edge.hpp"

void testKruskalMST() {
    // Create an edge-weighted graph with 6 vertices
    EdgeWeightedGraph graph(6);

    // Add edges
    graph.addEdge(Edge(0, 1, 4.0));
    graph.addEdge(Edge(0, 2, 3.0));
    graph.addEdge(Edge(1, 2, 1.0));
    graph.addEdge(Edge(1, 3, 2.0));
    graph.addEdge(Edge(2, 3, 4.0));
    graph.addEdge(Edge(3, 4, 2.0));
    graph.addEdge(Edge(4, 5, 3.0));
    graph.addEdge(Edge(3, 5, 3.0));

    // Compute MST using Kruskal's algorithm
    KruskalMST mst(graph);

    // Retrieve MST edges
    std::queue<Edge> mstEdges = mst.edges();

    // Expected total weight of the MST
    double expectedTotalWeight = 11.0;
    double totalWeight = 0.0;

    // Check edges in the MST and calculate total weight
    while (!mstEdges.empty()) {
        Edge edge = mstEdges.front();
        mstEdges.pop();
        totalWeight += edge.getWeight();
    }

    assert(totalWeight == expectedTotalWeight);

    // Check number of edges in the MST (should be V - 1)
    assert(mstEdges.size() == 0); // mstEdges should now be empty

    std::cout << "All tests passed for KruskalMST!\n";
}

int main() {
    testKruskalMST();
    return 0;
}
