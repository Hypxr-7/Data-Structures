#pragma once

#include <algorithm>
#include <vector>

#include "Graph.hpp"

class DepthFirstPaths {
private:
    std::vector<bool> marked;
    std::vector<int> edgeTo;
    const int source;

    void dfs(const Graph& graph, const int v) {
        marked[v] = true;
        for (const int w : graph.adj(v))
            if (!marked[w]) {
                edgeTo[w] = v;
                dfs(graph, w);
            }
    }

public:
    DepthFirstPaths(const Graph& graph, const int source) : marked(graph.V()),
                                                             edgeTo(graph.V()),
                                                             source(source) {
        dfs(graph, source);
    }

    bool hasPathTo(const int v) const {
        return marked[v];
    }

    std::vector<int> pathTo(const int v) const {
        if (!hasPathTo(v)) return std::vector<int>{};
        std::vector<int> path;
        for (int x = v; x != source; x = edgeTo[x])
            path.push_back(x);
        path.push_back(source);
        std::reverse(path.begin(), path.end());
        return std::move(path);
    }
};
