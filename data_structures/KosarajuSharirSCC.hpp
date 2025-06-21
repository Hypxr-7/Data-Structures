#pragma once

#include <cassert>
#include <stdexcept>
#include <vector>

#include "DepthFirstOrder.hpp"
#include "Digraph.hpp"
// #include "TransitiveClosure.hpp"

/**
 * Computes strongly connected components (SCCs) of a directed graph using
 * the Kosaraju-Sharir algorithm.
 *
 * A strongly connected component is a maximal set of vertices such that
 * there is a directed path from every vertex to every other vertex in the set.
 *
 * Algorithm:
 * 1. Compute reverse postorder of G^R (reverse graph)
 * 2. Run DFS on G in that order
 * 3. Each DFS tree in step 2 is a strongly connected component
 *
 * Time complexity: O(V + E), Space complexity: O(V)
 */
class KosarajuSharirSCC {
    std::vector<bool> marked;  // marked[v] = true if vertex v has been visited
    std::vector<int> id_;      // id_[v] = strongly connected component id of vertex v
    int count_;                // number of strongly connected components

    // DFS to mark vertices in the same strongly connected component
    void dfs(const Digraph& g, const int v) {
        marked[v] = true;
        id_[v] = count_;  // Assign current component id to vertex v

        // Visit all unvisited adjacent vertices
        for (const int w : g.adj(v)) {
            if (!marked[w]) {
                dfs(g, w);
            }
        }
    }

    // Verify the correctness of SCC computation using transitive closure
    // bool check(const Digraph& g) const {
    //     const TransitiveClosure tc(g);
    //
    //     // Two vertices are strongly connected iff they can reach each other
    //     for (int v = 0; v < g.V(); ++v) {
    //         for (int w = 0; w < g.V(); ++w) {
    //             if (stronglyConnected(v, w) != (tc.reachable(v, w) && tc.reachable(w, v))) {
    //                 return false;
    //             }
    //         }
    //     }
    //     return true;
    // }

    // Validate vertex index is within bounds
    void validateVertex(const int v) const {
        if (v < 0 || v >= static_cast<int>(marked.size())) {
            throw std::invalid_argument("Vertex out of range");
        }
    }

public:
    // Constructor: compute strongly connected components using Kosaraju-Sharir algorithm
    explicit KosarajuSharirSCC(const Digraph& g) :
        marked(g.V(), false),
        id_(g.V()),
        count_(0) {

        // Step 1: Compute reverse postorder of reverse graph
        const DepthFirstOrder dfo(g.reverse());

        // Step 2: Run DFS on original graph in reverse postorder of G^R
        // Each DFS tree found is a strongly connected component
        for (const int v : dfo.reversePost()) {
            if (!marked[v]) {
                dfs(g, v);    // Find one SCC starting from vertex v
                ++count_;     // Increment SCC count
            }
        }

        // assert(check(g));  // Verify correctness
    }

    // Get the number of strongly connected components
    int count() const {
        return count_;
    }

    // Check if two vertices are in the same strongly connected component
    bool stronglyConnected(const int v, const int w) const {
        validateVertex(v);
        validateVertex(w);
        return id_[v] == id_[w];
    }

    // Get the strongly connected component id of vertex v
    // Vertices with the same id are in the same SCC
    int id(const int v) const {
        validateVertex(v);
        return id_[v];
    }
};