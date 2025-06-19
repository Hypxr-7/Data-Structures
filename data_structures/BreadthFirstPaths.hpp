#pragma once

#include <algorithm>
#include <cassert>
#include <limits>
#include <queue>
#include <stdexcept>
#include <vector>

#include "Graph.hpp"

class BreadthFirstPaths {
    std::vector<bool> marked;   // marked[v] = true if v is reachable from source
    std::vector<int> edgeTo;    // edgeTo[v] = previous vertex on shortest path from source to v
    std::vector<int> distTo_;   // distTo_[v] = shortest distance from source to v
    const int s;                // source vertex

    // BFS traversal starting from vertex v
    void bfs(const Graph& g, const int v) {
        std::queue<int> queue;
        distTo_[v] = 0;
        marked[v] = true;
        queue.push(v);

        while (!queue.empty()) {
            const int w = queue.front();
            queue.pop();
            for (const auto x : g.adj(w)) {
                if (!marked[x]) {
                    edgeTo[x] = w;
                    distTo_[x] = distTo_[w] + 1;
                    marked[x] = true;
                    queue.push(x);
                }
            }
        }
    }

    // Validate vertex index is within bounds
    void validateVertex(const int v) const {
        if (v < 0 || v >= static_cast<int>(marked.size())) {
            throw std::invalid_argument("Vertex out of range");
        }
    }

    // Check invariants for debugging (fixed bugs in original)
    bool check(const Graph& g, const int s) const {
        // Source should have distance 0
        if (distTo(s) != 0) return false;

        // Check BFS tree property: adjacent vertices differ by at most 1 in distance
        for (int w = 0; w < g.V(); ++w) {
            for (const int x : g.adj(w)) {
                if (hasPathTo(w) != hasPathTo(x)) return false;  // Fixed: compare reachability, not paths
                if (hasPathTo(w) && (distTo(x) > distTo(w) + 1)) return false;  // Fixed: added hasPathTo check
            }
        }

        // Check that edgeTo gives correct distances
        for (int w = 0; w < g.V(); ++w) {
            if (!hasPathTo(w) || w == s) continue;
            if (const int x = edgeTo[w]; distTo(w) != distTo(x) + 1) return false;
        }

        return true;
    }

public:
    // Constructor: find the shortest paths from source vertex s using BFS
    BreadthFirstPaths(const Graph& g, const int s) :
        marked(g.V(), false),
        edgeTo(g.V()),
        distTo_(g.V(), std::numeric_limits<int>::max()),
        s(s) {
        validateVertex(s);
        bfs(g, s);
        // assert(check(g, s));
    }

    // Is there a path from source to vertex v?
    bool hasPathTo(const int v) const {
        validateVertex(v);
        return marked[v];
    }

    // Return shortest distance from source to vertex v
    int distTo(const int v) const {
        validateVertex(v);
        return distTo_[v];
    }

    // Return shortest path from source to vertex v (empty if no path exists)
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