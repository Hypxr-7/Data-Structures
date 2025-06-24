#pragma once

#include <cassert>
#include <limits>
#include <optional>
#include <stack>
#include <stdexcept>
#include <vector>

#include "Edge.hpp"
#include "EdgeWeightedGraph.hpp"
#include "IndexMinPQ.hpp"

/**
 * Computes shortest paths from a source vertex to all other vertices
 * in an edge-weighted undirected graph using Dijkstra's algorithm.
 *
 * Dijkstra's algorithm is a greedy algorithm that finds the shortest path tree
 * from a single source vertex to all other vertices in a weighted graph with
 * non-negative edge weights.
 *
 * Algorithm overview:
 * 1. Initialize distances to all vertices as infinity, except source (distance 0)
 * 2. Maintain a priority queue of vertices ordered by their current shortest distance
 * 3. Repeatedly extract the vertex with minimum distance and relax all its incident edges
 * 4. Relaxation: if path through current vertex is shorter, update distance and predecessor
 *
 * Key differences from directed version:
 * - Uses undirected edges (Edge class instead of DirectedEdge)
 * - Each edge can be traversed in both directions
 * - Relaxation considers both endpoints of each edge
 *
 * Key properties:
 * - Requires non-negative edge weights (throws exception if negative weights found)
 * - Produces shortest path tree rooted at source vertex
 * - Time complexity: O((V + E) log V) with binary heap
 * - Space complexity: O(V)
 *
 * Limitations:
 * - Cannot handle negative edge weights (use Bellman-Ford for that case)
 * - Single-source algorithm (use Floyd-Warshall for all-pairs shortest paths)
 */
class DijkstraUndirectedSP {
    std::vector<double> distTo_;                    // distTo_[v] = distance of shortest path from s to v
    std::vector<std::optional<Edge>> edgeTo_;       // edgeTo_[v] = last edge on shortest path from s to v
    IndexMinPQ<double> pq_;                        // priority queue of vertices

    /**
     * Relaxes the given undirected edge from vertex v.
     * Relaxation checks if the path to the other endpoint through vertex v
     * is shorter than the currently known shortest path.
     *
     * @param e the undirected edge to relax
     * @param v the vertex from which we're relaxing the edge
     */
    void relax(const Edge& e, const int v) {
        const int w = e.other(v);  // Get the other endpoint of the edge

        // If path v → w is shorter than current known path to w
        if (distTo_[w] > distTo_[v] + e.weight()) {
            distTo_[w] = distTo_[v] + e.weight();
            edgeTo_[w] = e;

            // Update priority queue with new shorter distance
            if (pq_.contains(w)) {
                pq_.decreaseKey(w, distTo_[w]);
            } else {
                pq_.insert(w, distTo_[w]);
            }
        }
    }

    /**
     * Verifies the correctness of the computed shortest path tree.
     * Checks optimality conditions and data structure invariants.
     *
     * @param g the edge-weighted undirected graph
     * @param s the source vertex
     * @return true if shortest path tree is valid, false otherwise
     */
    // bool check(const EdgeWeightedGraph& g, const int s) const {
    //     // Check 1: All edge weights must be non-negative
    //     for (const auto& e : g.edges()) {
    //         if (e.weight() < 0) {
    //             return false;
    //         }
    //     }
    //
    //     // Check 2: Source vertex should have distance 0 and no predecessor
    //     if (distTo_[s] != 0.0 || edgeTo_[s].has_value()) {
    //         return false;
    //     }
    //
    //     // Check 3: Unreachable vertices should have infinite distance and no predecessor
    //     for (int v = 0; v < g.V(); ++v) {
    //         if (v == s) continue;
    //         if (!edgeTo_[v].has_value() && distTo_[v] != std::numeric_limits<double>::max()) {
    //             return false;
    //         }
    //     }
    //
    //     // Check 4: Optimality conditions - no edge should violate shortest path property
    //     for (int v = 0; v < g.V(); ++v) {
    //         for (const auto& e : g.adj(v)) {
    //             const int w = e.other(v);
    //             if (distTo_[v] + e.weight() < distTo_[w]) {
    //                 return false;
    //             }
    //         }
    //     }
    //
    //     // Check 5: Edge-to consistency - each edge in SPT should satisfy optimality
    //     for (int w = 0; w < g.V(); ++w) {
    //         if (!edgeTo_[w].has_value()) continue;
    //         Edge e = edgeTo_[w].value();
    //
    //         // Check that w is one of the endpoints of edge e
    //         if (w != e.either() && w != e.other(e.either())) {
    //             return false;
    //         }
    //
    //         const int v = e.other(w);
    //         if (distTo_[v] + e.weight() != distTo_[w]) {
    //             return false;
    //         }
    //     }
    //
    //     return true;
    // }

    /**
     * Validates that vertex v is within valid range.
     *
     * @param v the vertex to validate
     * @throws std::invalid_argument if vertex is out of range
     */
    void validateVertex(const int v) const {
        int V = static_cast<int>(distTo_.size());
        if (v < 0 || v >= V) {
            throw std::invalid_argument("Vertex out of range");
        }
    }

public:
    /**
     * Computes shortest paths from source vertex s to every other vertex
     * in the edge-weighted undirected graph g using Dijkstra's algorithm.
     *
     * @param g the edge-weighted undirected graph
     * @param s the source vertex (must be valid vertex in g)
     * @throws std::invalid_argument if any edge has negative weight
     * @throws std::invalid_argument if source vertex is invalid
     */
    DijkstraUndirectedSP(const EdgeWeightedGraph& g, const int s) :
        distTo_(g.V(), std::numeric_limits<double>::max()),
        edgeTo_(g.V()),
        pq_(g.V()) {

        // Check for negative edge weights (Dijkstra requires non-negative weights)
        for (const auto& e : g.edges()) {
            if (e.weight() < 0) {
                throw std::invalid_argument("Edge has negative weight");
            }
        }

        // Validate source vertex
        validateVertex(s);

        // Initialize: source has distance 0, all others have infinite distance
        distTo_[s] = 0.0;

        // Start with source vertex in priority queue
        pq_.insert(s, distTo_[s]);

        // Main algorithm loop
        while (!pq_.empty()) {
            int v = pq_.removeMin();  // Extract vertex with minimum distance

            // Relax all edges incident to v
            for (const auto& e : g.adj(v)) {
                relax(e, v);
            }
        }

        // Verify correctness of computed shortest path tree
        // assert(check(g, s));
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
     * The path is returned as a sequence of undirected edges.
     *
     * @param v the destination vertex
     * @return the shortest path from source to v as a vector of Edge,
     *         or empty vector if no path exists
     * @throws std::invalid_argument if vertex v is invalid
     */
    std::vector<Edge> pathTo(const int v) const {
        validateVertex(v);
        if (!hasPathTo(v)) return std::vector<Edge>{};

        std::stack<Edge> pathStack;
        int x = v;

        // Build path by following edgeTo[] backwards from v to source
        for (std::optional<Edge> e = edgeTo_[v]; e.has_value(); e = edgeTo_[x]) {
            pathStack.push(*e);
            x = e->other(x);  // Move to the other endpoint of the edge
        }

        // Convert stack to vector (this reverses the path to go from source to destination)
        std::vector<Edge> path;
        while (!pathStack.empty()) {
            path.push_back(pathStack.top());
            pathStack.pop();
        }

        return path;
    }
};