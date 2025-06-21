#pragma once

#include <algorithm>
#include <cassert>
#include <stack>
#include <vector>

// #include "BreadthFirstPaths.hpp"
#include "Digraph.hpp"
// #include "Graph.hpp"

/**
 * Find an Eulerian path in a directed graph using Hierholzer's algorithm.
 * An Eulerian path visits every edge exactly once but doesn't need to return to start.
 *
 * For a directed graph to have an Eulerian path:
 * 1. At most one vertex has outdegree - indegree = 1 (start vertex)
 * 2. At most one vertex has indegree - outdegree = 1 (end vertex)
 * 3. All other vertices have indegree = outdegree
 * 4. All vertices with edges must be connected (when viewed as undirected graph)
 *
 * Time complexity: O(E + V), Space complexity: O(V)
 */
class DirectedEulerianPath {
    std::vector<int> path_;  // The Eulerian path (empty if none exists)

    // Find any vertex with at least one outgoing edge; return -1 if none exists
    static int nonIsolatedVertex(const Digraph& g) {
        for (int v = 0; v < g.V(); v++) {
            if (g.outdegree(v) > 0) {
                return v;
            }
        }
        return -1;
    }

    // Check if digraph satisfies Eulerian path conditions
    // static bool satisfiesNecessaryAndSufficientConditions(const Digraph& g) {
    //     // Special case: empty graph has trivial Eulerian path
    //     if (g.E() == 0) return true;
    //
    //     // Condition 1: At most one vertex can have outdegree > indegree
    //     // and the deficit must be exactly 1
    //     int deficit = 0;
    //     for (int v = 0; v < g.V(); v++) {
    //         if (g.outdegree(v) > g.indegree(v)) {
    //             deficit += (g.outdegree(v) - g.indegree(v));
    //         }
    //     }
    //     if (deficit > 1) return false;
    //
    //     // Condition 2: graph is connected when viewed as undirected graph
    //     // (ignoring isolated vertices)
    //     Graph h(g.V());  // Create undirected version
    //     for (int v = 0; v < g.V(); v++) {
    //         for (const int w : g.adj(v)) {
    //             h.addEdge(v, w);  // Add as undirected edge
    //         }
    //     }
    //
    //     // Check that all non-isolated vertices are connected
    //     const int s = nonIsolatedVertex(g);
    //     if (s == -1) return true;  // No edges, trivially has path
    //
    //     const BreadthFirstPaths bfs(h, s);
    //     for (int v = 0; v < g.V(); v++) {
    //         if (h.degree(v) > 0 && !bfs.hasPathTo(v)) {
    //             return false;
    //         }
    //     }
    //
    //     return true;
    // }

    // Verify the computed solution is correct
    // bool check(const Digraph& g) const {
    //     // Internal consistency check
    //     if (hasEulerianPath() == path_.empty()) return false;
    //
    //     // Check against theoretical conditions
    //     if (hasEulerianPath() != satisfiesNecessaryAndSufficientConditions(g)) {
    //         return false;
    //     }
    //
    //     // No further checks needed if no path exists
    //     if (path_.empty()) return true;
    //
    //     // Path should contain exactly E+1 vertices (visits each edge once)
    //     if (static_cast<int>(path_.size()) != g.E() + 1) return false;
    //
    //     return true;
    // }

public:
    // Constructor: find Eulerian path in the given directed graph
    explicit DirectedEulerianPath(const Digraph& g) {
        // Find vertex from which to start potential Eulerian path:
        // a vertex v with outdegree(v) > indegree(v) if it exists;
        // otherwise a vertex with outdegree(v) > 0
        int deficit = 0;
        int s = nonIsolatedVertex(g);

        for (int v = 0; v < g.V(); v++) {
            if (g.outdegree(v) > g.indegree(v)) {
                deficit += (g.outdegree(v) - g.indegree(v));
                s = v;  // Use this as start vertex
            }
        }

        // Digraph can't have an Eulerian path if deficit > 1
        if (deficit > 1) return;

        // Special case for digraph with zero edges (has degenerate Eulerian path)
        if (s == -1) s = 0;

        // Create local adjacency lists and track current position for each vertex
        std::vector<std::vector<int>> adjCopy(g.V());
        std::vector<int> currentIndex(g.V(), 0);  // Track current position in each adjacency list

        for (int v = 0; v < g.V(); v++) {
            for (int w : g.adj(v)) {
                adjCopy[v].push_back(w);
            }
        }

        // Start Hierholzer's algorithm from chosen start vertex
        std::stack<int> stack;
        stack.push(s);

        // Main algorithm: greedily follow edges, backtrack when stuck
        while (!stack.empty()) {
            // Check if current vertex has unused outgoing edges
            if (int v = stack.top(); currentIndex[v] < static_cast<int>(adjCopy[v].size())) {
                int next = adjCopy[v][currentIndex[v]];  // Get next vertex
                currentIndex[v]++;                       // Mark edge as used
                stack.push(next);                        // Push next vertex to stack
            } else {
                // No more edges from v: remove from stack and add to path
                stack.pop();
                path_.push_back(v);
            }
        }

        // Check if all edges have been used
        if (static_cast<int>(path_.size()) != g.E() + 1) {
            path_.clear();  // Invalid path
        }

        std::reverse(path_.begin(), path_.end());

        // assert(check(g));
    }

    // Return the Eulerian path (empty vector if none exists)
    std::vector<int> path() const {
        return path_;
    }

    // Check if a directed Eulerian path exists
    bool hasEulerianPath() const {
        return !path_.empty();
    }
};
