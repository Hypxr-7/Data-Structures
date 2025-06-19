#pragma once

#include <algorithm>
#include <stdexcept>
#include <vector>

#include "Graph.hpp"

class DepthFirstPaths {
    std::vector<bool> marked;  // marked[v] = true if v is reachable from source
    std::vector<int> edgeTo;   // edgeTo[v] = previous vertex on path from source to v
    const int s;               // source vertex

    // Recursive DFS from vertex v
    void dfs(const Graph& g, const int v) {
        marked[v] = true;
        for (const int w : g.adj(v)) {
            if (!marked[w]) {
                edgeTo[w] = v;
                dfs(g, w);
            }
        }
    }

    // Validate vertex index is within bounds
    void validateVertex(const int v) const {
        if (v < 0 || v >= static_cast<int>(marked.size())) {
            throw std::invalid_argument("Vertex out of range");
        }
    }

public:
    // Constructor: find all paths from source vertex s
    DepthFirstPaths(const Graph& g, const int s) : marked(g.V(), false), edgeTo(g.V()), s(s) {
        validateVertex(s);
        dfs(g, s);
    }

    // Is there a path from source to vertex v?
    bool hasPathTo(const int v) const {
        validateVertex(v);
        return marked[v];
    }

    // Return path from source to vertex v (empty if no path exists)
    std::vector<int> pathTo(const int v) const {
        validateVertex(v);
        if (!hasPathTo(v)) return std::vector<int>{};

        std::vector<int> path;
        // Trace back from v to source, building path in reverse
        for (int x = v; x != s; x = edgeTo[x]) {
            path.push_back(x);
        }
        path.push_back(s);  // Add source vertex to complete the path

        // Reverse to get path from source to v
        std::reverse(path.begin(), path.end());
        return path;
    }
};