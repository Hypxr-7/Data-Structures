#pragma once

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <vector>

#include "Digraph.hpp"

/**
 * Computes depth-first search preorder and postorder for vertices in a digraph.
 * This is useful for topological sorting (reverse postorder) and other graph algorithms.
 *
 * Preorder: order in which vertices are first encountered during DFS
 * Postorder: order in which vertices are finished (all descendants processed)
 * Reverse postorder: topological ordering for DAGs
 */
class DepthFirstOrder {
    std::vector<bool> marked;      // marked[v] = true if vertex v has been visited
    std::vector<int> pre_;         // pre_[v] = preorder number of vertex v
    std::vector<int> post_;        // post_[v] = postorder number of vertex v
    std::vector<int> preorder;     // vertices in preorder (order of first visit)
    std::vector<int> postorder;    // vertices in postorder (order of completion)
    int preCount;                  // counter for preorder numbering
    int postCount;                 // counter for postorder numbering

    // Recursive DFS to compute pre and postorder
    void dfs(const Digraph& g, const int v) {
        marked[v] = true;
        pre_[v] = preCount++;           // Assign preorder number
        preorder.push_back(v);          // Add to preorder sequence

        // Recursively visit all unvisited adjacent vertices
        for (const int w : g.adj(v)) {
            if (!marked[w]) {
                dfs(g, w);
            }
        }

        // After processing all descendants, add to postorder
        postorder.push_back(v);
        post_[v] = postCount++;         // Assign postorder number
    }

    // Verify the correctness of pre and postorder numbering
    bool check() const {
        // Check postorder: vertices should appear in order of their postorder numbers
        int r = 0;
        for (const int v : postorder) {
            if (post_[v] != r) {
                return false;
            }
            ++r;
        }

        // Check preorder: vertices should appear in order of their preorder numbers
        r = 0;
        for (const int v : preorder) {
            if (pre_[v] != r) {
                return false;
            }
            ++r;
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
    // Constructor: compute DFS preorder and postorder for the digraph
    explicit DepthFirstOrder(const Digraph& g) :
        marked(g.V(), false),
        pre_(g.V()),
        post_(g.V()),
        preCount(0),
        postCount(0) {

        // Run DFS from each unvisited vertex (handles disconnected components)
        for (int v = 0; v < g.V(); ++v) {
            if (!marked[v]) {
                dfs(g, v);
            }
        }
        // assert(check());
    }

    // Get preorder number of vertex v
    int pre(const int v) const {
        validateVertex(v);
        return pre_[v];
    }

    // Get all vertices in preorder
    std::vector<int> pre() const {
        return preorder;
    }

    // Get postorder number of vertex v
    int post(const int v) const {
        validateVertex(v);
        return post_[v];
    }

    // Get all vertices in postorder
    std::vector<int> post() const {
        return postorder;
    }

    // Get vertices in reverse postorder (topological order for DAGs)
    std::vector<int> reversePost() const {
        std::vector<int> rev;
        rev.reserve(postorder.size());
        std::reverse_copy(postorder.begin(), postorder.end(), std::back_inserter(rev));
        return rev;
    }
};