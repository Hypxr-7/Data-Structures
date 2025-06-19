#pragma once

#include <stdexcept>
#include <vector>

#include "Graph.hpp"

class ConnectedComponents {
    std::vector<bool> marked;   // marked[v] = true if vertex v has been visited
    std::vector<int> id_;       // id_[v] = component identifier for vertex v
    std::vector<int> size_;     // size_[i] = number of vertices in component i
    int count_;                 // number of connected components

    // DFS to mark all vertices in the same component
    void dfs(const Graph& g, const int v) {
        marked[v] = true;
        id_[v] = count_;
        ++size_[count_];

        for (const int w : g.adj(v)) {
            if (!marked[w]) {
                dfs(g, w);
            }
        }
    }

    // Validate vertex index is within bounds
    void validateVertex(const int v) const {
        if (v < 0 || v >= static_cast<int>(marked.size())) {
            throw std::invalid_argument("Vertex out of range");
        }
    }

public:
    // Constructor: find all connected components in graph g
    explicit ConnectedComponents(const Graph& g) :
        marked(g.V(), false),
        id_(g.V()),
        size_(g.V(), 0),
        count_(0) {

        for (int v = 0; v < g.V(); ++v) {
            if (!marked[v]) {
                dfs(g, v);
                ++count_;
            }
        }
    }

    // Get component identifier for vertex v
    int id(const int v) const {
        validateVertex(v);
        return id_[v];
    }

    // Get size of component containing vertex v
    int size(const int v) const {
        validateVertex(v);
        return size_[id_[v]];
    }

    // Get total number of connected components
    int count() const {
        return count_;
    }

    // Check if vertices v and w are in the same connected component
    bool connected(const int v, const int w) const {
        validateVertex(v);
        validateVertex(w);
        return id_[v] == id_[w];
    }
};