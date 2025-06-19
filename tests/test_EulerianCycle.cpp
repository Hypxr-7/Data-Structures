#include <cassert>
#include <iostream>

#include "../data_structures/EulerianCycle.hpp"
#include "../data_structures/Graph.hpp"

void testEulerianCycle() {
    // Test 1: Empty graph (no edges)
    Graph g1(3);
    EulerianCycle ec1(g1);
    assert(!ec1.hasEulerianCycle());
    assert(ec1.cycle().empty());

    // Test 2: Single vertex with self-loop
    Graph g2(1);
    g2.addEdge(0, 0);
    EulerianCycle ec2(g2);
    assert(ec2.hasEulerianCycle());
    assert(ec2.cycle().size() == 2);

    // Test 3: Triangle (3-cycle)
    Graph g3(3);
    g3.addEdge(0, 1);
    g3.addEdge(1, 2);
    g3.addEdge(2, 0);
    EulerianCycle ec3(g3);
    assert(ec3.hasEulerianCycle());
    assert(ec3.cycle().size() == 4);

    // Test 4: Square (4-cycle)
    Graph g4(4);
    g4.addEdge(0, 1);
    g4.addEdge(1, 2);
    g4.addEdge(2, 3);
    g4.addEdge(3, 0);
    EulerianCycle ec4(g4);
    assert(ec4.hasEulerianCycle());
    assert(ec4.cycle().size() == 5);

    // Test 5: Path (no Eulerian cycle - odd degree vertices)
    Graph g5(3);
    g5.addEdge(0, 1);
    g5.addEdge(1, 2);
    EulerianCycle ec5(g5);
    assert(!ec5.hasEulerianCycle());
    assert(ec5.cycle().empty());

    // Test 6: Disconnected graph
    Graph g6(4);
    g6.addEdge(0, 1);
    g6.addEdge(1, 0); // Double edge to make degrees even
    g6.addEdge(2, 3);
    g6.addEdge(3, 2); // Double edge to make degrees even
    EulerianCycle ec6(g6);
    assert(!ec6.hasEulerianCycle()); // Disconnected

    // Test 7: Complete graph K4 (has Eulerian cycle)
    Graph g7(5);
    g7.addEdge(0, 1);
    g7.addEdge(0, 2);
    g7.addEdge(0, 3);
    g7.addEdge(0, 4);
    g7.addEdge(1, 2);
    g7.addEdge(1, 3);
    g7.addEdge(1, 4);
    g7.addEdge(2, 3);
    g7.addEdge(2, 4);
    g7.addEdge(3, 4);
    EulerianCycle ec7(g7);
    assert(ec7.hasEulerianCycle());
    assert(ec7.cycle().size() == 11);

    std::cout << "All EulerianCycle tests passed!" << std::endl;
}

int main() {
    testEulerianCycle();
    return 0;
}