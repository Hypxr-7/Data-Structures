#pragma once

#include <cassert>
#include <stack>
#include <vector>

// #include "BreadthFirstPaths.hpp"
#include "Digraph.hpp"
// #include "Graph.hpp"

/**
 * Find an Eulerian cycle in a directed graph using Hierholzer's algorithm.
 * An Eulerian cycle visits every edge exactly once and returns to the starting vertex.
 *
 * For a directed graph to have an Eulerian cycle:
 * 1. At least one edge must exist
 * 2. indegree(v) == outdegree(v) for every vertex v
 * 3. All vertices with edges must be connected (when viewed as undirected graph)
 *
 * Time complexity: O(E + V), Space complexity: O(V)
 */
class DirectedEulerianCycle {
    std::vector<int> cycle_;  // The Eulerian cycle (empty if none exists)

    // Find any vertex with at least one outgoing edge; return -1 if none exists
    int nonIsolatedVertex(const Digraph& g) const {
        for (int v = 0; v < g.V(); v++) {
            if (g.outdegree(v) > 0) {
                return v;
            }
        }
        return -1;
    }

    // Check if digraph satisfies Eulerian cycle conditions
    // bool satisfiesNecessaryAndSufficientConditions(const Digraph& g) const {
    //     // Condition 0: at least one edge
    //     if (g.E() == 0) return false;
    //
    //     // Condition 1: indegree(v) == outdegree(v) for every vertex
    //     for (int v = 0; v < g.V(); v++) {
    //         if (g.outdegree(v) != g.indegree(v)) {
    //             return false;
    //         }
    //     }
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
    //     if (s == -1) return false;
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
    // bool certifySolution(const Digraph& g) const {
    //     // Internal consistency check
    //     if (hasEulerianCycle() == cycle_.empty()) return false;
    //
    //     // Check against theoretical conditions
    //     if (hasEulerianCycle() != satisfiesNecessaryAndSufficientConditions(g)) {
    //         return false;
    //     }
    //
    //     // No further checks needed if no cycle exists
    //     if (cycle_.empty()) return true;
    //
    //     // Cycle should contain exactly E+1 vertices (visits each edge once, returns to start)
    //     if (static_cast<int>(cycle_.size()) != g.E() + 1) return false;
    //
    //     // First and last vertices should be the same (forms a cycle)
    //     if (cycle_.front() != cycle_.back()) return false;
    //
    //     return true;
    // }

public:
    // Constructor: find Eulerian cycle in the given directed graph
    explicit DirectedEulerianCycle(const Digraph& g) {
        // Early exit if no edges
        if (g.E() == 0) return;

        // Check necessary condition: indegree == outdegree for all vertices
        for (int v = 0; v < g.V(); v++) {
            if (g.outdegree(v) != g.indegree(v)) {
                return;
            }
        }

        // Create local adjacency lists and track current position for each vertex
        std::vector<std::vector<int>> adjCopy(g.V());
        std::vector<int> currentIndex(g.V(), 0);  // Track current position in each adjacency list

        for (int v = 0; v < g.V(); v++) {
            for (int w : g.adj(v)) {
                adjCopy[v].push_back(w);
            }
        }

        // Start Hierholzer's algorithm from any non-isolated vertex
        const int s = nonIsolatedVertex(g);
        if (s == -1) return;

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
                // No more edges from v: remove from stack and add to cycle
                stack.pop();
                cycle_.push_back(v);
            }
        }

        // Verify we used all edges (cycle length should be E+1)
        if (static_cast<int>(cycle_.size()) != g.E() + 1) {
            cycle_.clear();  // Invalid cycle
        }

        // assert(certifySolution(g));
    }

    // Return the Eulerian cycle (empty vector if none exists)
    std::vector<int> cycle() const {
        return cycle_;
    }

    // Check if a directed Eulerian cycle exists
    bool hasEulerianCycle() const {
        return !cycle_.empty();
    }
};