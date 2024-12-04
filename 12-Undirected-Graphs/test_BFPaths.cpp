#include <cassert>
#include <iostream>
#include "BreadthFirstPaths.hpp"
#include "Graph.hpp"

void test();

int main() {
    test();
    std::cout << "All tests passed!\n";

    return 0;
}

void test() {
    Graph graph(5);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 3);
    graph.addEdge(1, 4);

    BreadthFirstPaths bfp(graph, 0);

    assert((bfp.pathTo(4) == std::vector{0, 1, 4}));
    assert((bfp.pathTo(2) == std::vector{0, 2}));
}
