
#include <cassert>
#include <iostream>

#include "../data_structures/EulerianPath.hpp"
#include "../data_structures/Graph.hpp"

void testEulerianPath() {
    // Test 1: Empty graph (has degenerate Eulerian path)
    Graph g1(3);
    EulerianPath ep1(g1);
    assert(ep1.hasEulerianPath());
    assert(ep1.path().size() == 1);

    // Test 2: Single edge (has Eulerian path)
    Graph g2(2);
    g2.addEdge(0, 1);
    EulerianPath ep2(g2);
    assert(ep2.hasEulerianPath());
    assert(ep2.path().size() == 2);

    // Test 3: Path with 3 vertices (has Eulerian path)
    Graph g3(3);
    g3.addEdge(0, 1);
    g3.addEdge(1, 2);
    EulerianPath ep3(g3);
    assert(ep3.hasEulerianPath());
    assert(ep3.path().size() == 3);

    // Test 4: Triangle (Eulerian cycle - also has Eulerian path)
    Graph g4(3);
    g4.addEdge(0, 1);
    g4.addEdge(1, 2);
    g4.addEdge(2, 0);
    EulerianPath ep4(g4);
    assert(ep4.hasEulerianPath());
    assert(ep4.path().size() == 4);

    // Test 5:
    Graph g5(4);
    g5.addEdge(0, 1);
    g5.addEdge(0, 2);
    g5.addEdge(1, 3);
    EulerianPath ep5(g5);
    assert(ep5.hasEulerianPath());
    assert(ep5.path().size() == 4);

    // Test 6: Disconnected graph (no Eulerian path)
    Graph g6(4);
    g6.addEdge(0, 1);
    g6.addEdge(2, 3);
    EulerianPath ep6(g6);
    assert(!ep6.hasEulerianPath());
    assert(ep6.path().empty());

    // Test 7: Self-loop (has Eulerian path)
    Graph g7(1);
    g7.addEdge(0, 0);
    EulerianPath ep7(g7);
    assert(ep7.hasEulerianPath());
    assert(ep7.path().size() == 2);

    // Test 8: Path with multiple edges (has Eulerian path)
    Graph g8(4);
    g8.addEdge(0, 1);
    g8.addEdge(1, 2);
    g8.addEdge(2, 3);
    g8.addEdge(1, 3); // Creates exactly 2 odd-degree vertices
    EulerianPath ep8(g8);
    assert(ep8.hasEulerianPath());
    assert(ep8.path().size() == 5);

    // Test 9: Star graph (no Eulerian path - center has odd degree > 2)
    Graph g9(4);
    g9.addEdge(0, 1);
    g9.addEdge(0, 2);
    g9.addEdge(0, 3);
    EulerianPath ep9(g9);
    assert(!ep9.hasEulerianPath());
    assert(ep9.path().empty());

    // Test 10: Bridge graph (has Eulerian path)
    Graph g10(4);
    g10.addEdge(0, 1);
    g10.addEdge(1, 2);
    g10.addEdge(2, 3);
    EulerianPath ep10(g10);
    assert(ep10.hasEulerianPath());
    assert(ep10.path().size() == 4);

    std::cout << "All EulerianPath tests passed!" << std::endl;
}

int main() {
    testEulerianPath();
    return 0;
}
