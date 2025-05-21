#pragma once

#include <vector>

#include "DepthFirstOrder.hpp"
#include "Digraph.hpp"

class SCC {
    std::vector<bool> _marked;
    std::vector<int> _id;
    int _count;

    void dfs(const Digraph& G, int v) {
        _marked[v] = true;
        _id[v] = _count;
        for (int w : G.adj(v))
            if (!_marked[w])
                dfs(G, w);
    }

public:
    explicit SCC(const Digraph& G) : _marked(G.V()), _id(G.V()), _count(0) {
        DepthFirstOrder d(G.reverse());
        for (const int v : d.reversePost()) {
            if (!_marked[v]) {
                dfs(G, v);
                _count++;
            }
        }
    }

    bool stronglyConnected(const int v, const int w) const { return _id[v] == _id[w]; }

    int count() const { return _count; }

    int id(const int v) const { return _id[v]; }
};
