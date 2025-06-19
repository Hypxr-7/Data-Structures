#pragma once

#include <cassert>
#include <memory>
#include <queue>
#include <stack>
#include <stdexcept>
#include <vector>

#include "BreadthFirstPaths.hpp"
#include "Graph.hpp"

class EulerianPath {
    std::vector<int> path_;    // Eulerian path; empty if no such path exists

    // Edge class to track usage and avoid using both copies of an undirected edge
    struct Edge {
        const int v;
        const int w;
        bool isUsed;

        Edge(const int v, const int w) : v(v), w(w), isUsed(false) {}

        // Returns the other vertex of the edge
        int other(const int vertex) const {
            if (vertex == v) return w;
            if (vertex == w) return v;
            throw std::invalid_argument("Vertex not in Edge");
        }
    };

    // Find any vertex with degree > 0; return -1 if no such vertex
    int nonIsolatedVertex(const Graph& g) const {
        for (int v = 0; v < g.V(); ++v)
            if (g.degree(v) > 0)
                return v;
        return -1;
    }

    // Check necessary and sufficient conditions for Eulerian path
    bool satisfiesNecessaryAndSufficientConditions(const Graph& g) const {
        // Special case: empty graph has degenerate Eulerian path
        if (g.E() == 0) return true;

        // Condition 1: degree(v) is even except for possibly two vertices
        int oddDegreeVertices = 0;
        for (int v = 0; v < g.V(); ++v)
            if (g.degree(v) % 2 != 0)
                oddDegreeVertices++;
        if (oddDegreeVertices > 2) return false;

        // Condition 2: graph is connected (ignoring isolated vertices)
        if (const int s = nonIsolatedVertex(g); s != -1) {
            const BreadthFirstPaths bfs(g, s);
            for (int v = 0; v < g.V(); ++v)
                if (g.degree(v) > 0 && !bfs.hasPathTo(v))
                    return false;
        }

        return true;
    }

    // Verify that the solution is correct
    bool certifySolution(const Graph& g) const {
        // Internal consistency check
        if (hasEulerianPath() == path_.empty()) return false;

        // hasEulerianPath() returns correct value
        if (hasEulerianPath() != satisfiesNecessaryAndSufficientConditions(g)) return false;

        // Nothing else to check if no Eulerian path
        if (path_.empty()) return true;

        // Check that path uses correct number of edges
        if (path_.size() != g.E() + 1) return false;

        return true;
    }

public:
    // Constructor: find Eulerian path in graph g
    explicit EulerianPath(const Graph& g) {
        // Find vertex from which to start potential Eulerian path:
        // a vertex v with odd degree(v) if it exists;
        // otherwise a vertex with degree(v) > 0
        int oddDegreeVertices = 0;
        int s = nonIsolatedVertex(g);
        for (int v = 0; v < g.V(); ++v) {
            if (g.degree(v) % 2 != 0) {
                oddDegreeVertices++;
                s = v;
            }
        }

        // Graph can't have an Eulerian path
        // (this condition is needed for correctness)
        if (oddDegreeVertices > 2) return;

        // Special case for graph with zero edges (has a degenerate Eulerian path)
        if (s == -1) s = 0;

        // Store pointers to edges instead of edge objects
        std::vector<std::queue<Edge*>> adj(g.V());

        // Create a vector to own all Edge objects
        std::vector<std::unique_ptr<Edge>> edges;

        for (int v = 0; v < g.V(); ++v) {
            int selfLoops = 0;
            for (const int w : g.adj(v)) {
                if (v == w) {
                    if (selfLoops % 2 == 0) {
                        auto edge = std::make_unique<Edge>(v, w);
                        Edge* edgePtr = edge.get();
                        edges.push_back(std::move(edge));
                        adj[v].push(edgePtr);
                        adj[w].push(edgePtr);
                    }
                    ++selfLoops;
                }
                else if (v < w) {
                    auto edge = std::make_unique<Edge>(v, w);
                    Edge* edgePtr = edge.get();
                    edges.push_back(std::move(edge));
                    adj[v].push(edgePtr);
                    adj[w].push(edgePtr);
                }
            }
        }

        // Initialize stack with starting vertex
        std::stack<int> stack;
        stack.push(s);

        // Greedily search through edges in iterative DFS style
        while (!stack.empty()) {
            int v = stack.top();
            stack.pop();
            while (!adj[v].empty()) {
                Edge* edge = adj[v].front();
                adj[v].pop();
                if (edge->isUsed) continue;
                edge->isUsed = true;
                stack.push(v);
                v = edge->other(v);
            }
            // Push vertex with no more leaving edges to path
            path_.push_back(v);
        }

        // Check if all edges are used
        if (path_.size() != g.E() + 1) {
            path_.clear();
        }

        assert(certifySolution(g));
    }

    // Returns the Eulerian path as a vector (empty if no path exists)
    std::vector<int> path() const { return path_; }

    // Returns true if the graph has an Eulerian path
    bool hasEulerianPath() const { return !path_.empty(); }
};
