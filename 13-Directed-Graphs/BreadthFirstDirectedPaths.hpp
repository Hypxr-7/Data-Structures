#pragma once

#include <algorithm>
#include <limits>
#include <queue>
#include <vector>

#include "Digraph.hpp"

class BreadthFirstDirectedPaths {
    const int MAX;
    std::vector<bool> _marked;
    std::vector<int> _edgeTo;
    std::vector<int> _distTo;

    void bfs(const Digraph& dg, const int s) {
        std::queue<int> q;
        _marked[s] = true;
        _distTo[s] = 0;
        q.push(s);
        while (!q.empty()) {
            const int v = q.front(); q.pop();
            for (const int w : dg.adj(v)) {
                if (!_marked[w]) {
                    _edgeTo[w] = v;
                    _distTo[w] = _distTo[v] + 1;
                    _marked[w] = true;
                    q.push(w);
                }
            }
        }
    }

public:
    BreadthFirstDirectedPaths(const Digraph& dg, const int source) :
        MAX(std::numeric_limits<int>::max()),
        _marked(dg.V()),
        _edgeTo(dg.V()),
        _distTo(dg.V(), MAX) {
        bfs(dg, source);
    }

    bool hasPathTo(const int v) const { return _marked[v]; }

    int distTo(const int v) const { return _distTo[v]; }

    std::vector<int> pathTo(const int v) const {
        if (!hasPathTo(v)) return std::vector<int>{};

        std::vector<int> path;
        int x;
        for (x = v; _distTo[x] != 0; x = _edgeTo[x])
            path.push_back(x);
        path.push_back(x);

        std::reverse(path.begin(), path.end());
        return std::move(path);
    }
};