#include <cassert>
#include <iostream>

#include "SCC.hpp"

void testSCC() {
    // Create a digraph
    Digraph g(8);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 3);
    g.addEdge(6, 7);
    g.addEdge(7, 6);
    g.addEdge(7, 0);

    // Instantiate SCC
    SCC scc(g);

    // Test the number of strongly connected components
    assert(scc.count() == 3);

    // Test strong connectivity within components
    assert(scc.stronglyConnected(0, 2)); // 0 and 2 are in the same SCC
    assert(scc.stronglyConnected(3, 5)); // 3 and 5 are in the same SCC
    assert(scc.stronglyConnected(6, 7)); // 6 and 7 are in the same SCC

    // Test strong connectivity between different components
    assert(!scc.stronglyConnected(0, 3)); // 0 and 3 are in different SCCs
    assert(!scc.stronglyConnected(4, 6)); // 4 and 6 are in different SCCs

    // Test component IDs
    int id0 = scc.id(0);
    int id3 = scc.id(3);
    int id6 = scc.id(6);

    assert(id0 != id3);
    assert(id3 != id6);
    assert(id0 != id6);

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    testSCC();
    return 0;
}