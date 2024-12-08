#include <cassert>
#include <iostream>0
#include <stdexcept>

#include "ConnectedComponents.hpp"
#include "Graph.hpp"

void testConnectedComponents() {
    // Create a graph with 6 vertices and 5 edges
    Graph graph(6);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(3, 4);
    graph.addEdge(4, 5);

    // Create ConnectedComponents object
    ConnectedComponents cc(graph);

    // Test the number of connected components
    assert(cc.getCount() == 2);

    // Test the component id of each vertex
    assert(cc.getID(0) == cc.getID(1));
    assert(cc.getID(0) == cc.getID(2));
    assert(cc.getID(3) == cc.getID(4));
    assert(cc.getID(3) == cc.getID(5));
    assert(cc.getID(0) != cc.getID(3));

    // Test the size of each component
    assert(cc.getSize(0) == 3);
    assert(cc.getSize(3) == 3);

    // Test if vertices are connected
    assert(cc.connected(0, 1));
    assert(cc.connected(0, 2));
    assert(cc.connected(3, 4));
    assert(cc.connected(3, 5));
    assert(!cc.connected(0, 3));
    assert(!cc.connected(1, 4));

    // Test validateVertex method (indirectly through public methods)
    try {
        cc.getID(6);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(true); // Expected exception
    }

    try {
        cc.getSize(6);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(true); // Expected exception
    }

    try {
        cc.connected(0, 6);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(true); // Expected exception
    }

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    testConnectedComponents();
    return 0;
}