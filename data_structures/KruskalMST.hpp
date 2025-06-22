#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

#include "Edge.hpp"
#include "EdgeWeightedGraph.hpp"
#include "WeightedQuickUnionPathCompressionUF.hpp"

/**
 * Computes the Minimum Spanning Tree (MST) of an edge-weighted undirected graph
 * using Kruskal's algorithm.
 *
 * A Minimum Spanning Tree is a subset of edges that:
 * 1. Connects all vertices (spanning tree property)
 * 2. Has minimum total weight among all possible spanning trees
 * 3. Contains exactly V-1 edges for V vertices in each connected component
 *
 * Kruskal's Algorithm:
 * 1. Sort all edges by weight in ascending order
 * 2. Initialize each vertex as its own component (using Union-Find)
 * 3. For each edge in sorted order:
 *    - If edge connects different components, add to MST
 *    - Union the components connected by this edge
 * 4. Stop when MST has V-1 edges (for connected graph)
 *
 * Key insight: Always choose the minimum weight edge that doesn't create a cycle.
 * This greedy approach works due to the cut property of MSTs.
 *
 * Time complexity: O(E log E) due to sorting edges
 * Space complexity: O(V + E) for Union-Find and edge storage
 * where E is the number of edges and V is the number of vertices.
 */
class KruskalMST {
    // static constexpr double EPS = 1.0e-12;  // Epsilon for floating-point comparison

    double weight_;          // total weight of MST
    std::vector<Edge> mst;   // edges in the MST

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
    //     // Fixed: Check connectivity after building MST, not during construction
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
    // Constructor: compute MST using Kruskal's algorithm
    explicit KruskalMST(const EdgeWeightedGraph& g) : weight_(0.0) {
        // Step 1: Get all edges and sort them by weight
        std::vector<Edge> edges;
        edges.reserve(g.E());  // Reserve space for efficiency

        for (const Edge& e : g.edges()) {
            edges.push_back(e);
        }

        // Sort edges by weight (ascending order)
        std::sort(edges.begin(), edges.end());

        // Step 2: Initialize Union-Find for cycle detection
        WeightedQuickUnionPathCompressionUF uf(g.V());

        // Step 3: Process edges in order, adding to MST if no cycle created
        for (auto e : edges) {
            const int v = e.either();
            const int w = e.other(v);

            // If edge connects different components (no cycle), add to MST
            if (!uf.connected(v, w)) {
                uf.unite(v, w);
                mst.push_back(e);
                weight_ += e.weight();
            }

            // Early termination optimization: stop when we have enough edges
            // For connected graph: V-1 edges
            // For disconnected graph: continue until all edges processed
            if (mst.size() == g.V() - 1) {
                break;
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