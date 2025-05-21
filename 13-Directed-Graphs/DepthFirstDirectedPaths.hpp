#pragma once

#include <algorithm>
#include <vector>

#include "Digraph.hpp"

class DepthFirstDirectedPaths {
    std::vector<bool> _marked;
    std::vector<int> _edgeTo;
    const int _source;

    void dfs(const Digraph& dg, const int v) {
        _marked[v] = true;
        for (const int w : dg.adj(v)) {
            if (!_marked[w]) {
                _edgeTo[w] = v;
                dfs(dg, w);
            }
        }
    }

public:
    DepthFirstDirectedPaths(const Digraph& dg, const int source) :
        _marked(dg.V()),
        _edgeTo(dg.V()),
        _source(source) {
        dfs(dg, source);
    }

    bool hasPathTo(const int v) const { return _marked[v]; }

    std::vector<int> pathTo(const int v) const {
        if (!hasPathTo(v)) return std::vector<int>{};

        std::vector<int> path;
        for (int x = v; x != _source; x = _edgeTo[x])
            path.push_back(x);
        path.push_back(_source);
        std::reverse(path.begin(), path.end());
        return std::move(path);
    }
};
