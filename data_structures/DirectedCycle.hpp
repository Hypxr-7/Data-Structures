#pragma once

#include <cassert>
#include <vector>

#include "Digraph.hpp"

/**
 * Detects directed cycles in a digraph using depth-first search.
 * A directed cycle exists when there's a back edge to a vertex currently on the DFS stack.
 */
class DirectedCycle {
    std::vector<bool> marked;   // marked[v] = true if vertex v has been visited
    std::vector<int> edgeTo;    // edgeTo[v] = previous vertex on path to v
    std::vector<bool> onStack;  // onStack[v] = true if vertex v is on the current DFS stack
    std::vector<int> cycle_;    // vertices in the directed cycle (empty if no cycle)

    // DFS to detect directed cycles
    void dfs(const Digraph& g, const int v) {
        onStack[v] = true;  // Add v to current DFS path
        marked[v] = true;   // Mark v as visited

        for (const int w : g.adj(v)) {
            // If cycle already found, stop searching
            if (!cycle_.empty()) return;

            if (!marked[w]) {
                // Tree edge: continue DFS
                edgeTo[w] = v;
                dfs(g, w);
            }
            else if (onStack[w]) {
                // Back edge found: w is on current stack, so we have a cycle
                // Build cycle from v back to w
                for (int x = v; x != w; x = edgeTo[x]) {
                    cycle_.push_back(x);
                }
                cycle_.push_back(w);
                cycle_.push_back(v);  // Complete the cycle
                assert(check());
            }
            // If marked[w] && !onStack[w]: forward/cross edge, no cycle
        }

        onStack[v] = false;  // Remove v from current DFS path (backtrack)
    }

    // Verify that the found cycle is valid
    bool check() const {
        if (hasCycle()) {
            // Check that cycle is actually a cycle (first == last)
            if (cycle_.empty()) return false;

            if (cycle_.front() != cycle_.back()) return false;

            // Verify cycle has at least 2 vertices
            if (cycle_.size() < 2) return false;
        }
        return true;
    }

public:
    // Constructor: find a directed cycle in the digraph
    explicit DirectedCycle(const Digraph& g) :
        marked(g.V(), false),
        edgeTo(g.V()),
        onStack(g.V(), false) {

        // Check each connected component for cycles
        for (int v = 0; v < g.V(); ++v) {
            if (!marked[v] && cycle_.empty()) {
                dfs(g, v);
            }
        }
    }

    // Returns true if the digraph contains a directed cycle
    bool hasCycle() const {
        return !cycle_.empty();
    }

    // Returns vertices in a directed cycle (empty if no cycle exists)
    std::vector<int> cycle() const {
        return cycle_;
    }
};