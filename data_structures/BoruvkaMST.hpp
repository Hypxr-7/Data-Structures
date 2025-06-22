#pragma once

#include <cassert>
#include <cmath>
#include <optional>
#include <vector>

#include "Edge.hpp"
#include "EdgeWeightedGraph.hpp"
#include "WeightedQuickUnionPathCompressionUF.hpp"

/**
 * Computes the Minimum Spanning Tree (MST) of an edge-weighted undirected graph
 * using Borůvka's algorithm.
 *
 * A Minimum Spanning Tree is a subset of edges that:
 * 1. Connects all vertices (spanning tree property)
 * 2. Has minimum total weight among all possible spanning trees
 * 3. Contains exactly V-1 edges for V vertices in each connected component
 *
 * Borůvka's Algorithm:
 * 1. Start with each vertex as its own component
 * 2. In each iteration:
 *    - For each component, find the minimum weight edge leaving it
 *    - Add all such edges to MST (they won't create cycles)
 *    - Merge components connected by these edges
 * 3. Repeat until only one component remains (or no more edges can be added)
 *
 * Key insight: In each iteration, each component finds its cheapest outgoing edge.
 * Since components are disjoint, these edges cannot create cycles within the iteration.
 * The number of components at least halves in each iteration.
 *
 * Time complexity: O(E log V) - at most log V iterations, each taking O(E) time
 * Space complexity: O(V + E) for Union-Find and edge storage
 * where E is the number of edges and V is the number of vertices.
 *
 * Historical note: This is the oldest MST algorithm, discovered by Otakar Borůvka in 1926
 * for finding efficient electrical power networks in Moravia.
 */
class BoruvkaMST {
    // static constexpr double EPS = 1.0e-12;  // Epsilon for floating-point comparison

    std::vector<Edge> mst;    // edges in the MST
    double weight_;           // total weight of MST

    // Verify that computed MST is correct
    // bool check(const EdgeWeightedGraph& g) const {
    //     // Check 1: Verify total weight matches sum of edge weights
    //     double total = 0.0;
    //     for (const Edge& e : mst) {
    //         total += e.weight();
    //     }
    //     if (std::abs(total - weight_) > EPS) return false;
    //
    //     // Check 2: Verify MST is acyclic (no edge creates cycle)
    //     WeightedQuickUnionPathCompressionUF uf(g.V());
    //     for (const Edge& e : mst) {
    //         int v = e.either();
    //         int w = e.other(v);
    //
    //         if (uf.connected(v, w)) return false;  // Edge would create cycle
    //         uf.unite(v, w);
    //     }
    //
    //     // Check 3: Verify MST connects all vertices in same component
    //     WeightedQuickUnionPathCompressionUF connectivityUF(g.V());
    //     for (const Edge& e : mst) {
    //         connectivityUF.unite(e.either(), e.other(e.either()));
    //     }
    //
    //     // Verify all vertices in same component in original graph are connected in MST
    //     WeightedQuickUnionPathCompressionUF graphUF(g.V());
    //     for (const Edge& edge : g.edges()) {
    //         graphUF.unite(edge.either(), edge.other(edge.either()));
    //     }
    //
    //     for (int v = 0; v < g.V() - 1; ++v) {
    //         for (int w = v + 1; w < g.V(); ++w) {
    //             // If vertices are in same component in original graph
    //             if (graphUF.connected(v, w)) {
    //                 // They should also be connected in MST
    //                 if (!connectivityUF.connected(v, w)) {
    //                     return false;
    //                 }
    //             }
    //         }
    //     }
    //
    //     // Check 4: Verify cut property - each edge is minimum crossing its cut
    //     for (int i = 0; i < mst.size(); ++i) {
    //         const Edge& mstEdge = mst[i];
    //
    //         // Create MST without this edge to form two components
    //         WeightedQuickUnionPathCompressionUF cutUF(g.V());
    //
    //         for (int j = 0; j < mst.size(); ++j) {
    //             if (i != j) {  // Skip the edge we're testing
    //                 const Edge& e = mst[j];
    //                 cutUF.unite(e.either(), e.other(e.either()));
    //             }
    //         }
    //
    //         // Check all edges in original graph crossing this cut
    //         for (const Edge& e : g.edges()) {
    //             const int x = e.either();
    //             const int y = e.other(x);
    //
    //             // If edge crosses the cut (connects different components)
    //             if (!cutUF.connected(x, y)) {
    //                 // MST edge should have minimum weight among all crossing edges
    //                 if (e.weight() < mstEdge.weight()) {
    //                     return false;
    //                 }
    //             }
    //         }
    //     }
    //
    //     return true;
    // }

public:
    // Constructor: compute MST using Borůvka's algorithm
    explicit BoruvkaMST(const EdgeWeightedGraph& g) : weight_(0.0) {
        WeightedQuickUnionPathCompressionUF uf(g.V());

        // Main loop: continue until we have a spanning forest
        // In each iteration, the number of components at least halves
        // so we need at most log V iterations
        for (int t = 1; t < g.V() && mst.size() < g.V() - 1; t *= 2) {
            // For each component, find the minimum weight edge leaving it
            std::vector<std::optional<Edge>> closest(g.V(), std::nullopt);

            // Examine all edges to find cheapest outgoing edge for each component
            for (const Edge& e : g.edges()) {
                const int v = e.either();
                const int w = e.other(v);
                const int i = uf.find(v);  // Component containing v
                const int j = uf.find(w);  // Component containing w

                // Skip edges within the same component
                if (i == j) continue;

                // Update minimum outgoing edge for component i
                if (!closest[i].has_value() || e.weight() < closest[i].value().weight()) {
                    closest[i] = e;
                }

                // Update minimum outgoing edge for component j
                if (!closest[j].has_value() || e.weight() < closest[j].value().weight()) {
                    closest[j] = e;
                }
            }

            // Add all minimum outgoing edges to MST
            // These edges cannot create cycles since they connect different components
            for (int i = 0; i < g.V(); ++i) {
                if (!closest[i].has_value()) continue;  // Component i has no outgoing edges

                Edge e = closest[i].value();
                const int v = e.either();
                const int w = e.other(v);

                // Double-check that edge still connects different components
                // (multiple components might have chosen the same edge)
                if (!uf.connected(v, w)) {
                    mst.push_back(e);
                    weight_ += e.weight();
                    uf.unite(v, w);
                }
            }
        }

        // assert(check(g));  // Verify correctness
    }

    // Get all edges in the MST
    std::vector<Edge> edges() const {
        return mst;
    }

    // Get total weight of the MST
    double weight() const {
        return weight_;
    }
};