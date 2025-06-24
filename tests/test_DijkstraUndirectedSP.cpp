#include <cassert>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <set>
#include <algorithm>

#include "../data_structures/DijkstraUndirectedSP.hpp"
#include "../data_structures/Edge.hpp"
#include "../data_structures/EdgeWeightedGraph.hpp"

const double EPS = 1e-9;

// Helper function to check if two doubles are approximately equal
bool isEqual(double a, double b) {
    return std::abs(a - b) < EPS;
}

void testBasicConstruction() {
    // Test construction with simple undirected graph
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 5.0));
    g.addEdge(Edge(1, 2, 3.0));

    DijkstraUndirectedSP sp(g, 0);

    assert(isEqual(sp.distTo(0), 0.0));
    assert(sp.hasPathTo(0));
    assert(sp.hasPathTo(1));
    assert(sp.hasPathTo(2));

    std::cout << "testBasicConstruction passed\n";
}

void testInvalidSourceVertex() {
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 1.0));

    // Test negative source vertex
    try {
        DijkstraUndirectedSP sp(g, -1);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex out of range") != std::string::npos);
    }

    // Test source vertex out of range
    try {
        DijkstraUndirectedSP sp(g, 3);  // Valid range is [0, 2]
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex out of range") != std::string::npos);
    }

    std::cout << "testInvalidSourceVertex passed\n";
}

void testNegativeWeights() {
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 5.0));
    g.addEdge(Edge(1, 2, -2.0));  // Negative weight

    try {
        DijkstraUndirectedSP sp(g, 0);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Edge has negative weight") != std::string::npos);
    }

    std::cout << "testNegativeWeights passed\n";
}

void testSingleVertex() {
    EdgeWeightedGraph g(1);
    DijkstraUndirectedSP sp(g, 0);

    assert(isEqual(sp.distTo(0), 0.0));
    assert(sp.hasPathTo(0));

    auto path = sp.pathTo(0);
    assert(path.empty());  // Path to self should be empty

    std::cout << "testSingleVertex passed\n";
}

void testDisconnectedGraph() {
    EdgeWeightedGraph g(4);
    g.addEdge(Edge(0, 1, 2.0));
    // Vertices 2 and 3 are disconnected from 0 and 1
    g.addEdge(Edge(2, 3, 1.0));

    DijkstraUndirectedSP sp(g, 0);

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

void testUndirectedShortestPath() {
    // Test undirected graph where paths can be traversed in both directions
    EdgeWeightedGraph g(4);
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(0, 2, 4.0));
    g.addEdge(Edge(1, 2, 2.0));
    g.addEdge(Edge(1, 3, 6.0));
    g.addEdge(Edge(2, 3, 3.0));

    DijkstraUndirectedSP sp(g, 0);

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

    // Verify path weights sum to distance
    double pathWeight = 0.0;
    for (const auto& edge : pathTo3) {
        pathWeight += edge.weight();
    }
    assert(isEqual(pathWeight, sp.distTo(3)));

    std::cout << "testUndirectedShortestPath passed\n";
}

void testBidirectionalTraversal() {
    // Test that edges can be traversed in both directions
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 2.0));
    g.addEdge(Edge(1, 2, 3.0));

    // Test from vertex 0
    DijkstraUndirectedSP sp0(g, 0);
    assert(isEqual(sp0.distTo(2), 5.0));  // 0->1->2

    // Test from vertex 2 (should work in reverse direction)
    DijkstraUndirectedSP sp2(g, 2);
    assert(isEqual(sp2.distTo(0), 5.0));  // 2->1->0

    // Test from vertex 1 (should reach both endpoints)
    DijkstraUndirectedSP sp1(g, 1);
    assert(isEqual(sp1.distTo(0), 2.0));  // 1->0
    assert(isEqual(sp1.distTo(2), 3.0));  // 1->2

    std::cout << "testBidirectionalTraversal passed\n";
}

void testPathReconstruction() {
    EdgeWeightedGraph g(4);
    g.addEdge(Edge(0, 1, 2.0));
    g.addEdge(Edge(1, 2, 3.0));
    g.addEdge(Edge(2, 3, 1.0));

    DijkstraUndirectedSP sp(g, 0);

    // Test path to vertex 3
    auto path = sp.pathTo(3);
    assert(path.size() == 3);

    // Verify path connectivity and weights
    assert(path[0].either() == 0 || path[0].other(path[0].either()) == 0);  // First edge connects to source

    double pathWeight = 0.0;
    int currentVertex = 0;

    for (const auto& edge : path) {
        pathWeight += edge.weight();
        // Verify edge connects to current vertex
        assert(edge.either() == currentVertex || edge.other(edge.either()) == currentVertex);
        // Move to other endpoint
        currentVertex = edge.other(currentVertex);
    }

    assert(currentVertex == 3);  // Should end at destination
    assert(isEqual(pathWeight, sp.distTo(3)));

    std::cout << "testPathReconstruction passed\n";
}

void testInvalidVertexQueries() {
    EdgeWeightedGraph g(3);
    DijkstraUndirectedSP sp(g, 0);

    // Test distTo with invalid vertex
    try {
        sp.distTo(-1);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex out of range") != std::string::npos);
    }

    try {
        sp.distTo(3);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex out of range") != std::string::npos);
    }

    // Test hasPathTo with invalid vertex
    try {
        sp.hasPathTo(-1);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex out of range") != std::string::npos);
    }

    // Test pathTo with invalid vertex
    try {
        sp.pathTo(5);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex out of range") != std::string::npos);
    }

    std::cout << "testInvalidVertexQueries passed\n";
}

void testParallelEdges() {
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 5.0));
    g.addEdge(Edge(0, 1, 2.0));  // Parallel edge with lower weight
    g.addEdge(Edge(1, 2, 3.0));

    DijkstraUndirectedSP sp(g, 0);

    // Should choose the lighter parallel edge
    assert(isEqual(sp.distTo(1), 2.0));  // Not 5.0
    assert(isEqual(sp.distTo(2), 5.0));  // 2.0 + 3.0

    auto pathTo1 = sp.pathTo(1);
    assert(pathTo1.size() == 1);
    assert(isEqual(pathTo1[0].weight(), 2.0));  // Should use lighter edge

    std::cout << "testParallelEdges passed\n";
}

void testZeroWeights() {
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 0.0));
    g.addEdge(Edge(1, 2, 0.0));

    DijkstraUndirectedSP sp(g, 0);

    assert(isEqual(sp.distTo(0), 0.0));
    assert(isEqual(sp.distTo(1), 0.0));
    assert(isEqual(sp.distTo(2), 0.0));

    auto pathTo2 = sp.pathTo(2);
    assert(pathTo2.size() == 2);

    std::cout << "testZeroWeights passed\n";
}

void testTriangleGraph() {
    // Test triangle graph where all vertices are connected
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(1, 2, 1.0));
    g.addEdge(Edge(0, 2, 3.0));  // Direct path is longer

    DijkstraUndirectedSP sp(g, 0);

    // Should choose shorter path 0->1->2 (cost 2) over direct 0->2 (cost 3)
    assert(isEqual(sp.distTo(2), 2.0));

    auto pathTo2 = sp.pathTo(2);
    assert(pathTo2.size() == 2);

    std::cout << "testTriangleGraph passed\n";
}

void testCompleteGraph() {
    // Complete undirected graph where every vertex connects to every other vertex
    const int n = 4;
    EdgeWeightedGraph g(n);

    // Add edges with weights equal to vertex distance
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {  // Only add each edge once (undirected)
            g.addEdge(Edge(i, j, std::abs(i - j)));
        }
    }

    DijkstraUndirectedSP sp(g, 0);

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

void testStarGraph() {
    // Star graph: center vertex connected to all others
    const int n = 6;  // 1 center + 5 outer vertices
    EdgeWeightedGraph g(n);

    // Center vertex 0 connects to all others
    for (int i = 1; i < n; ++i) {
        g.addEdge(Edge(0, i, static_cast<double>(i)));
    }

    DijkstraUndirectedSP sp(g, 0);

    // Distance from center to vertex i should be i
    for (int v = 0; v < n; ++v) {
        if (v == 0) {
            assert(isEqual(sp.distTo(v), 0.0));
        } else {
            assert(isEqual(sp.distTo(v), static_cast<double>(v)));
        }
        assert(sp.hasPathTo(v));
    }

    // Test from outer vertex (can reach center and other vertices via center)
    DijkstraUndirectedSP spFromOuter(g, 1);
    assert(isEqual(spFromOuter.distTo(0), 1.0));  // Direct to center
    assert(isEqual(spFromOuter.distTo(2), 3.0));  // 1->0->2 (1+2=3)

    std::cout << "testStarGraph passed\n";
}

void testChainGraph() {
    // Chain graph: 0-1-2-3-4
    const int n = 5;
    EdgeWeightedGraph g(n);

    for (int i = 0; i < n - 1; ++i) {
        g.addEdge(Edge(i, i + 1, 1.0));
    }

    DijkstraUndirectedSP sp(g, 0);

    // Distance to vertex i should be i
    for (int v = 0; v < n; ++v) {
        assert(isEqual(sp.distTo(v), static_cast<double>(v)));
        assert(sp.hasPathTo(v));

        auto path = sp.pathTo(v);
        assert(path.size() == v);  // Path length should equal vertex number
    }

    // Test from middle vertex
    DijkstraUndirectedSP spFromMiddle(g, 2);
    assert(isEqual(spFromMiddle.distTo(0), 2.0));
    assert(isEqual(spFromMiddle.distTo(4), 2.0));

    std::cout << "testChainGraph passed\n";
}

void testFloatingPointPrecision() {
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 0.1));
    g.addEdge(Edge(1, 2, 0.2));
    g.addEdge(Edge(0, 2, 0.4));  // Slightly longer direct path

    DijkstraUndirectedSP sp(g, 0);

    // Should choose path 0->1->2 (0.3) over direct 0->2 (0.4)
    assert(isEqual(sp.distTo(2), 0.3));

    auto pathTo2 = sp.pathTo(2);
    assert(pathTo2.size() == 2);

    std::cout << "testFloatingPointPrecision passed\n";
}

void testEmptyGraph() {
    EdgeWeightedGraph g(3);  // No edges
    DijkstraUndirectedSP sp(g, 1);

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
    // Graph with cycles (triangle + additional connections)
    EdgeWeightedGraph g(4);
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(1, 2, 1.0));
    g.addEdge(Edge(2, 0, 1.0));  // Creates triangle 0-1-2-0
    g.addEdge(Edge(1, 3, 5.0));
    g.addEdge(Edge(2, 3, 2.0));  // Shorter path to 3

    DijkstraUndirectedSP sp(g, 0);

    assert(isEqual(sp.distTo(0), 0.0));
    assert(isEqual(sp.distTo(1), 1.0));
    assert(isEqual(sp.distTo(2), 1.0));  // Direct via triangle edge
    assert(isEqual(sp.distTo(3), 3.0));  // 0->2->3 (1+2=3) vs 0->1->3 (1+5=6)

    // Verify shortest path to 3 goes through vertex 2
    auto pathTo3 = sp.pathTo(3);
    assert(pathTo3.size() == 2);

    std::cout << "testCyclicGraph passed\n";
}

void testPathConsistency() {
    // Test that path reconstruction is consistent with distances
    EdgeWeightedGraph g(5);
    g.addEdge(Edge(0, 1, 2.0));
    g.addEdge(Edge(0, 2, 1.0));
    g.addEdge(Edge(1, 3, 3.0));
    g.addEdge(Edge(2, 3, 1.0));
    g.addEdge(Edge(3, 4, 2.0));

    DijkstraUndirectedSP sp(g, 0);

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
                int currentVertex = 0;  // Start from source

                for (const auto& edge : path) {
                    // Verify edge connects to current vertex
                    assert(edge.either() == currentVertex || edge.other(edge.either()) == currentVertex);
                    // Move to other endpoint
                    currentVertex = edge.other(currentVertex);
                }

                assert(currentVertex == v);  // Should end at destination
            }
        }
    }

    std::cout << "testPathConsistency passed\n";
}

void testLargeWeights() {
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 1, 1e6));
    g.addEdge(Edge(1, 2, 1e6));

    DijkstraUndirectedSP sp(g, 0);

    assert(isEqual(sp.distTo(0), 0.0));
    assert(isEqual(sp.distTo(1), 1e6));
    assert(isEqual(sp.distTo(2), 2e6));

    std::cout << "testLargeWeights passed\n";
}

void testDifferentSourceVertices() {
    // Test same graph with different source vertices
    EdgeWeightedGraph g(4);
    g.addEdge(Edge(0, 1, 1.0));
    g.addEdge(Edge(1, 2, 2.0));
    g.addEdge(Edge(2, 3, 3.0));
    g.addEdge(Edge(0, 3, 10.0));  // Long direct path

    // Test from vertex 0
    DijkstraUndirectedSP sp0(g, 0);
    assert(isEqual(sp0.distTo(3), 6.0));  // 0->1->2->3

    // Test from vertex 3 (should get same distances in reverse)
    DijkstraUndirectedSP sp3(g, 3);
    assert(isEqual(sp3.distTo(0), 6.0));  // 3->2->1->0

    // Test from vertex 1
    DijkstraUndirectedSP sp1(g, 1);
    assert(isEqual(sp1.distTo(0), 1.0));
    assert(isEqual(sp1.distTo(3), 5.0));  // 1->2->3

    std::cout << "testDifferentSourceVertices passed\n";
}

void testSelfLoops() {
    // Test graph with self-loops (should be ignored in shortest paths)
    EdgeWeightedGraph g(3);
    g.addEdge(Edge(0, 0, 5.0));  // Self-loop (shouldn't affect shortest paths)
    g.addEdge(Edge(0, 1, 2.0));
    g.addEdge(Edge(1, 2, 3.0));

    DijkstraUndirectedSP sp(g, 0);

    assert(isEqual(sp.distTo(0), 0.0));  // Distance to self is 0, not via self-loop
    assert(isEqual(sp.distTo(1), 2.0));
    assert(isEqual(sp.distTo(2), 5.0));

    std::cout << "testSelfLoops passed\n";
}

void testDenseGraph() {
    // Test with a moderately dense graph
    const int n = 6;
    EdgeWeightedGraph g(n);

    // Add many edges to create dense connectivity
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if ((i + j) % 2 == 0) {
                // Add subset of possible edges
                g.addEdge(Edge(i, j, std::abs(i - j) + 1.0));
            }
        }
    }

    DijkstraUndirectedSP sp(g, 0);

    // Verify basic properties
    assert(isEqual(sp.distTo(0), 0.0));

// Count reachable vertices and verify path consistency
    int reachableCount = 0;
    for (int v = 0; v < n; ++v) {
        if (sp.hasPathTo(v)) {
            reachableCount++;

            auto path = sp.pathTo(v);
            double pathWeight = 0.0;
            for (const auto& edge : path) {
                pathWeight += edge.weight();
            }
            assert(isEqual(pathWeight, sp.distTo(v)));
        }
    }

    assert(reachableCount >= 1);  // At least source should be reachable

    std::cout << "testDenseGraph passed\n";
}

int main() {
    std::cout << "Running DijkstraUndirectedSP tests...\n";

    testBasicConstruction();
    testInvalidSourceVertex();
    testNegativeWeights();
    testSingleVertex();
    testDisconnectedGraph();
    testUndirectedShortestPath();
    testBidirectionalTraversal();
    testPathReconstruction();
    testInvalidVertexQueries();
    testParallelEdges();
    testZeroWeights();
    testTriangleGraph();
    testCompleteGraph();
    testStarGraph();
    testChainGraph();
    testFloatingPointPrecision();
    testEmptyGraph();
    testCyclicGraph();
    testPathConsistency();
    testLargeWeights();
    testDifferentSourceVertices();
    testSelfLoops();
    testDenseGraph();

    std::cout << "All tests passed!\n";
    return 0;
}