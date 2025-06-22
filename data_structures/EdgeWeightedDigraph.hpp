#pragma once

#include <list>
#include <random>
#include <stdexcept>
#include <vector>

#include "DirectedEdge.hpp"

/**
 * Represents an edge-weighted directed graph (digraph).
 *
 * A directed graph consists of:
 * - A set of vertices (numbered 0 to V-1)
 * - A set of directed edges, each with a weight
 * - Each edge goes from one vertex to another (direction matters)
 *
 * Key properties:
 * - Parallel edges are allowed (multiple edges between same vertices)
 * - Self-loops are allowed (edges from vertex to itself)
 * - Edge weights can be positive, negative, or zero
 * - Adjacency list representation for efficient edge enumeration
 *
 * Used in algorithms like:
 * - Shortest path algorithms (Dijkstra, Bellman-Ford, Floyd-Warshall)
 * - Topological sorting
 * - Strongly connected components (Kosaraju, Tarjan)
 * - Network flow algorithms
 * - Cycle detection in directed graphs
 *
 * Time complexities:
 * - Add edge: O(1)
 * - Get adjacent edges: O(outdegree(v))
 * - Get all edges: O(V + E)
 *
 * Space complexity: O(V + E)
 */
class EdgeWeightedDigraph {
    std::vector<std::list<DirectedEdge>> adj_;  // adjacency lists for each vertex
    int V_;                                     // number of vertices
    int E_;                                     // number of edges
    std::vector<int> indegree_;                // indegree of each vertex

    /**
     * Validates that vertex v is in valid range [0, V_).
     *
     * @param v the vertex to validate
     * @throws std::invalid_argument if vertex is out of range
     */
    void validateVertex(const int v) const {
        if (v < 0 || v >= V_) {
            throw std::invalid_argument("Vertex " + std::to_string(v) +
                                      " is out of range [0, " + std::to_string(V_) + ")");
        }
    }

    /**
     * Validates vertex count during construction.
     *
     * @param v the number of vertices
     * @return v if valid
     * @throws std::invalid_argument if v is negative
     */
    int validateConstructorInit(const int v) const {
        if (v < 0) {
            throw std::invalid_argument("Number of vertices cannot be negative, got: " + std::to_string(v));
        }
        return v;
    }

public:
    /**
     * Constructs an empty edge-weighted digraph with V vertices and 0 edges.
     * Vertices are numbered 0 through V-1.
     *
     * @param v the number of vertices
     * @throws std::invalid_argument if v is negative
     */
    explicit EdgeWeightedDigraph(const int v) :
        adj_(validateConstructorInit(v)),
        V_(v),
        E_(0),
        indegree_(v, 0) {}

    /**
     * Constructs a random edge-weighted digraph with V vertices and E edges.
     * Edge weights are uniformly distributed in [0.0, 1.0).
     * Vertices for each edge are chosen uniformly at random.
     *
     * @param v the number of vertices
     * @param e the number of edges
     * @throws std::invalid_argument if v is negative or e is negative
     */
    EdgeWeightedDigraph(const int v, const int e) :
        adj_(validateConstructorInit(v)),
        V_(v),
        E_(0),
        indegree_(v, 0) {

        if (e < 0) {
            throw std::invalid_argument("Number of edges cannot be negative, got: " + std::to_string(e));
        }

        // Use a fixed seed for reproducible random graphs in testing
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<> vertexDist(0, v - 1);
        std::uniform_real_distribution<> weightDist(0.0, 1.0);

        for (int i = 0; i < e; ++i) {
            const int u = vertexDist(rng);
            const int w = vertexDist(rng);
            const double weight = weightDist(rng);
            DirectedEdge edge{u, w, weight};
            addEdge(edge);
        }
    }

    /**
     * Returns the number of vertices in this digraph.
     *
     * @return the number of vertices
     */
    int V() const {
        return V_;
    }

    /**
     * Returns the number of edges in this digraph.
     *
     * @return the number of edges
     */
    int E() const {
        return E_;
    }

    /**
     * Adds the directed edge to this digraph.
     *
     * @param e the directed edge to add
     * @throws std::invalid_argument if either vertex is invalid
     */
    void addEdge(const DirectedEdge& e) {
        const int v = e.from();
        const int w = e.to();
        validateVertex(v);
        validateVertex(w);

        adj_[v].push_front(e);  // Add edge to adjacency list of source vertex
        ++indegree_[w];         // Increment indegree of destination vertex
        ++E_;                   // Increment total edge count
    }

    /**
     * Returns the directed edges incident from vertex v.
     *
     * @param v the vertex
     * @return the directed edges incident from vertex v as a vector
     * @throws std::invalid_argument if vertex is invalid
     */
    std::vector<DirectedEdge> adj(const int v) const {
        validateVertex(v);
        return std::vector<DirectedEdge>{adj_[v].begin(), adj_[v].end()};
    }

    /**
     * Returns the number of directed edges incident from vertex v.
     * This is equal to the outdegree of vertex v.
     *
     * @param v the vertex
     * @return the outdegree of vertex v
     * @throws std::invalid_argument if vertex is invalid
     */
    int outdegree(const int v) const {
        validateVertex(v);
        return static_cast<int>(adj_[v].size());
    }

    /**
     * Returns the number of directed edges incident to vertex v.
     * This is equal to the indegree of vertex v.
     *
     * @param v the vertex
     * @return the indegree of vertex v
     * @throws std::invalid_argument if vertex is invalid
     */
    int indegree(const int v) const {
        validateVertex(v);
        return indegree_[v];
    }

    /**
     * Returns all directed edges in this digraph.
     * To iterate over the edges incident from vertex v, use adj(v) instead.
     *
     * @return all directed edges in this digraph as a vector
     */
    std::vector<DirectedEdge> edges() const {
        std::vector<DirectedEdge> edgeList;
        edgeList.reserve(E_);

        for (int v = 0; v < V_; ++v) {
            for (const DirectedEdge& e : adj_[v]) {
                edgeList.push_back(e);
            }
        }
        return edgeList;
    }

    /**
     * Returns the reverse of this digraph.
     * The reverse digraph has the same vertices but all edge directions reversed.
     *
     * @return the reverse of this digraph
     */
    EdgeWeightedDigraph reverse() const {
        EdgeWeightedDigraph reverse(V_);
        for (int v = 0; v < V_; ++v) {
            for (const DirectedEdge& e : adj_[v]) {
                // Create reversed edge: to -> from with same weight
                reverse.addEdge(DirectedEdge(e.to(), e.from(), e.weight()));
            }
        }
        return reverse;
    }
};
