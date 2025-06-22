#pragma once

#include <cassert>
#include <cmath>
#include <limits>
#include <optional>
#include <vector>

#include "Edge.hpp"
#include "EdgeWeightedGraph.hpp"
#include "IndexMinPQ.hpp"
// #include "WeightedQuickUnionPathCompressionUF.hpp"

/**
 * Computes the Minimum Spanning Tree (MST) of an edge-weighted undirected graph
 * using Prim's algorithm with an eager implementation.
 *
 * A Minimum Spanning Tree is a subset of edges that:
 * 1. Connects all vertices (spanning tree property)
 * 2. Has minimum total weight among all possible spanning trees
 * 3. Contains exactly V-1 edges for V vertices in each connected component
 *
 * Eager Prim's Algorithm:
 * 1. Start with arbitrary vertex, mark it as visited
 * 2. Maintain priority queue of vertices not yet in MST
 * 3. For each vertex, track the minimum weight edge connecting it to MST
 * 4. Repeatedly add vertex with minimum connection weight to MST
 * 5. Update connection weights for neighbors of newly added vertex
 *
 * "Eager" because it maintains only one edge per vertex in the priority queue,
 * updating edge weights when better connections are found.
 *
 * Time complexity: O(E log V), Space complexity: O(V)
 * where E is the number of edges and V is the number of vertices.
 * This is better than lazy Prim's O(E log E) when the graph is dense.
 */
class PrimMST {
    // static constexpr double EPS = 1.0e-12;  // Epsilon for floating-point comparison

    std::vector<std::optional<Edge>> edgeTo; // edgeTo[v] = shortest edge from tree vertex to non-tree vertex v
    std::vector<double> distTo;              // distTo[v] = weight of shortest such edge
    std::vector<bool> marked;                // marked[v] = true if v is on the tree
    IndexMinPQ<double> pq;                  // eligible crossing edges (vertices not yet in MST)

    // Run Prim's algorithm starting from vertex s
    void prim(const EdgeWeightedGraph& g, const int s) {
        distTo[s] = 0.0;
        pq.insert(s, distTo[s]);

        while (!pq.empty()) {
            const int v = pq.removeMin();  // Add closest vertex to MST
            scan(g, v);
        }
    }

    // Add vertex v to MST and update distances to adjacent vertices
    void scan(const EdgeWeightedGraph& g, const int v) {
        marked[v] = true;

        // Check all edges from v to update minimum connection weights
        for (const Edge& e : g.adj(v)) {
            int w = e.other(v);

            // Skip if w is already in MST
            if (marked[w]) continue;

            // If edge v-w is shorter than current best edge to w
            if (e.weight() < distTo[w]) {
                distTo[w] = e.weight();
                edgeTo[w] = e;  // std::optional assignment

                // Update priority queue
                if (pq.contains(w)) {
                    pq.decreaseKey(w, distTo[w]);
                } else {
                    pq.insert(w, distTo[w]);
                }
            }
        }
    }

    // Verify that computed MST is correct
    // bool check(const EdgeWeightedGraph& g) const {
    //     // Check 1: Verify total weight matches sum of edge weights
    //     double totalWeight = 0.0;
    //     for (const Edge& e : edges()) {
    //         totalWeight += e.weight();
    //     }
    //
    //     if (std::abs(totalWeight - weight()) > EPS) return false;
    //
    //     // Check 2: Verify MST is acyclic (no edge creates cycle)
    //     WeightedQuickUnionPathCompressionUF uf(g.V());
    //     for (const Edge& e : edges()) {
    //         const int v = e.either();
    //         const int w = e.other(v);
    //
    //         if (uf.connected(v, w)) return false;  // Edge would create cycle
    //         uf.unite(v, w);
    //     }
    //
    //     // Check 3: Verify MST connects all vertices in same component
    //     for (int v = 0; v < g.V() - 1; ++v) {
    //         for (int w = v + 1; w < g.V(); ++w) {
    //             if (!uf.connected(v, w)) {
    //                 // Check if vertices are in same component in original graph
    //                 WeightedQuickUnionPathCompressionUF graphUF(g.V());
    //                 for (const Edge& edge : g.edges()) {
    //                     graphUF.unite(edge.either(), edge.other(edge.either()));
    //                 }
    //                 if (graphUF.connected(v, w)) {
    //                     return false;  // MST should connect all vertices in same component
    //                 }
    //             }
    //         }
    //     }
    //
    //     // Check 4: Verify cut property - each edge is minimum crossing its cut
    //     for (int i = 0; i < edges().size(); ++i) {
    //         const Edge& mstEdge = edges()[i];
    //
    //         // Create MST without this edge to form two components
    //         WeightedQuickUnionPathCompressionUF cutUF(g.V());
    //         auto mstEdges = edges();
    //
    //         for (int j = 0; j < mstEdges.size(); ++j) {
    //             if (i != j) {  // Skip the edge we're testing
    //                 const Edge& e = mstEdges[j];
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
    // Constructor: compute MST using eager Prim's algorithm
    explicit PrimMST(const EdgeWeightedGraph& g) :
        edgeTo(g.V()),  // Creates vector of std::optional<Edge> (default empty)
        distTo(g.V(), std::numeric_limits<double>::max()),
        marked(g.V(), false),
        pq(g.V()) {

        // Handle disconnected components by running Prim from each unvisited vertex
        for (int v = 0; v < g.V(); ++v) {
            if (!marked[v]) {
                prim(g, v);
            }
        }

        // assert(check(g));  // Verify correctness
    }

    // Get all edges in the MST
    std::vector<Edge> edges() const {
        std::vector<Edge> mst;
        mst.reserve(edgeTo.size());  // Reserve space for efficiency

        for (const auto& v : edgeTo) {
            // Only include vertices that have incoming MST edges
            if (v.has_value()) {
                mst.push_back(v.value());
            }
        }
        return mst;
    }

    // Get total weight of the MST
    double weight() const {
        double totalWeight = 0.0;
        for (const auto& v : edgeTo) {
            if (v.has_value()) {
                totalWeight += v.value().weight();
            }
        }
        return totalWeight;
    }
};
