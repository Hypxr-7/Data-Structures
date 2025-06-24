#pragma once

#include <stdexcept>
#include <vector>

#include "DijkstraSP.hpp"

/**
 * Computes all-pairs shortest paths in edge-weighted directed graphs
 * using Dijkstra's algorithm from each vertex as source.
 *
 * Precomputes shortest path trees from every vertex to all other vertices.
 * Provides O(1) query time for distances and paths after O(V * (E + V) log V) preprocessing.
 *
 * Space complexity: O(V²)
 * Query time: O(1) for distance, O(path length) for path reconstruction
 *
 * Use when multiple shortest path queries are needed between various vertex pairs.
 */
class DijkstraAllPairsSP {
    std::vector<DijkstraSP> all;  // Shortest path trees from each vertex

    /**
     * Validates that vertex v is within valid range.
     * @param v the vertex to validate
     * @throws std::invalid_argument if vertex is out of range
     */
    void validateVertex(const int v) const {
        if (v < 0 || v >= static_cast<int>(all.size()))
            throw std::invalid_argument("Vertex out of range");
    }

public:
    /**
     * Computes all-pairs shortest paths using Dijkstra's algorithm.
     * Runs Dijkstra from each vertex as source.
     *
     * @param g the edge-weighted directed graph
     * @throws std::invalid_argument if any edge has negative weight
     */
    explicit DijkstraAllPairsSP(const EdgeWeightedDigraph& g) {
        all.reserve(g.V());
        for (int v = 0; v < g.V(); ++v) {
            all.emplace_back(g, v);
        }
    }

    /**
     * Returns shortest path from vertex s to vertex v.
     * @param s source vertex
     * @param v destination vertex
     * @return shortest path as vector of directed edges, empty if no path
     * @throws std::invalid_argument if s or v is out of range
     */
    std::vector<DirectedEdge> path(const int s, const int v) const {
        validateVertex(s);
        validateVertex(v);
        return all[s].pathTo(v);
    }

    /**
     * Returns true if there is a path from vertex s to vertex v.
     * @param s source vertex
     * @param v destination vertex
     * @return true if path exists, false otherwise
     * @throws std::invalid_argument if s or v is out of range
     */
    bool hasPath(const int s, const int v) const {
        validateVertex(s);
        validateVertex(v);
        return all[s].hasPathTo(v);
    }

    /**
     * Returns length of shortest path from vertex s to vertex v.
     * @param s source vertex
     * @param v destination vertex
     * @return shortest distance, or infinity if no path
     * @throws std::invalid_argument if s or v is out of range
     */
    double dist(const int s, const int v) const {
        validateVertex(s);
        validateVertex(v);
        return all[s].distTo(v);
    }

    /**
     * Returns the number of vertices.
     * @return number of vertices
     */
    int V() const {
        return static_cast<int>(all.size());
    }
};