#pragma once

#include <cassert>
#include <limits>
#include <optional>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include "DirectedEdge.hpp"
#include "EdgeWeightedDigraph.hpp"
#include "IndexMinPQ.hpp"

/**
 * Computes shortest paths from a source vertex to all other vertices
 * in an edge-weighted directed graph using Dijkstra's algorithm.
 *
 * Dijkstra's algorithm is a greedy algorithm that finds the shortest path tree
 * from a single source vertex to all other vertices in a weighted graph with
 * non-negative edge weights.
 *
 * Algorithm overview:
 * 1. Initialize distances to all vertices as infinity, except source (distance 0)
 * 2. Maintain a priority queue of vertices ordered by their current shortest distance
 * 3. Repeatedly extract the vertex with minimum distance and relax all its outgoing edges
 * 4. Relaxation: if path through current vertex is shorter, update distance and predecessor
 *
 * Key properties:
 * - Requires non-negative edge weights (throws exception if negative weights found)
 * - Produces shortest path tree rooted at source vertex
 * - Time complexity: O((V + E) log V) with binary heap, O(V² + E) with Fibonacci heap
 * - Space complexity: O(V)
 *
 * Applications:
 * - GPS navigation and route planning
 * - Network routing protocols
 * - Social network analysis (shortest connection paths)
 * - Game AI pathfinding
 *
 * Limitations:
 * - Cannot handle negative edge weights (use Bellman-Ford for that case)
 * - Single-source algorithm (use Floyd-Warshall for all-pairs shortest paths)
 */
class DijkstraSP {
private:
    std::vector<std::optional<DirectedEdge>> edgeTo;  // edgeTo[v] = last edge on shortest path from s to v
    std::vector<double> distTo_;                      // distTo[v] = distance of shortest path from s to v
    IndexMinPQ<double> pq;                           // priority queue of vertices

    /**
     * Relaxes the given directed edge.
     * Relaxation checks if the path to the destination vertex through the source vertex
     * of this edge is shorter than the currently known shortest path.
     *
     * @param e the directed edge to relax
     */
    void relax(const DirectedEdge& e) {
        int v = e.from();
        int w = e.to();

        // If path v → w is shorter than current known path to w
        if (distTo_[w] > distTo_[v] + e.weight()) {
            distTo_[w] = distTo_[v] + e.weight();
            edgeTo[w] = e;

            // Update priority queue with new shorter distance
            if (pq.contains(w)) {
                pq.decreaseKey(w, distTo_[w]);
            } else {
                pq.insert(w, distTo_[w]);
            }
        }
    }

    /**
     * Verifies the correctness of the computed shortest path tree.
     * Checks optimality conditions and data structure invariants.
     *
     * @param g the edge-weighted digraph
     * @param s the source vertex
     * @return true if shortest path tree is valid, false otherwise
     */
    bool check(const EdgeWeightedDigraph& g, const int s) const {
        // Check 1: All edge weights must be non-negative
        for (const auto& e : g.edges()) {
            if (e.weight() < 0) {
                return false;
            }
        }

        // Check 2: Source vertex should have distance 0 and no predecessor
        if (distTo_[s] != 0.0 || edgeTo[s].has_value()) {
            return false;
        }

        // Check 3: Unreachable vertices should have infinite distance and no predecessor
        for (int v = 0; v < g.V(); ++v) {
            if (v == s) continue;
            if (!edgeTo[v].has_value() && distTo_[v] != std::numeric_limits<double>::max()) {
                return false;
            }
        }

        // Check 4: Optimality conditions - no edge should violate shortest path property
        for (int v = 0; v < g.V(); ++v) {
            for (const auto& e : g.adj(v)) {
                int w = e.to();
                if (distTo_[v] + e.weight() < distTo_[w]) {
                    return false;  // Found a shorter path, algorithm failed
                }
            }
        }

        // Check 5: Edge-to consistency - each edge in SPT should satisfy optimality
        for (int w = 0; w < g.V(); ++w) {
            if (!edgeTo[w].has_value()) continue;
            DirectedEdge e = edgeTo[w].value();
            int v = e.from();
            if (w != e.to()) return false;                           // Edge should point to w
            if (distTo_[v] + e.weight() != distTo_[w]) return false; // Distance should be exact
        }

        return true;
    }

    /**
     * Validates that vertex v is within valid range.
     *
     * @param v the vertex to validate
     * @throws std::invalid_argument if vertex is out of range
     */
    void validateVertex(const int v) const {
        int V = static_cast<int>(edgeTo.size());
        if (v < 0 || v >= V) {
            throw std::invalid_argument("Vertex " + std::to_string(v) +
                                      " is out of range [0, " + std::to_string(V) + ")");
        }
    }

public:
    /**
     * Computes shortest paths from source vertex s to every other vertex
     * in the edge-weighted digraph g using Dijkstra's algorithm.
     *
     * @param g the edge-weighted digraph
     * @param s the source vertex (must be valid vertex in g)
     * @throws std::invalid_argument if any edge has negative weight
     * @throws std::invalid_argument if source vertex is invalid
     */
    DijkstraSP(const EdgeWeightedDigraph& g, const int s) :
        edgeTo(g.V()),
        distTo_(g.V(), std::numeric_limits<double>::max()),
        pq(g.V()) {

        // Validate source vertex
        if (s < 0 || s >= g.V()) {
            throw std::invalid_argument("Source vertex " + std::to_string(s) +
                                      " is out of range [0, " + std::to_string(g.V()) + ")");
        }

        // Check for negative edge weights (Dijkstra requires non-negative weights)
        for (const auto& e : g.edges()) {
            if (e.weight() < 0) {
                throw std::invalid_argument("Edge has negative weight");
            }
        }

        // Initialize: source has distance 0, all others have infinite distance
        distTo_[s] = 0.0;
        pq.insert(s, distTo_[s]);

        // Main algorithm loop
        while (!pq.empty()) {
            const int v = pq.removeMin();  // Extract vertex with minimum distance

            // Relax all edges leaving v
            for (const auto& e : g.adj(v)) {
                relax(e);
            }
        }

        // Verify correctness of computed shortest path tree
        assert(check(g, s));
    }

    /**
     * Returns the length of the shortest path from source vertex to vertex v.
     *
     * @param v the destination vertex
     * @return the length of the shortest path from source to v,
     *         or positive infinity if no path exists
     * @throws std::invalid_argument if vertex v is invalid
     */
    double distTo(const int v) const {
        validateVertex(v);
        return distTo_[v];
    }

    /**
     * Returns true if there is a path from source vertex to vertex v.
     *
     * @param v the destination vertex
     * @return true if there is a path from source to v, false otherwise
     * @throws std::invalid_argument if vertex v is invalid
     */
    bool hasPathTo(const int v) const {
        validateVertex(v);
        return distTo_[v] < std::numeric_limits<double>::max();
    }

    /**
     * Returns the shortest path from source vertex to vertex v.
     * The path is returned as a sequence of directed edges.
     *
     * @param v the destination vertex
     * @return the shortest path from source to v as a vector of DirectedEdge,
     *         or empty vector if no path exists
     * @throws std::invalid_argument if vertex v is invalid
     */
    std::vector<DirectedEdge> pathTo(const int v) const {
        validateVertex(v);
        if (!hasPathTo(v)) return std::vector<DirectedEdge>{};

        std::vector<DirectedEdge> path;

        // Build path by following edgeTo[] backwards from v to source
        for (std::optional<DirectedEdge> e = edgeTo[v]; e.has_value(); e = edgeTo[e->from()]) {
            path.push_back(*e);
        }

        // Reverse path so it goes from source to destination
        std::reverse(path.begin(), path.end());
        return path;
    }

    /**
     * Returns the source vertex of this shortest path tree.
     *
     * @return the source vertex
     */
    int source() const {
        // Find the vertex with distance 0 (the source)
        for (int v = 0; v < distTo_.size(); ++v) {
            if (distTo_[v] == 0.0) {
                return v;
            }
        }
        return -1;  // Should never happen if properly constructed
    }

    /**
     * Returns the number of vertices in the shortest path tree.
     *
     * @return the number of vertices
     */
    int V() const {
        return static_cast<int>(distTo_.size());
    }
};