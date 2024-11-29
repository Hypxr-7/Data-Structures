#include <iostream>
#include <cassert>
#include "Graph.hpp"
#include "DepthFirstSearch.hpp"

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

    DepthFirstSearch dfs(graph, 0);

    assert(dfs.getCount() == 5);
    assert(dfs.isMarked(1));
    assert(dfs.isMarked(2));
    assert(dfs.isMarked(3));
    assert(dfs.isMarked(4));
    assert(dfs.isMarked(0));
}
