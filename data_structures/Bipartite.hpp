#pragma once

#include <cassert>
#include <stdexcept>
#include <vector>

#include "Graph.hpp"

/**
 * Determines if an undirected graph is bipartite and finds an odd cycle if not
 */
class Bipartite {
private:
    bool isBipartite_;           // true if graph is bipartite
    std::vector<bool> color_;    // color_[v] = color of vertex v (true/false for two partitions)
    std::vector<bool> marked;    // marked[v] = true if vertex v has been visited
    std::vector<int> edgeTo;     // edgeTo[v] = previous vertex on path to v
    std::vector<int> cycle;      // odd cycle (empty if graph is bipartite)

    // DFS to check bipartiteness and find odd cycle
    void dfs(const Graph& g, const int v) {
        marked[v] = true;
        for (const int w : g.adj(v)) {
            // If odd cycle already found, stop searching
            if (!cycle.empty()) return;

            if (!marked[w]) {
                edgeTo[w] = v;
                color_[w] = !color_[v];  // Fixed: assign opposite color, not flip
                dfs(g, w);
            }
            // Found back edge with same color - indicates odd cycle
            else if (color_[w] == color_[v]) {
                isBipartite_ = false;
                // Build odd cycle from v back to w
                cycle.clear();
                for (int x = v; x != w; x = edgeTo[x]) {
                    cycle.push_back(x);
                }
                cycle.push_back(w);
            }
        }
    }

    // Check invariants for correctness
    bool check(const Graph& g) {
        if (isBipartite_) {
            // If bipartite, no edge should connect same-colored vertices
            for (int v = 0; v < g.V(); ++v) {
                for (const int w : g.adj(v)) {
                    if (color_[v] == color_[w]) {
                        return false;
                    }
                }
            }
        }
        else {
            // If not bipartite, should have valid odd cycle
            if (cycle.size() < 3) return false;

            // Check cycle is actually a cycle in the graph
            for (size_t i = 0; i < cycle.size(); ++i) {
                const int v = cycle[i];
                const int w = cycle[(i + 1) % cycle.size()];
                bool found = false;
                for (const int x : g.adj(v)) {
                    if (x == w) {
                        found = true;
                        break;
                    }
                }
                if (!found) return false;
            }

            // Check cycle has odd length
            if (cycle.size() % 2 == 0) return false;
        }
        return true;
    }

    // Validate vertex index is within bounds
    void validateVertex(const int v) const {
        if (v < 0 || v >= static_cast<int>(marked.size())) {
            throw std::invalid_argument("Vertex out of range");
        }
    }

public:
    // Constructor: determine if graph is bipartite
    explicit Bipartite(const Graph& g) :
        isBipartite_(true),
        color_(g.V(), false),
        marked(g.V(), false),
        edgeTo(g.V()) {

        // Check each connected component
        for (int v = 0; v < g.V(); ++v) {
            if (!marked[v]) {
                dfs(g, v);
            }
        }
        assert(check(g));
    }

    // Returns true if graph is bipartite
    bool isBipartite() const {
        return isBipartite_;
    }

    // Returns the color/partition of vertex v (only valid if graph is bipartite)
    bool color(const int v) const {
        validateVertex(v);
        if (!isBipartite_) {
            throw std::runtime_error("Graph is not bipartite");
        }
        return color_[v];
    }

    // Returns an odd cycle if graph is not bipartite (empty if bipartite)
    std::vector<int> oddCycle() const {
        return cycle;
    }
};