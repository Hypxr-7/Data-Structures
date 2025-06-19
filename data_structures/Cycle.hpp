#pragma once

#include <vector>

#include "Graph.hpp"

class Cycle {
    std::vector<bool> marked;    // marked[v] = true if vertex v has been visited
    std::vector<int> edgeTo;     // edgeTo[v] = previous vertex on path to v
    std::vector<int> cycle_;     // vertices in the cycle (empty if no cycle)

    // Check for self-loops (vertex connected to itself)
    bool hasSelfLoop(const Graph& g) {
        for (int v = 0; v < g.V(); ++v) {
            for (const int w : g.adj(v)) {
                if (v == w) {
                    cycle_.push_back(v);
                    cycle_.push_back(v);
                    return true;
                }
            }
        }
        return false;
    }

    // Check for parallel edges (multiple edges between same vertices)
    bool hasParallelEdges(const Graph& g) {
        marked = std::vector<bool>(g.V(), false);

        for (int v = 0; v < g.V(); ++v) {
            // Check if any adjacent vertex is already marked (parallel edge)
            for (const int w : g.adj(v)) {
                if (marked[w]) {
                    cycle_.push_back(v);
                    cycle_.push_back(w);
                    cycle_.push_back(v);
                    return true;
                }
                marked[w] = true;
            }

            // Unmark all adjacent vertices for next iteration
            for (const int w : g.adj(v)) {
                marked[w] = false;
            }
        }
        return false;
    }

    // DFS to find cycle, u is the parent of v
    void dfs(const Graph& g, const int u, const int v) {
        marked[v] = true;
        for (const int w : g.adj(v)) {
            // If cycle already found, stop searching
            if (!cycle_.empty()) return;

            if (!marked[w]) {
                edgeTo[w] = v;
                dfs(g, v, w);  // v becomes parent of w
            }
            // Found back edge to non-parent vertex - indicates cycle
            else if (w != u) {
                // Build cycle from v back to w
                for (int x = v; x != w; x = edgeTo[x]) {
                    cycle_.push_back(x);
                }
                cycle_.push_back(w);
                cycle_.push_back(v);
            }
        }
    }

public:
    // Constructor: find a cycle in the graph
    explicit Cycle(const Graph& g) : marked(g.V(), false), edgeTo(g.V()) {
        // Check for trivial cycles first
        if (hasSelfLoop(g)) return;
        if (hasParallelEdges(g)) return;

        // Use DFS to find cycles in each connected component
        for (int v = 0; v < g.V(); ++v) {
            if (!marked[v]) {
                dfs(g, -1, v);  // -1 indicates no parent for root
            }
        }
    }

    // Returns true if graph contains a cycle
    bool hasCycle() const {
        return !cycle_.empty();
    }

    // Returns vertices in a cycle (empty if no cycle exists)
    std::vector<int> cycle() const {
        return cycle_;
    }
};
