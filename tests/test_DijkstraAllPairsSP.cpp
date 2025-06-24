#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>
#include <vector>

#include "../data_structures/DijkstraAllPairsSP.hpp"
#include "../data_structures/DirectedEdge.hpp"
#include "../data_structures/EdgeWeightedDigraph.hpp"

const double EPS = 1e-9;

// Helper function to check if two doubles are approximately equal
bool isEqual(double a, double b) {
    return std::abs(a - b) < EPS;
}

void testBasicConstruction() {
    // Test construction with simple directed graph
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 2.0));
    g.addEdge(DirectedEdge(1, 2, 3.0));
    g.addEdge(DirectedEdge(0, 2, 6.0));  // Longer direct path

    DijkstraAllPairsSP allPairs(g);

    assert(allPairs.V() == 3);

    // Test distances from vertex 0
    assert(isEqual(allPairs.dist(0, 0), 0.0));
    assert(isEqual(allPairs.dist(0, 1), 2.0));
    assert(isEqual(allPairs.dist(0, 2), 5.0));  // 0->1->2 is shorter than direct

    // Test path existence
    assert(allPairs.hasPath(0, 0));
    assert(allPairs.hasPath(0, 1));
    assert(allPairs.hasPath(0, 2));

    std::cout << "testBasicConstruction passed\n";
}

void testInvalidVertexQueries() {
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 1.0));

    DijkstraAllPairsSP allPairs(g);

    // Test dist with invalid vertices
    try {
        allPairs.dist(-1, 0);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex out of range") != std::string::npos);
    }

    try {
        allPairs.dist(0, 3);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex out of range") != std::string::npos);
    }

    // Test hasPath with invalid vertices
    try {
        allPairs.hasPath(3, 0);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex out of range") != std::string::npos);
    }

    // Test path with invalid vertices
    try {
        allPairs.path(0, -1);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("Vertex out of range") != std::string::npos);
    }

    std::cout << "testInvalidVertexQueries passed\n";
}

void testNegativeWeights() {
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 2.0));
    g.addEdge(DirectedEdge(1, 2, -1.0));  // Negative weight

    try {
        DijkstraAllPairsSP allPairs(g);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("negative weight") != std::string::npos);
    }

    std::cout << "testNegativeWeights passed\n";
}

void testAllPairsDistances() {
    // Test that all pairs of vertices have correct distances
    EdgeWeightedDigraph g(4);
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(1, 2, 2.0));
    g.addEdge(DirectedEdge(2, 3, 3.0));
    g.addEdge(DirectedEdge(0, 3, 10.0));  // Long direct path

    DijkstraAllPairsSP allPairs(g);

    // Expected distances (shortest paths)
    std::vector<std::vector<double>> expected = {
        {0.0, 1.0, 3.0, 6.0},  // From vertex 0
        {std::numeric_limits<double>::max(), 0.0, 2.0, 5.0},  // From vertex 1
        {std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 0.0, 3.0},  // From vertex 2
        {std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 0.0}   // From vertex 3
    };

    for (int s = 0; s < 4; ++s) {
        for (int v = 0; v < 4; ++v) {
            if (expected[s][v] == std::numeric_limits<double>::max()) {
                assert(!allPairs.hasPath(s, v));
                assert(allPairs.dist(s, v) == std::numeric_limits<double>::max());
            } else {
                assert(allPairs.hasPath(s, v));
                assert(isEqual(allPairs.dist(s, v), expected[s][v]));
            }
        }
    }

    std::cout << "testAllPairsDistances passed\n";
}

void testPathReconstruction() {
    EdgeWeightedDigraph g(4);
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(1, 2, 2.0));
    g.addEdge(DirectedEdge(2, 3, 3.0));

    DijkstraAllPairsSP allPairs(g);

    // Test path from 0 to 3
    auto path = allPairs.path(0, 3);
    assert(path.size() == 3);

    // Verify path: 0->1->2->3
    assert(path[0].from() == 0 && path[0].to() == 1 && isEqual(path[0].weight(), 1.0));
    assert(path[1].from() == 1 && path[1].to() == 2 && isEqual(path[1].weight(), 2.0));
    assert(path[2].from() == 2 && path[2].to() == 3 && isEqual(path[2].weight(), 3.0));

    // Verify path weight equals distance
    double pathWeight = 0.0;
    for (const auto& edge : path) {
        pathWeight += edge.weight();
    }
    assert(isEqual(pathWeight, allPairs.dist(0, 3)));

    std::cout << "testPathReconstruction passed\n";
}

void testDisconnectedGraph() {
    EdgeWeightedDigraph g(4);
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(2, 3, 2.0));
    // Two disconnected components: {0,1} and {2,3}

    DijkstraAllPairsSP allPairs(g);

    // Within component connections
    assert(allPairs.hasPath(0, 1));
    assert(allPairs.hasPath(2, 3));
    assert(isEqual(allPairs.dist(0, 1), 1.0));
    assert(isEqual(allPairs.dist(2, 3), 2.0));

    // Cross-component connections (should not exist)
    assert(!allPairs.hasPath(0, 2));
    assert(!allPairs.hasPath(0, 3));
    assert(!allPairs.hasPath(1, 2));
    assert(!allPairs.hasPath(1, 3));

    // Distances to unreachable vertices
    assert(allPairs.dist(0, 2) == std::numeric_limits<double>::max());
    assert(allPairs.dist(1, 3) == std::numeric_limits<double>::max());

    // Paths to unreachable vertices should be empty
    auto emptyPath = allPairs.path(0, 2);
    assert(emptyPath.empty());

    std::cout << "testDisconnectedGraph passed\n";
}

void testSelfPaths() {
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 2.0));
    g.addEdge(DirectedEdge(1, 2, 3.0));

    DijkstraAllPairsSP allPairs(g);

    // Distance to self should always be 0
    for (int v = 0; v < 3; ++v) {
        assert(isEqual(allPairs.dist(v, v), 0.0));
        assert(allPairs.hasPath(v, v));

        auto selfPath = allPairs.path(v, v);
        assert(selfPath.empty());  // Path to self should be empty
    }

    std::cout << "testSelfPaths passed\n";
}

void testSingleVertex() {
    EdgeWeightedDigraph g(1);
    DijkstraAllPairsSP allPairs(g);

    assert(allPairs.V() == 1);
    assert(isEqual(allPairs.dist(0, 0), 0.0));
    assert(allPairs.hasPath(0, 0));

    auto path = allPairs.path(0, 0);
    assert(path.empty());

    std::cout << "testSingleVertex passed\n";
}

void testCompleteDigraph() {
    // Complete directed graph where every vertex has edge to every other
    const int n = 4;
    EdgeWeightedDigraph g(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                g.addEdge(DirectedEdge(i, j, std::abs(i - j)));
            }
        }
    }

    DijkstraAllPairsSP allPairs(g);

    // Every vertex should be reachable from every other vertex
    for (int s = 0; s < n; ++s) {
        for (int v = 0; v < n; ++v) {
            assert(allPairs.hasPath(s, v));

            if (s == v) {
                assert(isEqual(allPairs.dist(s, v), 0.0));
            } else {
                assert(isEqual(allPairs.dist(s, v), std::abs(s - v)));

                auto path = allPairs.path(s, v);
                assert(path.size() == 1);  // Direct edge should be shortest
            }
        }
    }

    std::cout << "testCompleteDigraph passed\n";
}

void testCyclicGraph() {
    // Graph with cycles
    EdgeWeightedDigraph g(4);
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(1, 2, 1.0));
    g.addEdge(DirectedEdge(2, 0, 1.0));  // Creates cycle 0->1->2->0
    g.addEdge(DirectedEdge(2, 3, 2.0));

    DijkstraAllPairsSP allPairs(g);

    // Test distances within cycle
    assert(isEqual(allPairs.dist(0, 1), 1.0));
    assert(isEqual(allPairs.dist(0, 2), 2.0));  // 0->1->2
    assert(isEqual(allPairs.dist(1, 0), 2.0));  // 1->2->0

    // Test distance to vertex outside cycle
    assert(isEqual(allPairs.dist(0, 3), 4.0));  // 0->1->2->3

    // Vertex 3 cannot reach others (no outgoing edges)
    assert(!allPairs.hasPath(3, 0));
    assert(!allPairs.hasPath(3, 1));
    assert(!allPairs.hasPath(3, 2));

    std::cout << "testCyclicGraph passed\n";
}

void testParallelEdges() {
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 5.0));
    g.addEdge(DirectedEdge(0, 1, 2.0));  // Parallel edge with lower weight
    g.addEdge(DirectedEdge(1, 2, 3.0));

    DijkstraAllPairsSP allPairs(g);

    // Should choose lighter parallel edge
    assert(isEqual(allPairs.dist(0, 1), 2.0));
    assert(isEqual(allPairs.dist(0, 2), 5.0));  // 2.0 + 3.0

    auto pathTo1 = allPairs.path(0, 1);
    assert(pathTo1.size() == 1);
    assert(isEqual(pathTo1[0].weight(), 2.0));

    std::cout << "testParallelEdges passed\n";
}

void testZeroWeights() {
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 0.0));
    g.addEdge(DirectedEdge(1, 2, 0.0));

    DijkstraAllPairsSP allPairs(g);

    assert(isEqual(allPairs.dist(0, 0), 0.0));
    assert(isEqual(allPairs.dist(0, 1), 0.0));
    assert(isEqual(allPairs.dist(0, 2), 0.0));

    auto pathTo2 = allPairs.path(0, 2);
    assert(pathTo2.size() == 2);

    std::cout << "testZeroWeights passed\n";
}

void testAsymmetricDistances() {
    // Test that directed graph produces asymmetric distances
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(1, 0, 5.0));  // Much more expensive return path
    g.addEdge(DirectedEdge(1, 2, 2.0));

    DijkstraAllPairsSP allPairs(g);

    // 0->1 should be cheaper than 1->0
    assert(isEqual(allPairs.dist(0, 1), 1.0));
    assert(isEqual(allPairs.dist(1, 0), 5.0));

    // Verify path lengths are different
    auto path01 = allPairs.path(0, 1);
    auto path10 = allPairs.path(1, 0);
    assert(path01.size() == 1);
    assert(path10.size() == 1);
    assert(path01[0].weight() != path10[0].weight());

    std::cout << "testAsymmetricDistances passed\n";
}

void testTransitivity() {
    // Test that shortest paths satisfy triangle inequality
    EdgeWeightedDigraph g(4);
    g.addEdge(DirectedEdge(0, 1, 2.0));
    g.addEdge(DirectedEdge(1, 2, 3.0));
    g.addEdge(DirectedEdge(2, 3, 1.0));
    g.addEdge(DirectedEdge(0, 3, 10.0));  // Direct path is longer

    DijkstraAllPairsSP allPairs(g);

    // Indirect path should be shorter
    double indirectDist = allPairs.dist(0, 1) + allPairs.dist(1, 2) + allPairs.dist(2, 3);
    double directDist = 10.0;
    double computedDist = allPairs.dist(0, 3);

    assert(computedDist < directDist);
    assert(isEqual(computedDist, indirectDist));
    assert(isEqual(computedDist, 6.0));  // 2+3+1

    std::cout << "testTransitivity passed\n";
}

void testFloatingPointPrecision() {
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 0.1));
    g.addEdge(DirectedEdge(1, 2, 0.2));
    g.addEdge(DirectedEdge(0, 2, 0.4));  // Slightly longer direct path

    DijkstraAllPairsSP allPairs(g);

    // Should choose indirect path
    assert(isEqual(allPairs.dist(0, 2), 0.3));

    auto path = allPairs.path(0, 2);
    assert(path.size() == 2);

    std::cout << "testFloatingPointPrecision passed\n";
}

void testEmptyGraph() {
    EdgeWeightedDigraph g(3);  // No edges
    DijkstraAllPairsSP allPairs(g);

    // Only self-paths should exist
    for (int s = 0; s < 3; ++s) {
        for (int v = 0; v < 3; ++v) {
            if (s == v) {
                assert(allPairs.hasPath(s, v));
                assert(isEqual(allPairs.dist(s, v), 0.0));
            } else {
                assert(!allPairs.hasPath(s, v));
                assert(allPairs.dist(s, v) == std::numeric_limits<double>::max());
            }
        }
    }

    std::cout << "testEmptyGraph passed\n";
}

void testLargeWeights() {
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 1e6));
    g.addEdge(DirectedEdge(1, 2, 1e6));

    DijkstraAllPairsSP allPairs(g);

    assert(isEqual(allPairs.dist(0, 0), 0.0));
    assert(isEqual(allPairs.dist(0, 1), 1e6));
    assert(isEqual(allPairs.dist(0, 2), 2e6));

    std::cout << "testLargeWeights passed\n";
}

void testPathConsistency() {
    // Test that all reconstructed paths are consistent with distances
    EdgeWeightedDigraph g(5);
    g.addEdge(DirectedEdge(0, 1, 2.0));
    g.addEdge(DirectedEdge(0, 2, 1.0));
    g.addEdge(DirectedEdge(1, 3, 3.0));
    g.addEdge(DirectedEdge(2, 3, 1.0));
    g.addEdge(DirectedEdge(3, 4, 2.0));

    DijkstraAllPairsSP allPairs(g);

    // Test all pairs
    for (int s = 0; s < g.V(); ++s) {
        for (int v = 0; v < g.V(); ++v) {
            if (allPairs.hasPath(s, v)) {
                auto path = allPairs.path(s, v);

                double pathWeight = 0.0;
                for (const auto& edge : path) {
                    pathWeight += edge.weight();
                }

                assert(isEqual(pathWeight, allPairs.dist(s, v)));

                // Verify path connectivity
                if (!path.empty()) {
                    assert(path[0].from() == s);  // Path starts from source

                    for (size_t i = 1; i < path.size(); ++i) {
                        assert(path[i-1].to() == path[i].from());  // Edges are connected
                    }

                    assert(path.back().to() == v);  // Path ends at destination
                }
            }
        }
    }

    std::cout << "testPathConsistency passed\n";
}

void testStronglyConnectedComponent() {
    // Create strongly connected component
    EdgeWeightedDigraph g(3);
    g.addEdge(DirectedEdge(0, 1, 1.0));
    g.addEdge(DirectedEdge(1, 2, 2.0));
    g.addEdge(DirectedEdge(2, 0, 3.0));

    DijkstraAllPairsSP allPairs(g);

    // All vertices should be reachable from all others
    for (int s = 0; s < 3; ++s) {
        for (int v = 0; v < 3; ++v) {
            assert(allPairs.hasPath(s, v));
            if (s != v) {
                assert(allPairs.dist(s, v) < std::numeric_limits<double>::max());
            }
        }
    }

    // Check specific distances
    assert(isEqual(allPairs.dist(0, 1), 1.0));
    assert(isEqual(allPairs.dist(0, 2), 3.0));  // 0->1->2
    assert(isEqual(allPairs.dist(1, 0), 5.0));  // 1->2->0

    std::cout << "testStronglyConnectedComponent passed\n";
}

void testPerformanceIndicator() {
    // Test with moderate-sized graph to indicate performance
    const int n = 20;
    EdgeWeightedDigraph g(n);

    // Create chain with some cross edges
    for (int i = 0; i < n - 1; ++i) {
        g.addEdge(DirectedEdge(i, i + 1, 1.0));
        if (i % 3 == 0 && i + 3 < n) {
            g.addEdge(DirectedEdge(i, i + 3, 2.5));  // Skip edges
        }
    }

    DijkstraAllPairsSP allPairs(g);

    // Verify basic properties
    assert(allPairs.V() == n);

    // Check that distances are computed correctly
    for (int s = 0; s < n; ++s) {
        assert(isEqual(allPairs.dist(s, s), 0.0));

        // Test a few reachable vertices
        for (int v = s + 1; v < std::min(s + 5, n); ++v) {
            assert(allPairs.hasPath(s, v));
            assert(allPairs.dist(s, v) < std::numeric_limits<double>::max());
        }
    }

    std::cout << "testPerformanceIndicator passed\n";
}

int main() {
    std::cout << "Running DijkstraAllPairsSP tests...\n";

    testBasicConstruction();
    testInvalidVertexQueries();
    testNegativeWeights();
    testAllPairsDistances();
    testPathReconstruction();
    testDisconnectedGraph();
    testSelfPaths();
    testSingleVertex();
    testCompleteDigraph();
    testCyclicGraph();
    testParallelEdges();
    testZeroWeights();
    testAsymmetricDistances();
    testTransitivity();
    testFloatingPointPrecision();
    testEmptyGraph();
    testLargeWeights();
    testPathConsistency();
    testStronglyConnectedComponent();
    testPerformanceIndicator();

    std::cout << "All tests passed!\n";
    return 0;
}