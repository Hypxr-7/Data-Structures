#pragma once

#include <algorithm>
#include <limits>
#include <queue>
#include <vector>
#include "Graph.hpp"

class BreadthFirstPaths {
private:
    std::vector<bool> marked;
    std::vector<int> edgeTo;
    std::vector<int> distTo;

    void bfs(const Graph& graph, const int source) {
        std::queue<int> q;
        distTo[source] = 0;
        marked[source] = true;
        q.push(source);

        while (!q.empty()) {
            const int v = q.front();
            q.pop();

            for (const int w : graph.adj(v)) {
                if (!marked[w]) {
                    edgeTo[w] = v;
                    distTo[w] = distTo[v] + 1;
                    marked[w] = true;
                    q.push(w);
                }
            }
        }
    }

    void bfs(const Graph& graph, const std::vector<int>& sources) {
        std::queue<int> q;

        for (const int source : sources) {
            distTo[source] = 0;
            marked[source] = true;
            q.push(source);
        }

        while (!q.empty()) {
            const int v = q.front();
            q.pop();

            for (const int w : graph.adj(v)) {
                if (!marked[w]) {
                    edgeTo[w] = v;
                    distTo[w] = distTo[v] + 1;
                    marked[w] = true;
                    q.push(w);
                }
            }
        }
    }

public:
    BreadthFirstPaths(const Graph& graph, const int source) :
        marked(graph.V(), false),
        edgeTo(graph.V()),
        distTo(graph.V(), std::numeric_limits<int>::max()) {
        bfs(graph, source);
    }

    BreadthFirstPaths(const Graph& graph, const std::vector<int>& sources) :
        marked(graph.V()),
        edgeTo(graph.V()),
        distTo(graph.V(), std::numeric_limits<int>::max()) {
        bfs(graph, sources);
    }

    bool hasPathTo(const int v) const {
        return marked[v];
    }

    int distanceTo(const int v) const {
        return distTo[v];
    }

    std::vector<int> pathTo(const int v) const {
        if (!hasPathTo(v)) return {};

        std::vector<int> path;
        int x;
        for (x = v; distTo[x] != 0; x = edgeTo[x])
            path.push_back(x);
        path.push_back(x);
        std::reverse(path.begin(), path.end());
        return std::move(path);
    }
};
