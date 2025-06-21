#pragma once

#include <stdexcept>
#include <vector>

#include "DepthFirstDirectedPaths.hpp"
#include "Digraph.hpp"

/**
 * Computes the transitive closure of a directed graph.
 * The transitive closure contains an edge (v,w) if and only if
 * there is a directed path from vertex v to vertex w in the original graph.
 *
 * This implementation precomputes all-pairs reachability by running DFS
 * from each vertex. Query time is O(1), but preprocessing takes O(V*(V+E))
 * time and O(V²) space in the worst case.
 *
 * For sparse graphs, this is efficient for many reachability queries.
 * For dense graphs or few queries, consider using single-source DFS instead.
 */
class TransitiveClosure {
    std::vector<DepthFirstDirectedPaths> tc;  // tc[v] contains all vertices reachable from v

    // Validate vertex index is within bounds
    void validateVertex(const int v) const {
        if (v < 0 || v >= static_cast<int>(tc.size())) {
            throw std::invalid_argument("Vertex out of range");
        }
    }

public:
    // Constructor: precompute reachability from each vertex using DFS
    explicit TransitiveClosure(const Digraph& g) {
        tc.reserve(g.V());  // Reserve space for efficiency

        // For each vertex, compute all vertices reachable from it
        for (int v = 0; v < g.V(); ++v) {
            tc.emplace_back(g, v);  // DFS from vertex v
        }
    }

    // Check if vertex w is reachable from vertex v
    // Returns true if there exists a directed path from v to w
    bool reachable(const int v, const int w) const {
        validateVertex(v);
        validateVertex(w);
        return tc[v].hasPathTo(w);
    }
};
