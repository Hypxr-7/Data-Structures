#pragma once

#include <vector>

#include "Digraph.hpp"

class DirectedDFS {
    std::vector<bool> _marked;
    int _count;

    void dfs(const Digraph& dg, const int v) {
        ++_count;
        _marked[v] = true;
        for (const int w : dg.adj(v))
            if (!_marked[w])
                dfs(dg, w);
    }

public:
    DirectedDFS(const Digraph& dg, const int source) : _marked(dg.V()), _count(0) {
        dfs(dg, source);
    }

    bool marked(const int v) const { return _marked[v]; }

    int count() const { return _count; }
};
