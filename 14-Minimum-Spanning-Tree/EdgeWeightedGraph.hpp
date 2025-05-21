#pragma once

#include <list>
#include <vector>

#include "Edge.hpp"

class EdgeWeightedGraph {
    int vertices;
    std::vector<std::list<Edge>> _adj;

public:
    explicit EdgeWeightedGraph(const int v) : vertices(v), _adj(v) {}

    void addEdge(const Edge& e) {
        const int v = e.either();
        const int w = e.other(v);
        _adj[v].push_back(e);
        _adj[w].push_back(e);
    }

    std::list<Edge> adj(const int v) const {return _adj[v]; }

    int V() const { return vertices; }

    std::vector<Edge> getEdges() const {
        std::vector<Edge> edges;
        for (int v = 0; v < vertices; ++v)
            for (const auto& w : _adj[v])
                edges.push_back(w);
        return std::move(edges);
    }
};