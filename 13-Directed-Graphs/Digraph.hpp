#pragma once

#include <forward_list>
#include <vector>

class Digraph {
    const int vertices;
    int edges;
    std::vector<std::forward_list<int>> _adj;

public:
    explicit Digraph(const int v) : vertices(v), edges(0), _adj(v) {}

    int V() const { return vertices; }

    int E() const { return edges; }

    void addEdge(const int v, const int w) {
        _adj[v].push_front(w);
        ++edges;
    }

    std::forward_list<int> adj(const int v) const { return _adj[v]; }

    Digraph reverse() const {
        Digraph dg(vertices);
        for (int v = 0; v < vertices; ++v)
            for (const int w : _adj[v])
                dg.addEdge(w, v);
        return dg;
    }
};
