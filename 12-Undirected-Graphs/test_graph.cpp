#include <algorithm>
#include <iostream>
#include <cassert>
#include "Graph.hpp"

void test();

int main() {
    test();
    std::cout << "All tests passed!\n";

    return 0;
}

void test() {
    Graph graph(5);

    assert(graph.V() == 5);
    assert(graph.E() == 0);

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 3);

    assert(graph.E() == 3);

    std::cout << graph.toString();

    assert((graph.adj(0) == std::vector{3, 2, 1}));

    assert(graph.degree(0) == 3);
    assert(graph.degree(4) == 0);

    assert(graph.maxDegree() == 3);
    assert(graph.minDegree() == 0);
    assert(graph.avgDegree() == 1);

    assert(graph.numberOfSelfLoops() == 0);
    graph.addEdge(4, 4);
    assert(graph.numberOfSelfLoops() == 2);
}
