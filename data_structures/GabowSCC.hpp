#pragma once

#include <cassert>
#include <stack>
#include <stdexcept>
#include <vector>

#include "Digraph.hpp"
// #include "TransitiveClosure.hpp"

/**
 * Computes strongly connected components (SCCs) of a directed graph using
 * Gabow's algorithm (also known as Cheriyan-Mehlhorn/Gabow algorithm).
 *
 * A strongly connected component is a maximal set of vertices such that
 * there is a directed path from every vertex to every other vertex in the set.
 *
 * Gabow's algorithm uses two stacks during a single DFS traversal:
 * 1. Stack1: maintains all vertices in current DFS path
 * 2. Stack2: maintains potential SCC roots (vertices that could be SCC leaders)
 *
 * Algorithm:
 * 1. Perform DFS and assign preorder numbers
 * 2. Use stack1 to track current path, stack2 to track potential SCC roots
 * 3. When returning from DFS, if current vertex is top of stack2, it's an SCC root
 * 4. Pop vertices from stack1 until reaching the root - they form one SCC
 *
 * Time complexity: O(V + E), Space complexity: O(V)
 */
class GabowSCC {
    std::vector<bool> marked;    // marked[v] = true if vertex v has been visited
    std::vector<int> id_;        // id_[v] = strongly connected component id of vertex v
    std::vector<int> preorder;   // preorder[v] = preorder number of vertex v
    int pre;                     // counter for preorder numbering
    int count_;                  // number of strongly connected components
    std::stack<int> stack1;      // stack of all vertices in current DFS path
    std::stack<int> stack2;      // stack of potential SCC roots

    // DFS to find strongly connected components using Gabow's two-stack algorithm
    void dfs(const Digraph& g, const int v) {
        marked[v] = true;
        preorder[v] = pre++;     // Assign preorder number
        stack1.push(v);          // Add to path stack
        stack2.push(v);          // Add as potential SCC root

        // Explore all adjacent vertices
        for (const int w : g.adj(v)) {
            if (!marked[w]) {
                dfs(g, w);       // Recursive DFS for unvisited vertices
            }
            else if (id_[w] == -1) {  // w is visited but not yet assigned to an SCC
                // Remove vertices from stack2 that cannot be roots of SCC containing w
                while (!stack2.empty() && preorder[stack2.top()] > preorder[w]) {
                    stack2.pop();
                }
            }
        }

        // If v is still on top of stack2, it's the root of an SCC
        if (!stack2.empty() && stack2.top() == v) {
            stack2.pop();
            int w;
            // Pop all vertices from stack1 until v to form the SCC
            do {
                w = stack1.top();
                stack1.pop();
                id_[w] = count_;  // Assign SCC id
            } while (w != v);
            ++count_;
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
    // Constructor: compute strongly connected components using Gabow's algorithm
    explicit GabowSCC(const Digraph& g) :
        marked(g.V(), false),
        id_(g.V(), -1),          // Initialize to -1 to indicate unprocessed
        preorder(g.V()),
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