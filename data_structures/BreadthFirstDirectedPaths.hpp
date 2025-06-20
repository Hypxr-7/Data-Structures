#pragma once

#include <algorithm>
#include <limits>
#include <queue>
#include <stdexcept>
#include <vector>

#include "Digraph.hpp"

/**
 * Breadth-first search to find shortest directed paths from a source vertex
 * to all other vertices in a directed graph (digraph)
 */
class BreadthFirstDirectedPaths {
    std::vector<bool> marked;   // marked[v] = true if v is reachable from source
    std::vector<int> edgeTo;    // edgeTo[v] = previous vertex on shortest path from source to v
    std::vector<int> distTo_;   // distTo_[v] = shortest distance from source to v
    const int source;           // source vertex

    // BFS traversal starting from vertex s
    void bfs(const Digraph& g, const int s) {
        std::queue<int> q;
        marked[s] = true;
        distTo_[s] = 0;
        q.push(s);

        while (!q.empty()) {
            const int v = q.front();
            q.pop();

            // Explore all vertices adjacent to v via directed edges
            for (const int w : g.adj(v)) {
                if (!marked[w]) {
                    edgeTo[w] = v;
                    distTo_[w] = distTo_[v] + 1;
                    marked[w] = true;
                    q.push(w);
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

public:
    // Constructor: find shortest directed paths from source vertex s using BFS
    BreadthFirstDirectedPaths(const Digraph& g, const int s) :
        marked(g.V(), false),
        edgeTo(g.V()),
        distTo_(g.V(), std::numeric_limits<int>::max()),
        source(s) {
        validateVertex(s);
        bfs(g, s);
    }

    // Is there a directed path from source to vertex v?
    bool hasPathTo(const int v) const {
        validateVertex(v);
        return marked[v];
    }

    // Return shortest distance from source to vertex v
    int distTo(const int v) const {
        validateVertex(v);
        return distTo_[v];
    }

    // Return shortest directed path from source to vertex v (empty if no path exists)
    std::vector<int> pathTo(const int v) const {
        validateVertex(v);
        if (!hasPathTo(v)) return std::vector<int>{};

        std::vector<int> path;
        // Trace back from v to source, building path in reverse
        for (int x = v; x != source; x = edgeTo[x]) {
            path.push_back(x);
        }
        path.push_back(source);  // Add source vertex to complete the path

        // Reverse to get path from source to v
        std::reverse(path.begin(), path.end());
        return path;
    }
};
