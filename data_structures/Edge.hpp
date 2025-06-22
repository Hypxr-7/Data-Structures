#pragma once

#include <stdexcept>

/**
 * Represents a weighted edge in an undirected graph.
 *
 * An edge connects two vertices with an associated weight (typically representing
 * distance, cost, capacity, etc.). This class is commonly used in algorithms like:
 * - Minimum Spanning Tree (Kruskal's, Prim's)
 * - Shortest Path algorithms
 * - Maximum Flow algorithms
 *
 * The edge is undirected, meaning edge (v, w) is the same as edge (w, v).
 * Comparison operators are based solely on edge weights for use in priority queues
 * and sorting algorithms.
 */
class Edge {
    int v;        // first vertex of the edge
    int w;        // second vertex of the edge
    double weight_; // weight of the edge

public:
    // Constructor: create an edge between vertices v and w with given weight
    Edge(const int v_, const int w_, const double weight) : v(v_), w(w_), weight_(weight) {
        if (v < 0 || w < 0) throw std::invalid_argument("Vertex must be non-negative");
    }

    // Get the weight of this edge
    double weight() const {
        return weight_;
    }

    // Get either endpoint of the edge (returns vertex v)
    // Useful when you need any vertex from the edge
    int either() const {
        return v;
    }

    // Get the other endpoint of the edge given one vertex
    // If given vertex v, returns w; if given vertex w, returns v
    int other(const int vertex) const {
        if (vertex == v) return w;
        if (vertex == w) return v;
        throw std::invalid_argument("Vertex not in edge");
    }

    // Comparison operators based on edge weight
    // Used for sorting edges and priority queue operations in MST algorithms

    bool operator<(const Edge& other) const {
        return this->weight_ < other.weight_;
    }

    bool operator>(const Edge& other) const {
        return this->weight_ > other.weight_;
    }

    bool operator<=(const Edge& other) const {
        return this->weight_ <= other.weight_;
    }

    bool operator>=(const Edge& other) const {
        return this->weight_ >= other.weight_;
    }

    bool operator==(const Edge& other) const {
        return this->weight_ == other.weight_;
    }

    bool operator!=(const Edge& other) const {
        return this->weight_ != other.weight_;
    }
};