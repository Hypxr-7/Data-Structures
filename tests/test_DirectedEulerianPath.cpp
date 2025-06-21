#include <cassert>
#include <iostream>

#include "../data_structures/Digraph.hpp"
#include "../data_structures/DirectedEulerianPath.hpp"

void testSimpleDirectedPath() {
    // Create directed path: 0->1->2
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);

    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == true);

    std::vector<int> path = dep.path();
    assert(path.size() == 3);  // 2 edges + 1
    assert(path[0] == 0);      // Should start from vertex with outdegree > indegree
    assert(path[1] == 1);
    assert(path[2] == 2);

    std::cout << "testSimpleDirectedPath passed\n";
}

void testDirectedCycleAsPath() {
    // Directed cycle: 0->1->2->0 (cycle is also a valid path)
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == true);

    std::vector<int> path = dep.path();
    assert(path.size() == 4);  // 3 edges + 1
    // Since all vertices have equal in/out degree, can start from any vertex

    std::cout << "testDirectedCycleAsPath passed\n";
}

void testSelfLoop() {
    // Single vertex with self-loop: 0->0
    Digraph g(1);
    g.addEdge(0, 0);

    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == true);

    std::vector<int> path = dep.path();
    assert(path.size() == 2);  // 1 edge + 1
    assert(path[0] == 0);
    assert(path[1] == 0);

    std::cout << "testSelfLoop passed\n";
}

void testSingleEdge() {
    // Single edge: 0->1
    Digraph g(2);
    g.addEdge(0, 1);

    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == true);

    std::vector<int> path = dep.path();
    assert(path.size() == 2);  // 1 edge + 1
    assert(path[0] == 0);      // Start vertex (outdegree=1, indegree=0)
    assert(path[1] == 1);      // End vertex (outdegree=0, indegree=1)

    std::cout << "testSingleEdge passed\n";
}

void testEmptyDigraph() {
    // Empty digraph (no edges)
    Digraph g(3);

    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == true);  // Empty graph has trivial path

    std::vector<int> path = dep.path();
    assert(path.size() == 1);  // Just the start vertex

    std::cout << "testEmptyDigraph passed\n";
}

void testNoEulerianPathTooManyStarts() {
    // Two vertices with outdegree > indegree: 0->2, 1->2
    Digraph g(3);
    g.addEdge(0, 2);  // Vertex 0: out=1, in=0
    g.addEdge(1, 2);  // Vertex 1: out=1, in=0 (two start vertices!)

    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == false);  // Deficit = 2 > 1
    assert(dep.path().empty());

    std::cout << "testNoEulerianPathTooManyStarts passed\n";
}

void testComplexEulerianPath() {
    // More complex path: 0->1->2->3, 0->2, 1->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(0, 2);  // Additional edge from 0
    g.addEdge(1, 3);  // Additional edge from 1

    // Degrees: 0(out=2,in=0), 1(out=2,in=1), 2(out=1,in=2), 3(out=0,in=2)
    // Only vertex 0 has outdegree > indegree by 2, but deficit = 2 > 1
    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == false);

    std::cout << "testComplexEulerianPath passed\n";
}

void testValidComplexPath() {
    // Valid complex path: 0->1->2->3, 1->0
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(1, 0);  // Return edge to balance degrees

    // Degrees: 0(out=1,in=1), 1(out=2,in=1), 2(out=1,in=1), 3(out=0,in=1)
    // Only vertex 1 has deficit=1, vertex 3 has surplus=1
    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == true);

    std::vector<int> path = dep.path();
    assert(path.size() == 5);  // 4 edges + 1

    std::cout << "testValidComplexPath passed\n";
}

void testDisconnectedDigraph() {
    // Disconnected: 0->1 and 2->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(2, 3);

    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == false);  // Not connected
    assert(dep.path().empty());

    std::cout << "testDisconnectedDigraph passed\n";
}

void testBranchedPath() {
    // Branched structure: 0->1, 0->2, 1->3, 2->3
    Digraph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);

    // Degrees: 0(out=2,in=0), 1(out=1,in=1), 2(out=1,in=1), 3(out=0,in=2)
    // Vertex 0 has deficit=2, vertex 3 has surplus=2, but total deficit=2 > 1
    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == false);

    std::cout << "testBranchedPath passed\n";
}

void testValidBranchedPath() {
    // Valid branched path: 0->1->3, 0->2->3, 3->4
    Digraph g(5);
    g.addEdge(0, 1);
    g.addEdge(1, 3);
    g.addEdge(0, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);

    // Degrees: 0(out=2,in=0), 1(out=1,in=1), 2(out=1,in=1), 3(out=1,in=2), 4(out=0,in=1)
    // Only vertex 0 has deficit=2, but wait... let me recalculate
    // Actually vertex 0: deficit=2, vertex 3: surplus=1, vertex 4: surplus=1
    // Total deficit = 2 > 1, so should be false
    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == false);

    std::cout << "testValidBranchedPath passed\n";
}

void testLinearPath() {
    // Linear path: 0->1->2->3->4
    Digraph g(5);
    for (int i = 0; i < 4; i++) {
        g.addEdge(i, i + 1);
    }

    // Degrees: 0(out=1,in=0), 1-3(out=1,in=1), 4(out=0,in=1)
    // Only vertex 0 has deficit=1
    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == true);

    std::vector<int> path = dep.path();
    assert(path.size() == 5);  // 4 edges + 1
    assert(path[0] == 0);      // Should start from deficit vertex
    assert(path[4] == 4);      // Should end at surplus vertex

    std::cout << "testLinearPath passed\n";
}

void testMultipleSelfLoops() {
    // Multiple self-loops: 0->0, 0->0
    Digraph g(1);
    g.addEdge(0, 0);
    g.addEdge(0, 0);

    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == true);

    std::vector<int> path = dep.path();
    assert(path.size() == 3);  // 2 edges + 1

    std::cout << "testMultipleSelfLoops passed\n";
}

void testMixedStructure() {
    // Mixed: cycle + additional path: 0->1->0, 0->2
    Digraph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 0);
    g.addEdge(0, 2);

    // Degrees: 0(out=2,in=1), 1(out=1,in=1), 2(out=0,in=1)
    // Only vertex 0 has deficit=1
    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == true);

    std::vector<int> path = dep.path();
    assert(path.size() == 4);  // 3 edges + 1

    std::cout << "testMixedStructure passed\n";
}

void testParallelEdges() {
    // Parallel edges: 0->1, 0->1
    Digraph g(2);
    g.addEdge(0, 1);
    g.addEdge(0, 1);

    // Degrees: 0(out=2,in=0), 1(out=0,in=2)
    // Vertex 0 has deficit=2, but this exceeds 1
    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == false);

    std::cout << "testParallelEdges passed\n";
}

void testValidParallelEdges() {
    // Valid parallel structure: 0->1, 1->0, 0->1
    Digraph g(2);
    g.addEdge(0, 1);
    g.addEdge(1, 0);
    g.addEdge(0, 1);

    // Degrees: 0(out=2,in=1), 1(out=1,in=2)
    // Vertex 0 has deficit=1, vertex 1 has surplus=1
    DirectedEulerianPath dep(g);

    assert(dep.hasEulerianPath() == true);

    std::vector<int> path = dep.path();
    assert(path.size() == 4);  // 3 edges + 1

    std::cout << "testValidParallelEdges passed\n";
}

int main() {
    std::cout << "Running DirectedEulerianPath tests...\n";

    testSimpleDirectedPath();
    testDirectedCycleAsPath();
    testSelfLoop();
    testSingleEdge();
    testEmptyDigraph();
    testNoEulerianPathTooManyStarts();
    testComplexEulerianPath();
    testValidComplexPath();
    testDisconnectedDigraph();
    testBranchedPath();
    testValidBranchedPath();
    testLinearPath();
    testMultipleSelfLoops();
    testMixedStructure();
    testParallelEdges();
    testValidParallelEdges();

    std::cout << "All tests passed!\n";
    return 0;
}