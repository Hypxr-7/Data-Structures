#pragma once

#include <stdexcept>
#include <string>

/**
 * Represents a weighted directed edge in an edge-weighted directed graph.
 *
 * A directed edge has:
 * - A source vertex (from)
 * - A destination vertex (to)
 * - A weight (can be positive, negative, or zero)
 *
 * Unlike undirected edges, directed edges have a specific direction:
 * an edge from vertex v to vertex w is different from an edge from w to v.
 *
 * This class is immutable - once created, the edge's vertices and weight cannot be changed.
 *
 * Used in algorithms like:
 * - Shortest path algorithms (Dijkstra, Bellman-Ford, Floyd-Warshall)
 * - Topological sorting
 * - Strongly connected components
 * - Network flow algorithms
 */
class DirectedEdge {
    int v;           // source vertex (from)
    int w;           // destination vertex (to)
    double weight_;  // weight of the edge

public:
    /**
     * Constructs a directed edge from vertex v to vertex w with given weight.
     *
     * @param v the source vertex (must be non-negative)
     * @param w the destination vertex (must be non-negative)
     * @param weight the weight of the edge (can be any real number)
     * @throws std::invalid_argument if either vertex is negative
     */
    DirectedEdge(const int v, const int w, const double weight) : v(v), w(w), weight_(weight) {
        if (v < 0) {
            throw std::invalid_argument("Source vertex must be non-negative, got: " + std::to_string(v));
        }
        if (w < 0) {
            throw std::invalid_argument("Destination vertex must be non-negative, got: " + std::to_string(w));
        }
    }

    /**
     * Returns the source vertex of this directed edge.
     *
     * @return the source vertex
     */
    int from() const {
        return v;
    }

    /**
     * Returns the destination vertex of this directed edge.
     *
     * @return the destination vertex
     */
    int to() const {
        return w;
    }

    /**
     * Returns the weight of this directed edge.
     *
     * @return the weight of the edge
     */
    double weight() const {
        return weight_;
    }

    /**
     * Compares two directed edges for equality.
     * Two edges are equal if they have the same source, destination, and weight.
     *
     * @param other the other directed edge to compare with
     * @return true if edges are equal, false otherwise
     */
    bool operator==(const DirectedEdge& other) const {
        return v == other.v && w == other.w && weight_ == other.weight_;
    }

    /**
     * Compares two directed edges for inequality.
     *
     * @param other the other directed edge to compare with
     * @return true if edges are not equal, false otherwise
     */
    bool operator!=(const DirectedEdge& other) const {
        return !(*this == other);
    }

    /**
     * Compares directed edges by weight (for sorting/priority queues).
     * This enables use in algorithms that need to process edges by weight.
     *
     * @param other the other directed edge to compare with
     * @return true if this edge has smaller weight than other
     */
    bool operator<(const DirectedEdge& other) const {
        return weight_ < other.weight_;
    }

    /**
     * Compares directed edges by weight.
     *
     * @param other the other directed edge to compare with
     * @return true if this edge has larger weight than other
     */
    bool operator>(const DirectedEdge& other) const {
        return weight_ > other.weight_;
    }

    /**
     * Compares directed edges by weight.
     *
     * @param other the other directed edge to compare with
     * @return true if this edge has weight less than or equal to other
     */
    bool operator<=(const DirectedEdge& other) const {
        return weight_ <= other.weight_;
    }

    /**
     * Compares directed edges by weight.
     *
     * @param other the other directed edge to compare with
     * @return true if this edge has weight greater than or equal to other
     */
    bool operator>=(const DirectedEdge& other) const {
        return weight_ >= other.weight_;
    }
    /**
     * Stream insertion operator for DirectedEdge.
     * Allows printing DirectedEdge objects using std::cout.
     *
     * @param os the output stream
     * @param edge the directed edge to print
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const DirectedEdge& edge) {
        os << std::to_string(edge.v) + "->" + std::to_string(edge.w) + " " + std::to_string(edge.weight_);
        return os;
    }
};

