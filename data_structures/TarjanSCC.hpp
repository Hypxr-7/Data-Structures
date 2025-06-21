#pragma once

#include <algorithm>
#include <cassert>
#include <stack>
#include <stdexcept>
#include <vector>

#include "Digraph.hpp"
// #include "TransitiveClosure.hpp"

/**
 * Computes strongly connected components (SCCs) of a directed graph using
 * Tarjan's algorithm.
 *
 * A strongly connected component is a maximal set of vertices such that
 * there is a directed path from every vertex to every other vertex in the set.
 *
 * Tarjan's algorithm uses a single DFS traversal with a stack to identify SCCs.
 * It maintains low-link values to detect when a complete SCC has been found.
 *
 * Algorithm:
 * 1. Perform DFS and maintain discovery times and low-link values
 * 2. Use a stack to keep track of vertices in current path
 * 3. When a vertex's low-link equals its discovery time, it's an SCC root
 * 4. Pop all vertices from stack until reaching the root - they form one SCC
 *
 * Time complexity: O(V + E), Space complexity: O(V)
 */
class TarjanSCC {
    std::vector<bool> marked;   // marked[v] = true if vertex v has been visited
    std::vector<int> id_;       // id_[v] = strongly connected component id of vertex v
    std::vector<int> low;       // low[v] = lowest preorder number reachable from v
    int pre;                    // counter for preorder numbering
    int count_;                 // number of strongly connected components
    std::stack<int> stack;      // stack of vertices in current DFS path

    // DFS to find strongly connected components using Tarjan's algorithm
    void dfs(const Digraph& g, const int v) {
        marked[v] = true;
        low[v] = pre++;         // Initialize low-link value
        int min = low[v];       // Track minimum low-link in subtree
        stack.push(v);          // Add vertex to stack

        // Explore all adjacent vertices
        for (const int w : g.adj(v)) {
            if (!marked[w]) {
                dfs(g, w);      // Recursive DFS
            }
            // Update minimum low-link value considering edge v->w
            min = std::min(low[w], min);
        }

        // Update low-link value
        if (min < low[v]) {
            low[v] = min;
            return;
        }

        // If low[v] == preorder[v], then v is root of an SCC
        // Pop all vertices from stack until v to form the SCC
        int w;
        do {
            w = stack.top();
            stack.pop();
            id_[w] = count_;         // Assign SCC id
            low[w] = g.V();          // Mark as processed (set to infinity)
        } while (w != v);
        ++count_;
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
    // Constructor: compute strongly connected components using Tarjan's algorithm
    explicit TarjanSCC(const Digraph& g) :
        marked(g.V(), false),
        id_(g.V(), -1),
        low(g.V()),
        pre(0),
        count_(0) {

        // Run DFS from each unvisited vertex
        for (int v = 0; v < g.V(); ++v) {
            if (!marked[v]) {
                dfs(g, v);
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
