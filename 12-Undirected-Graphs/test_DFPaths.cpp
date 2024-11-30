#include <cassert>
#include <iostream>
#include "DepthFirstPaths.hpp"

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

    DepthFirstPaths dfp(graph, 0);

    assert((dfp.pathTo(4) == std::vector{0, 1, 4}));
    assert((dfp.pathTo(2) == std::vector{0, 2}));
}
