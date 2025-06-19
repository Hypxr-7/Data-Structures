#pragma once

#include <cassert>
#include <memory>
#include <queue>
#include <stack>
#include <stdexcept>
#include <vector>

#include "BreadthFirstPaths.hpp"
#include "Graph.hpp"

class EulerianCycle {
    std::vector<int> cycle_;    // Eulerian cycle; empty if no such cycle exists

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

    // Check necessary and sufficient conditions for Eulerian cycle
    bool satisfiesNecessaryAndSufficientConditions(const Graph& g) const {
        // Condition 0: at least 1 edge
        if (g.E() == 0) return false;

        // Condition 1: degree(v) is even for every vertex
        for (int v = 0; v < g.V(); ++v)
            if (g.degree(v) % 2 != 0)
                return false;

        // Condition 2: graph is connected (ignoring isolated vertices)
        const int s = nonIsolatedVertex(g);
        const BreadthFirstPaths bfs(g, s);
        for (int v = 0; v < g.V(); ++v)
            if (g.degree(v) > 0 && !bfs.hasPathTo(v))
                return false;

        return true;
    }

    // Verify that the solution is correct
    bool certifySolution(const Graph& g) const {
        // Internal consistency check
        if (hasEulerianCycle() == cycle_.empty()) return false;

        // hasEulerianCycle() returns correct value
        if (hasEulerianCycle() != satisfiesNecessaryAndSufficientConditions(g)) return false;

        // Nothing else to check if no Eulerian cycle
        if (cycle_.empty()) return true;

        // Check that cycle uses correct number of edges
        if (cycle_.size() != g.E() + 1) return false;

        // Check that first and last vertices in cycle are the same
        int first = -1;
        int last = -1;
        for (const int v : cycle()) {
            if (first == -1) first = v;
            last = v;
        }
        if (first != last) return false;

        return true;
    }

public:
    // Constructor: find Eulerian cycle in graph g
    explicit EulerianCycle(const Graph& g) {
        // Must have at least one edge
        if (g.E() == 0) return;

        // Necessary condition: all vertices have even degree
        for (int v = 0; v < g.V(); ++v) {
            if (g.degree(v) % 2 != 0) return;
        }

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

        // Initialize stack with any non-isolated vertex
        const int s = nonIsolatedVertex(g);
        if (s == -1) return;

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
            // Push vertex with no more leaving edges to cycle
            cycle_.push_back(v);
        }

        // Check if all edges are used
        if (cycle_.size() != g.E() + 1) {
            cycle_.clear();
        }

        assert(certifySolution(g));
    }

    // Returns the Eulerian cycle as a vector (empty if no cycle exists)
    std::vector<int> cycle() const { return cycle_; }

    // Returns true if the graph has an Eulerian cycle
    bool hasEulerianCycle() const { return !cycle_.empty(); }
};