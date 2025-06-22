#pragma once

#include <list>
#include <random>
#include <stdexcept>
#include <vector>

#include "Edge.hpp"

/**
 * Represents an edge-weighted undirected graph.
 *
 * This data structure stores a graph where:
 * - Vertices are represented by integers from 0 to V-1
 * - Edges have associated weights (typically double values)
 * - The graph is undirected (edge (v,w) is same as edge (w,v))
 *
 * Common applications:
 * - Minimum Spanning Tree algorithms (Kruskal's, Prim's)
 * - Shortest path algorithms in weighted graphs
 * - Network flow problems
 * - Transportation and logistics optimization
 *
 * Implementation uses adjacency lists where each vertex maintains
 * a list of all edges incident to it.
 */
class EdgeWeightedGraph {
    std::vector<std::list<Edge>> adj_;  // adjacency lists for each vertex
    int V_;                             // number of vertices
    int E_;                             // number of edges

    // Validate that vertex v is within valid range [0, V_)
    void validateVertex(const int v) const {
        if (v < 0 || v >= V_) {
            throw std::invalid_argument("Vertex out of range");
        }
    }

    // Validate vertex count during construction
    int validateConstructorInit(const int v) const {
        if (v < 0) {
            throw std::invalid_argument("Number of vertices cannot be negative");
        }
        return v;
    }

public:
    // Constructor: create empty graph with V vertices and no edges
    explicit EdgeWeightedGraph(const int v) :
        adj_(validateConstructorInit(v)),
        V_(v),
        E_(0) {}

    // Constructor: create random graph with V vertices and E edges
    // Edges have random weights in [0,1) and connect random vertex pairs
    EdgeWeightedGraph(const int v, const int e) :
        adj_(validateConstructorInit(v)),
        V_(v),
        E_(0) {

        if (e < 0) {
            throw std::invalid_argument("Number of edges cannot be negative");
        }

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<> vertexDist(0, v - 1);
        std::uniform_real_distribution<> weightDist(0.0, 1.0);

        for (int i = 0; i < e; ++i) {
            const int u = vertexDist(rng);
            const int w = vertexDist(rng);
            const double weight = weightDist(rng);
            Edge edge{u, w, weight};
            addEdge(edge);
        }
    }

    // Get the number of vertices in the graph
    int V() const {
        return V_;
    }

    // Get the number of edges in the graph
    int E() const {
        return E_;
    }

    // Add an edge to the graph
    // Since graph is undirected, edge is added to adjacency lists of both endpoints
    void addEdge(const Edge& e) {
        const int v = e.either();
        const int w = e.other(v);
        validateVertex(v);
        validateVertex(w);

        // Add edge to adjacency list of both vertices (undirected graph)
        adj_[v].push_front(e);
        if (v != w) {
            adj_[w].push_front(e);
        }
        ++E_;
    }

    // Get all edges incident to vertex v
    // Returns a vector copy for safe iteration (caller can modify without affecting graph)
    std::vector<Edge> adj(const int v) const {
        validateVertex(v);
        return std::vector<Edge>{adj_[v].begin(), adj_[v].end()};
    }

    // Get the degree of vertex v (number of incident edges)
    int degree(const int v) const {
        validateVertex(v);
        return static_cast<int>(adj_[v].size());
    }

    // Get all edges in the graph exactly once
    // Handles the fact that each edge appears in two adjacency lists
    std::vector<Edge> edges() const {
        std::vector<Edge> allEdges;
        allEdges.reserve(E_);

        for (int v = 0; v < V_; ++v) {
            int selfLoops = 0;
            for (const Edge& e : adj_[v]) {
                const int w = e.other(v);

                // Only include each edge once by using vertex ordering
                if (w > v) {
                    // Normal edge: include if other vertex has higher index
                    allEdges.push_back(e);
                }
                else if (w == v) {
                    // Self-loop: include every other occurrence to avoid duplicates
                    if (selfLoops % 2 == 0) {
                        allEdges.push_back(e);
                    }
                    ++selfLoops;
                }
                // If w < v, skip (edge already included when processing vertex w)
            }
        }
        return allEdges;
    }
};
