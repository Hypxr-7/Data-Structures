#pragma once

#include <cassert>
#include <cmath>
#include <queue>
#include <vector>

#include "Edge.hpp"
#include "EdgeWeightedGraph.hpp"
// #include "WeightedQuickUnionPathCompressionUF.hpp"

/**
 * Computes the Minimum Spanning Tree (MST) of an edge-weighted undirected graph
 * using Prim's algorithm with a lazy implementation.
 *
 * A Minimum Spanning Tree is a subset of edges that:
 * 1. Connects all vertices (spanning tree property)
 * 2. Has minimum total weight among all possible spanning trees
 * 3. Contains exactly V-1 edges for V vertices
 *
 * Lazy Prim's Algorithm:
 * 1. Start with arbitrary vertex, mark it as visited
 * 2. Add all edges from visited vertices to priority queue
 * 3. Repeatedly extract minimum weight edge from queue
 * 4. If edge connects visited to unvisited vertex, add to MST
 * 5. Mark new vertex as visited and add its edges to queue
 * 6. Continue until all vertices are visited
 *
 * "Lazy" because obsolete edges (connecting two visited vertices)
 * are kept in priority queue and filtered out when dequeued.
 *
 * Time complexity: O(E log E), Space complexity: O(E)
 * where E is the number of edges and V is the number of vertices.
 */
class LazyPrimMST {
    // static constexpr double EPS = 1.0e-12;  // Epsilon for floating-point comparison

    std::vector<Edge> mst;           // edges in the MST
    std::vector<bool> marked;        // marked[v] = true if vertex v is in MST
    std::priority_queue<Edge, std::vector<Edge>, std::greater<>> pq;  // min-heap of edges
    double weight_;                  // total weight of MST

    // Run Prim's algorithm starting from vertex s
    void prim(const EdgeWeightedGraph& g, const int s) {
        scan(g, s);  // Add vertex s to MST

        while (!pq.empty()) {
            Edge e = pq.top();
            pq.pop();

            int v = e.either();
            int w = e.other(v);

            // Skip if both vertices already in MST (obsolete edge)
            if (marked[v] && marked[w]) continue;

            // Add edge to MST
            mst.push_back(e);
            weight_ += e.weight();

            // Add newly connected vertices to MST
            if (!marked[v]) scan(g, v);
            if (!marked[w]) scan(g, w);
        }
    }

    // Add vertex v to MST and add all its crossing edges to priority queue
    void scan(const EdgeWeightedGraph& g, const int v) {
        marked[v] = true;

        // Add all edges from v to unvisited vertices to priority queue
        for (const Edge& e : g.adj(v)) {
            if (!marked[e.other(v)]) {
                pq.push(e);
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
    //     if (std::abs(totalWeight - weight_) > EPS) return false;
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
    //     // Check 4: Verify MST satisfies cut property (each edge is minimum crossing edge)
    //     // For each edge in MST, removing it should disconnect the components
    //     // and the edge should be minimum weight among all edges crossing that cut
    //     for (const Edge& mstEdge : edges()) {
    //         WeightedQuickUnionPathCompressionUF cutUF(g.V());
    //
    //         // Build MST without this edge
    //         for (const Edge& e : mst) {
    //             if (e.weight() != mstEdge.weight() ||
    //                 e.either() != mstEdge.either() ||
    //                 e.other(e.either()) != mstEdge.other(mstEdge.either())) {
    //                 cutUF.unite(e.either(), e.other(e.either()));
    //             }
    //         }
    //
    //         // Check all edges in original graph crossing the cut
    //         for (const Edge& e : g.edges()) {
    //             const int x = e.either();
    //             const int y = e.other(x);
    //
    //             // If edge crosses the cut created by removing mstEdge
    //             if (!cutUF.connected(x, y)) {
    //                 // MST edge should have minimum weight among crossing edges
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
    // Constructor: compute MST using Prim's algorithm
    explicit LazyPrimMST(const EdgeWeightedGraph& g) :
        marked(g.V(), false),
        weight_(0.0) {

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
        return mst;
    }

    // Get total weight of the MST
    double weight() const {
        return weight_;
    }
};