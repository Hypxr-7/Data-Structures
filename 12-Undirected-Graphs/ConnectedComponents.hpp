#pragma once

#include <vector>

#include "Graph.hpp"

class ConnectedComponents {
    std::vector<bool> marked; // marked[v] = has vertex v been marked?
    std::vector<int> id; // id[v] = id of connected component containing v
    std::vector<int> size; // size[id] = number of vertices in given component
    int count; // number of connected components

    void dfs(const Graph& graph, const int v) {
        marked[v] = true;
        id[v] = count;
        size[count]++;
        for (const int w : graph.adj(v)) {
            if (!marked[w]) {
                dfs(graph, w);
            }
        }
    }

    void validateVertex(const int v) const {
        if (v < 0 || v >= marked.size()) {
            throw std::invalid_argument(
                "vertex " + std::to_string(v) + " is not between 0 and " + std::to_string(
                    marked.size() - 1));
        }
    }

public:
    explicit ConnectedComponents(const Graph& graph) : marked(graph.V(), false), id(graph.V()),
                                                       size(graph.V(), 0), count(0) {
        for (int v = 0; v < graph.V(); v++) {
            if (!marked[v]) {
                dfs(graph, v);
                count++;
            }
        }
    }

    int getID(const int v) const {
        validateVertex(v);
        return id[v];
    }

    int getSize(const int v) const {
        validateVertex(v);
        return size[id[v]];
    }

    int getCount() const {
        return count;
    }

    bool connected(const int v, const int w) const {
        validateVertex(v);
        validateVertex(w);
        return getID(v) == getID(w);
    }
};
