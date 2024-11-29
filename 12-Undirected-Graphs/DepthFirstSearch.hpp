#pragma once

#include <vector>
#include "Graph.hpp"

class DepthFirstSearch {
private:
    std::vector<bool> marked;
    int count;

    void dfs(const Graph& graph, const int v) {
        marked[v] = true;
        ++count;
        for (const int w : graph.adj(v))
            if (!marked[w])
                dfs(graph, w);
    }

public:
    explicit DepthFirstSearch(const Graph& graph, const int source) : marked(graph.V()), count(0) {
        dfs(graph, source);
    }

    bool isMarked(const int w) const { return marked[w]; }

    int getCount() const { return count; }
};
