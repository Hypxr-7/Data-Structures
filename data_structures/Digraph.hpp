#pragma once

#include <iostream>
#include <list>
#include <stdexcept>
#include <vector>

class Digraph {
    std::vector<std::list<int>> adj_;  // Adjacency lists for each vertex
    const int V_;                      // Number of vertices
    int E_;                           // Number of edges
    std::vector<int> indegree_;       // In-degree count for each vertex

    // Validates that vertex is in valid range [0, V)
    void validateVertex(const int v) const {
        if (v < 0 || v >= V_)
            throw std::invalid_argument("Vertex out of range");
    }

    int validateVertexInit(const int v) const {
        if (v < 0) throw std::invalid_argument("Vertices must be non-negative");
        return v;
    }


public:
    // Constructor: creates digraph with V vertices and 0 edges
    explicit Digraph(const int v) : adj_(validateVertexInit(v)), V_(v), E_(0), indegree_(v, 0) {}

    // Returns number of vertices
    int V() const { return V_; }

    // Returns number of edges
    int E() const { return E_; }

    // Adds directed edge v->w
    void addEdge(const int v, const int w) {
        validateVertex(v);
        validateVertex(w);
        adj_[v].push_front(w);
        ++indegree_[w];
        ++E_;
    }

    // Returns adjacency list for vertex v
    std::vector<int> adj(const int v) const {
        validateVertex(v);
        return std::vector<int>{adj_[v].begin(), adj_[v].end()};
    }

    // Returns out-degree of vertex v
    int outdegree(const int v) const {
        validateVertex(v);
        return static_cast<int>(adj_[v].size());
    }

    // Returns in-degree of vertex v
    int indegree(const int v) const {
        validateVertex(v);
        return indegree_[v];
    }

    // Returns reverse digraph
    Digraph reverse() const {
        Digraph rev(V_);
        for (int v = 0; v < V_; ++v)
            for (const int w : adj_[v])
                rev.addEdge(w, v);
        return rev;
    }

    // Stream output operator
    friend std::ostream& operator<<(std::ostream& os, const Digraph& graph) {
        os << "Number of Vertices: " << graph.V() << '\n';
        os << "Number of Edges: " << graph.E() << '\n';

        for (int v = 0; v < graph.V(); ++v) {
            os << "V: " << v << "\tE: ";
            std::vector<int> neighbors = graph.adj(v);
            for (const int neighbor : neighbors) {
                os << neighbor << ' ';
            }
            os << '\n';
        }
        return os;
    }
};
