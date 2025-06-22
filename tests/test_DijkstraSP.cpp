#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

#include "../data_structures/DijkstraSP.hpp"
#include "../data_structures/DirectedEdge.hpp"
#include "../data_structures/EdgeWeightedDigraph.hpp"

const double EPS = 1e-9;

// Helper function to check if two doubles are approximately equal
bool isEqual(double a, double b) {
    return std::abs(a - b) < EPS;
}

void testBasicConstruction() {
    // Test construction with simple graph
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 5.0));
    g.addEdge(DirectedEdge(1, 2, 3.0));

    DijkstraSP sp(g, 0);

    assert(sp.V() == 3);
    assert(sp.source() == 0);
    assert(isEqual(sp.distTo(0), 0.0));

    std::cout << "testBasicConstruction passed\n";
}

void testInvalidSourceVertex() {
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 1.0));

    // Test negative source vertex
    try {
        DijkstraSP sp(g, -1);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Source vertex -1 is out of range") != std::string::npos);
    }

    // Test source vertex out of range
    try {
        DijkstraSP sp(g, 3);  // Valid range is [0, 2]
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Source vertex 3 is out of range") != std::string::npos);
    }

    std::cout << "testInvalidSourceVertex passed\n";
}

void testNegativeWeights() {
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 5.0));
    g.addEdge(DirectedEdge(1, 2, -2.0));  // Negative weight

    try {
        DijkstraSP sp(g, 0);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        // assert(msg.find("has negative weight") != std::string::npos);
        // assert(msg.find("Use Bellman-Ford") != std::string::npos);
    }

    std::cout << "testNegativeWeights passed\n";
}

void testSingleVertex() {
    EdgeWeightedDigraph g(1);
    DijkstraSP sp(g, 0);

    assert(sp.V() == 1);
    assert(sp.source() == 0);
    assert(isEqual(sp.distTo(0), 0.0));
    assert(sp.hasPathTo(0));

    auto path = sp.pathTo(0);
    assert(path.empty());  // Path to self should be empty

    std::cout << "testSingleVertex passed\n";
}

void testSelfLoop() {
    EdgeWeightedDigraph g(2);
    g.addEdge(DirectedEdge(0, 0, 3.0));  // Self-loop
    g.addEdge(DirectedEdge(0, 1, 5.0));

    DijkstraSP sp(g, 0);

    assert(isEqual(sp.distTo(0), 0.0));  // Distance to self is 0, not via self-loop
    assert(isEqual(sp.distTo(1), 5.0));
    assert(sp.hasPathTo(0));
    assert(sp.hasPathTo(1));

    std::cout << "testSelfLoop passed\n";
}

void testDisconnectedGraph() {
    EdgeWeightedDigraph g(4);
    g.addEdge(DirectedEdge(0, 1, 2.0));
    // Vertices 2 and 3 are disconnected from 0 and 1
    g.addEdge(DirectedEdge(2, 3, 1.0));

    DijkstraSP sp(g, 0);

    assert(isEqual(sp.distTo(0), 0.0));
    assert(isEqual(sp.distTo(1), 2.0));
    assert(sp.distTo(2) == std::numeric_limits<double>::max());
    assert(sp.distTo(3) == std::numeric_limits<double>::max());

    assert(sp.hasPathTo(0));
    assert(sp.hasPathTo(1));
    assert(!sp.hasPathTo(2));
    assert(!sp.hasPathTo(3));

    auto pathTo2 = sp.pathTo(2);
    assert(pathTo2.empty());

    std::cout << "testDisconnectedGraph passed\n";
}

void testSimpleShortestPath() {
    EdgeWeightedDigraph g(4);
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(0, 2, 4.0));
    g.addEdge(DirectedEdge(1, 2, 2.0));
    g.addEdge(DirectedEdge(1, 3, 6.0));
    g.addEdge(DirectedEdge(2, 3, 3.0));

    DijkstraSP sp(g, 0);

    // Check distances
    assert(isEqual(sp.distTo(0), 0.0));
    assert(isEqual(sp.distTo(1), 1.0));
    assert(isEqual(sp.distTo(2), 3.0));  // 0->1->2 (1+2=3) is shorter than 0->2 (4)
    assert(isEqual(sp.distTo(3), 6.0));  // 0->1->2->3 (1+2+3=6)

    // Check paths exist
    for (int v = 0; v < 4; ++v) {
        assert(sp.hasPathTo(v));
    }

    // Check specific path to vertex 3
    auto pathTo3 = sp.pathTo(3);
    assert(pathTo3.size() == 3);
    assert(pathTo3[0].from() == 0 && pathTo3[0].to() == 1);
    assert(pathTo3[1].from() == 1 && pathTo3[1].to() == 2);
    assert(pathTo3[2].from() == 2 && pathTo3[2].to() == 3);

    std::cout << "testSimpleShortestPath passed\n";
}

void testPathReconstruction() {
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 2.0));
    g.addEdge(DirectedEdge(1, 2, 3.0));

    DijkstraSP sp(g, 0);

    // Test path to vertex 2
    auto path = sp.pathTo(2);
    assert(path.size() == 2);

    // Verify path is correct: 0->1->2
    assert(path[0].from() == 0 && path[0].to() == 1 && isEqual(path[0].weight(), 2.0));
    assert(path[1].from() == 1 && path[1].to() == 2 && isEqual(path[1].weight(), 3.0));

    // Verify path weights sum to distance
    double pathWeight = 0.0;
    for (const auto& edge : path) {
        pathWeight += edge.weight();
    }
    assert(isEqual(pathWeight, sp.distTo(2)));

    std::cout << "testPathReconstruction passed\n";
}

void testInvalidVertexQueries() {
    EdgeWeightedDigraph g(3);
    DijkstraSP sp(g, 0);

    // Test distTo with invalid vertex
    try {
        sp.distTo(-1);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex -1 is out of range") != std::string::npos);
    }

    try {
        sp.distTo(3);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex 3 is out of range") != std::string::npos);
    }

    // Test hasPathTo with invalid vertex
    try {
        sp.hasPathTo(-1);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex -1 is out of range") != std::string::npos);
    }

    // Test pathTo with invalid vertex
    try {
        sp.pathTo(5);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex 5 is out of range") != std::string::npos);
    }

    std::cout << "testInvalidVertexQueries passed\n";
}

void testParallelEdges() {
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 5.0));
    g.addEdge(DirectedEdge(0, 1, 2.0));  // Parallel edge with lower weight
    g.addEdge(DirectedEdge(1, 2, 3.0));

    DijkstraSP sp(g, 0);

    // Should choose the lighter parallel edge
    assert(isEqual(sp.distTo(1), 2.0));  // Not 5.0
    assert(isEqual(sp.distTo(2), 5.0));  // 2.0 + 3.0

    auto pathTo1 = sp.pathTo(1);
    assert(pathTo1.size() == 1);
    assert(isEqual(pathTo1[0].weight(), 2.0));  // Should use lighter edge

    std::cout << "testParallelEdges passed\n";
}

void testZeroWeights() {
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 0.0));
    g.addEdge(DirectedEdge(1, 2, 0.0));

    DijkstraSP sp(g, 0);

    assert(isEqual(sp.distTo(0), 0.0));
    assert(isEqual(sp.distTo(1), 0.0));
    assert(isEqual(sp.distTo(2), 0.0));

    auto pathTo2 = sp.pathTo(2);
    assert(pathTo2.size() == 2);

    std::cout << "testZeroWeights passed\n";
}

void testTriangleInequality() {
    // Test that Dijkstra respects triangle inequality
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(1, 2, 1.0));
    g.addEdge(DirectedEdge(0, 2, 3.0));  // Direct path is longer

    DijkstraSP sp(g, 0);

    // Should choose shorter path 0->1->2 (cost 2) over direct 0->2 (cost 3)
    assert(isEqual(sp.distTo(2), 2.0));

    auto pathTo2 = sp.pathTo(2);
    assert(pathTo2.size() == 2);

    std::cout << "testTriangleInequality passed\n";
}

void testCompleteGraph() {
    // Complete graph where every vertex connects to every other vertex
    const int n = 4;
    EdgeWeightedDigraph g(n);

    // Add edges with weights equal to vertex distance
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                g.addEdge(DirectedEdge(i, j, std::abs(i - j)));
            }
        }
    }

    DijkstraSP sp(g, 0);

    // In this case, direct edges should be shortest
    for (int v = 0; v < n; ++v) {
        assert(isEqual(sp.distTo(v), std::abs(0 - v)));
        assert(sp.hasPathTo(v));

        if (v != 0) {
            auto path = sp.pathTo(v);
            assert(path.size() == 1);  // Direct edge should be shortest
        }
    }

    std::cout << "testCompleteGraph passed\n";
}

void testLargeGraph() {
    // Test with larger graph - chain of vertices
    const int n = 100;
    EdgeWeightedDigraph g(n);

    // Create chain: 0->1->2->...->99
    for (int i = 0; i < n - 1; ++i) {
        g.addEdge(DirectedEdge(i, i + 1, 1.0));
    }

    DijkstraSP sp(g, 0);

    // Distance to vertex i should be i
    for (int v = 0; v < n; ++v) {
        assert(isEqual(sp.distTo(v), static_cast<double>(v)));
        assert(sp.hasPathTo(v));

        auto path = sp.pathTo(v);
        assert(path.size() == v);  // Path length should equal vertex number
    }

    std::cout << "testLargeGraph passed\n";
}

void testStarGraph() {
    // Star graph: center vertex connected to all others
    const int n = 6;  // 1 center + 5 outer vertices
    EdgeWeightedDigraph g(n);

    // Center vertex 0 connects to all others
    for (int i = 1; i < n; ++i) {
        g.addEdge(DirectedEdge(0, i, static_cast<double>(i)));
    }

    DijkstraSP sp(g, 0);

    // Distance from center to vertex i should be i
    for (int v = 0; v < n; ++v) {
        if (v == 0) {
            assert(isEqual(sp.distTo(v), 0.0));
        } else {
            assert(isEqual(sp.distTo(v), static_cast<double>(v)));
        }
        assert(sp.hasPathTo(v));
    }

    // Test from outer vertex (should only reach that vertex)
    DijkstraSP spFromOuter(g, 1);
    assert(isEqual(spFromOuter.distTo(1), 0.0));
    for (int v = 0; v < n; ++v) {
        if (v != 1) {
            assert(!spFromOuter.hasPathTo(v));
        }
    }

    std::cout << "testStarGraph passed\n";
}

void testFloatingPointPrecision() {
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 0.1));
    g.addEdge(DirectedEdge(1, 2, 0.2));
    g.addEdge(DirectedEdge(0, 2, 0.4));  // Slightly longer direct path

    DijkstraSP sp(g, 0);

    // Should choose path 0->1->2 (0.3) over direct 0->2 (0.4)
    assert(isEqual(sp.distTo(2), 0.3));

    auto pathTo2 = sp.pathTo(2);
    assert(pathTo2.size() == 2);

    std::cout << "testFloatingPointPrecision passed\n";
}

void testSourceVertexProperties() {
    EdgeWeightedDigraph g(5);
    g.addEdge(DirectedEdge(2, 1, 3.0));
    g.addEdge(DirectedEdge(2, 3, 1.0));

    DijkstraSP sp(g, 2);  // Source is vertex 2

    assert(sp.source() == 2);
    assert(isEqual(sp.distTo(2), 0.0));  // Distance to source is always 0
    assert(sp.hasPathTo(2));

    auto pathToSource = sp.pathTo(2);
    assert(pathToSource.empty());  // Path to source should be empty

    std::cout << "testSourceVertexProperties passed\n";
}

void testMultiplePathsSameWeight() {
    // Graph where multiple paths have same weight
    EdgeWeightedDigraph g(4);
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(0, 2, 1.0));
    g.addEdge(DirectedEdge(1, 3, 1.0));
    g.addEdge(DirectedEdge(2, 3, 1.0));

    DijkstraSP sp(g, 0);

    // Both paths 0->1->3 and 0->2->3 have weight 2
    assert(isEqual(sp.distTo(3), 2.0));

    auto pathTo3 = sp.pathTo(3);
    assert(pathTo3.size() == 2);

    // Verify path weight sums correctly
    double totalWeight = 0.0;
    for (const auto& edge : pathTo3) {
        totalWeight += edge.weight();
    }
    assert(isEqual(totalWeight, 2.0));

    std::cout << "testMultiplePathsSameWeight passed\n";
}

void testEmptyGraph() {
    EdgeWeightedDigraph g(3);  // No edges
    DijkstraSP sp(g, 1);

    assert(sp.source() == 1);
    assert(isEqual(sp.distTo(1), 0.0));
    assert(sp.hasPathTo(1));

    // Other vertices should be unreachable
    assert(!sp.hasPathTo(0));
    assert(!sp.hasPathTo(2));
    assert(sp.distTo(0) == std::numeric_limits<double>::max());
    assert(sp.distTo(2) == std::numeric_limits<double>::max());

    std::cout << "testEmptyGraph passed\n";
}

void testCyclicGraph() {
    // Graph with cycles
    EdgeWeightedDigraph g(4);
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(1, 2, 1.0));
    g.addEdge(DirectedEdge(2, 0, 1.0));  // Creates cycle 0->1->2->0
    g.addEdge(DirectedEdge(1, 3, 5.0));
    g.addEdge(DirectedEdge(2, 3, 2.0));  // Shorter path to 3

    DijkstraSP sp(g, 0);

    assert(isEqual(sp.distTo(0), 0.0));
    assert(isEqual(sp.distTo(1), 1.0));
    assert(isEqual(sp.distTo(2), 2.0));
    assert(isEqual(sp.distTo(3), 4.0));  // 0->1->2->3 (1+1+2=4) vs 0->1->3 (1+5=6)

    // Verify shortest path to 3 goes through vertex 2
    auto pathTo3 = sp.pathTo(3);
    assert(pathTo3.size() == 3);
    assert(pathTo3[2].from() == 2 && pathTo3[2].to() == 3);

    std::cout << "testCyclicGraph passed\n";
}

void testPathConsistency() {
    // Test that path reconstruction is consistent with distances
    EdgeWeightedDigraph g(5);
    g.addEdge(DirectedEdge(0, 1, 2.0));
    g.addEdge(DirectedEdge(0, 2, 1.0));
    g.addEdge(DirectedEdge(1, 3, 3.0));
    g.addEdge(DirectedEdge(2, 3, 1.0));
    g.addEdge(DirectedEdge(3, 4, 2.0));

    DijkstraSP sp(g, 0);

    // For each reachable vertex, verify path weight equals distance
    for (int v = 0; v < g.V(); ++v) {
        if (sp.hasPathTo(v)) {
            auto path = sp.pathTo(v);
            double pathWeight = 0.0;

            for (const auto& edge : path) {
                pathWeight += edge.weight();
            }

            assert(isEqual(pathWeight, sp.distTo(v)));

            // Verify path connectivity
            if (!path.empty()) {
                assert(path[0].from() == sp.source());  // Path starts from source

                for (size_t i = 1; i < path.size(); ++i) {
                    assert(path[i-1].to() == path[i].from());  // Edges are connected
                }

                assert(path.back().to() == v);  // Path ends at destination
            }
        }
    }

    std::cout << "testPathConsistency passed\n";
}

void testLargeWeights() {
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 1e6));
    g.addEdge(DirectedEdge(1, 2, 1e6));

    DijkstraSP sp(g, 0);

    assert(isEqual(sp.distTo(0), 0.0));
    assert(isEqual(sp.distTo(1), 1e6));
    assert(isEqual(sp.distTo(2), 2e6));

    std::cout << "testLargeWeights passed\n";
}

void testPerformanceWithDenseGraph() {
    // Test performance with moderately dense graph
    const int n = 50;
    EdgeWeightedDigraph g(n);

    // Add many edges
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j && (i + j) % 3 == 0) {  // Add subset of possible edges
                g.addEdge(DirectedEdge(i, j, std::abs(i - j) + 1.0));
            }
        }
    }

    DijkstraSP sp(g, 0);

    // Verify basic properties
    assert(sp.source() == 0);
    assert(isEqual(sp.distTo(0), 0.0));

    // Count reachable vertices
    int reachableCount = 0;
    for (int v = 0; v < n; ++v) {
        if (sp.hasPathTo(v)) {
            reachableCount++;
            // Verify path consistency for a few vertices
            if (v < 10) {
                auto path = sp.pathTo(v);
                double pathWeight = 0.0;
                for (const auto& edge : path) {
                    pathWeight += edge.weight();
                }
                assert(isEqual(pathWeight, sp.distTo(v)));
            }
        }
    }

    assert(reachableCount >= 1);  // At least source should be reachable

    std::cout << "testPerformanceWithDenseGraph passed\n";
}

int main() {
    std::cout << "Running DijkstraSP tests...\n";

    testBasicConstruction();
    testInvalidSourceVertex();
    testNegativeWeights();
    testSingleVertex();
    testSelfLoop();
    testDisconnectedGraph();
    testSimpleShortestPath();
    testPathReconstruction();
    testInvalidVertexQueries();
    testParallelEdges();
    testZeroWeights();
    testTriangleInequality();
    testCompleteGraph();
    testLargeGraph();
    testStarGraph();
    testFloatingPointPrecision();
    testSourceVertexProperties();
    testMultiplePathsSameWeight();
    testEmptyGraph();
    testCyclicGraph();
    testPathConsistency();
    testLargeWeights();
    testPerformanceWithDenseGraph();

    std::cout << "All tests passed!\n";
    return 0;
}
