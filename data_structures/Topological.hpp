#pragma once

#include <stdexcept>
#include <vector>

#include "DepthFirstOrder.hpp"
#include "Digraph.hpp"
#include "DirectedCycle.hpp"

/**
 * Computes a topological ordering of vertices in a directed acyclic graph (DAG).
 * A topological ordering is a linear ordering of vertices such that for every
 * directed edge (u,v), vertex u comes before vertex v in the ordering.
 *
 * Uses DFS-based algorithm: reverse postorder gives topological ordering for DAGs.
 * Time complexity: O(V + E), Space complexity: O(V)
 */
class Topological {
    std::vector<int> order_;  // topological ordering of vertices (empty if graph has cycle)
    std::vector<int> rank_;   // rank_[v] = position of vertex v in topological ordering

    // Validate vertex index is within bounds
    void validateVertex(const int v) const {
        if (v < 0 || v >= static_cast<int>(rank_.size())) {
            throw std::invalid_argument("Vertex out of range");
        }
    }

public:
    // Constructor: compute topological ordering if DAG, empty if has cycle
    explicit Topological(const Digraph& g) : rank_(g.V()) {
        // First check if graph has a directed cycle
        if (const DirectedCycle finder(g); !finder.hasCycle()) {
            // Graph is a DAG - compute topological ordering using DFS
            const DepthFirstOrder dfs(g);
            order_ = dfs.reversePost();  // Reverse postorder gives topological ordering

            // Compute rank for each vertex (position in topological ordering)
            int i = 0;
            for (const int v : order_)
                rank_[v] = i++;
        }
        // If graph has cycle, order_ remains empty and rank_ contains default values
    }

    // Get the topological ordering (empty vector if graph has cycle)
    std::vector<int> order() const {
        return order_;
    }

    // Check if graph has a topological ordering (i.e., is a DAG)
    bool hasOrder() const {
        return !order_.empty();
    }

    // Get the rank (position) of vertex v in topological ordering
    // Returns -1 if graph has cycle
    int rank(const int v) const {
        validateVertex(v);
        if (hasOrder()) {
            return rank_[v];
        }
        return -1;
    }
};
