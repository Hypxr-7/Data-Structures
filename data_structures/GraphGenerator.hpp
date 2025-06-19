#pragma once

#include <algorithm>
#include <chrono>
#include <queue>
#include <random>
#include <set>
#include <stdexcept>
#include <vector>

#include "Graph.hpp"

/**
 * The GraphGenerator class provides static methods for creating
 * various graphs, including Erdos-Renyi random graphs, random bipartite
 * graphs, random k-regular graphs, and random rooted trees.
 */
class GraphGenerator {
    struct Edge {
        int v, w;

        Edge(const int v, const int w) {
            if (v < w) {
                this->v = v;
                this->w = w;
            } else {
                this->v = w;
                this->w = v;
            }
        }

        bool operator<(const Edge& other) const {
            if (v != other.v) return v < other.v;
            return w < other.w;
        }

        bool operator==(const Edge& other) const {
            return v == other.v && w == other.w;
        }
    };

    static std::mt19937 rng;

    // Utility methods
    static int uniformInt(const int n) {
        std::uniform_int_distribution<int> dist(0, n - 1);
        return dist(rng);

    }

    static bool bernoulli(const double p) {
        std::bernoulli_distribution dist(p);
        return dist(rng);
    }

    static void shuffle(std::vector<int>& arr) {
        std::shuffle(arr.begin(), arr.end(), rng);
    }

    // This class cannot be instantiated

public:
    GraphGenerator() = delete;
    /**
     * Returns a random simple graph containing V vertices and E edges.
     */
    static Graph simple(const int V, const int E) {
        if (E > static_cast<long long>(V) * (V - 1) / 2) {
            throw std::invalid_argument("Too many edges");
        }
        if (E < 0) {
            throw std::invalid_argument("Too few edges");
        }

        Graph G(V);
        std::set<Edge> edgeSet;

        while (G.E() < E) {
            const int v = uniformInt(V);
            const int w = uniformInt(V);

            if (Edge e(v, w); v != w && edgeSet.find(e) == edgeSet.end()) {
                edgeSet.insert(e);
                G.addEdge(v, w);
            }
        }
        return G;
    }

    /**
     * Returns a random simple graph on V vertices, with an
     * edge between any two vertices with probability p (Erdos-Renyi model).
     */
    static Graph simple(const int V, const double p) {
        if (p < 0.0 || p > 1.0) {
            throw std::invalid_argument("Probability must be between 0 and 1");
        }

        Graph G(V);
        for (int v = 0; v < V; v++) {
            for (int w = v + 1; w < V; w++) {
                if (bernoulli(p)) {
                    G.addEdge(v, w);
                }
            }
        }
        return G;
    }

    /**
     * Returns the complete graph on V vertices.
     */
    static Graph complete(const int V) {
        return simple(V, 1.0);
    }

    /**
     * Returns a complete bipartite graph on V1 and V2 vertices.
     */
    static Graph completeBipartite(const int V1, const int V2) {
        return bipartite(V1, V2, V1 * V2);
    }

    /**
     * Returns a random simple bipartite graph on V1 and V2 vertices with E edges.
     */
    static Graph bipartite(const int V1, const int V2, const int E) {
        if (E > static_cast<long long>(V1) * V2) {
            throw std::invalid_argument("Too many edges");
        }
        if (E < 0) {
            throw std::invalid_argument("Too few edges");
        }

        Graph G(V1 + V2);
        std::vector<int> vertices(V1 + V2);
        for (int i = 0; i < V1 + V2; i++) {
            vertices[i] = i;
        }
        shuffle(vertices);

        std::set<Edge> edgeSet;
        while (G.E() < E) {
            const int i = uniformInt(V1);
            const int j = V1 + uniformInt(V2);

            if (Edge e(vertices[i], vertices[j]); edgeSet.find(e) == edgeSet.end()) {
                edgeSet.insert(e);
                G.addEdge(vertices[i], vertices[j]);
            }
        }
        return G;
    }

    /**
     * Returns a random simple bipartite graph on V1 and V2 vertices,
     * containing each possible edge with probability p.
     */
    static Graph bipartite(const int V1, const int V2, const double p) {
        if (p < 0.0 || p > 1.0) {
            throw std::invalid_argument("Probability must be between 0 and 1");
        }

        std::vector<int> vertices(V1 + V2);
        for (int i = 0; i < V1 + V2; i++) {
            vertices[i] = i;
        }
        shuffle(vertices);

        Graph G(V1 + V2);
        for (int i = 0; i < V1; i++) {
            for (int j = 0; j < V2; j++) {
                if (bernoulli(p)) {
                    G.addEdge(vertices[i], vertices[V1 + j]);
                }
            }
        }
        return G;
    }

    /**
     * Returns a path graph on V vertices.
     */
    static Graph path(const int V) {
        Graph G(V);
        std::vector<int> vertices(V);
        for (int i = 0; i < V; i++) {
            vertices[i] = i;
        }
        shuffle(vertices);

        for (int i = 0; i < V - 1; i++) {
            G.addEdge(vertices[i], vertices[i + 1]);
        }
        return G;
    }

    /**
     * Returns a complete binary tree graph on V vertices.
     */
    static Graph binaryTree(const int V) {
        Graph G(V);
        std::vector<int> vertices(V);
        for (int i = 0; i < V; i++) {
            vertices[i] = i;
        }
        shuffle(vertices);

        for (int i = 1; i < V; i++) {
            G.addEdge(vertices[i], vertices[(i - 1) / 2]);
        }
        return G;
    }

    /**
     * Returns a cycle graph on V vertices.
     */
    static Graph cycle(const int V) {
        Graph G(V);
        std::vector<int> vertices(V);
        for (int i = 0; i < V; i++) {
            vertices[i] = i;
        }
        shuffle(vertices);

        for (int i = 0; i < V - 1; i++) {
            G.addEdge(vertices[i], vertices[i + 1]);
        }
        G.addEdge(vertices[V - 1], vertices[0]);
        return G;
    }

    /**
     * Returns an Eulerian cycle graph on V vertices.
     */
    static Graph eulerianCycle(const int V, const int E) {
        if (E <= 0) {
            throw std::invalid_argument("An Eulerian cycle must have at least one edge");
        }
        if (V <= 0) {
            throw std::invalid_argument("An Eulerian cycle must have at least one vertex");
        }

        Graph G(V);
        std::vector<int> vertices(E);
        for (int i = 0; i < E; i++) {
            vertices[i] = uniformInt(V);
        }

        for (int i = 0; i < E - 1; i++) {
            G.addEdge(vertices[i], vertices[i + 1]);
        }
        G.addEdge(vertices[E - 1], vertices[0]);
        return G;
    }

    /**
     * Returns an Eulerian path graph on V vertices.
     */
    static Graph eulerianPath(const int V, const int E) {
        if (E < 0) {
            throw std::invalid_argument("negative number of edges");
        }
        if (V <= 0) {
            throw std::invalid_argument("An Eulerian path must have at least one vertex");
        }

        Graph G(V);
        std::vector<int> vertices(E + 1);
        for (int i = 0; i < E + 1; i++) {
            vertices[i] = uniformInt(V);
        }

        for (int i = 0; i < E; i++) {
            G.addEdge(vertices[i], vertices[i + 1]);
        }
        return G;
    }

    /**
     * Returns a wheel graph on V vertices.
     */
    static Graph wheel(const int V) {
        if (V <= 1) {
            throw std::invalid_argument("Number of vertices must be at least 2");
        }

        Graph G(V);
        std::vector<int> vertices(V);
        for (int i = 0; i < V; i++) {
            vertices[i] = i;
        }
        shuffle(vertices);

        // Simple cycle on V-1 vertices
        for (int i = 1; i < V - 1; i++) {
            G.addEdge(vertices[i], vertices[i + 1]);
        }
        G.addEdge(vertices[V - 1], vertices[1]);

        // Connect vertices[0] to every vertex on cycle
        for (int i = 1; i < V; i++) {
            G.addEdge(vertices[0], vertices[i]);
        }

        return G;
    }

    /**
     * Returns a star graph on V vertices.
     */
    static Graph star(const int V) {
        if (V <= 0) {
            throw std::invalid_argument("Number of vertices must be at least 1");
        }

        Graph G(V);
        std::vector<int> vertices(V);
        for (int i = 0; i < V; i++) {
            vertices[i] = i;
        }
        shuffle(vertices);

        // Connect vertices[0] to every other vertex
        for (int i = 1; i < V; i++) {
            G.addEdge(vertices[0], vertices[i]);
        }

        return G;
    }

    /**
     * Returns a uniformly random k-regular graph on V vertices.
     */
    static Graph regular(const int V, const int k) {
        if (V * k % 2 != 0) {
            throw std::invalid_argument("Number of vertices * k must be even");
        }

        Graph G(V);

        // Create k copies of each vertex
        std::vector<int> vertices(V * k);
        for (int v = 0; v < V; v++) {
            for (int j = 0; j < k; j++) {
                vertices[v + V * j] = v;
            }
        }

        // Pick a random perfect matching
        shuffle(vertices);
        for (int i = 0; i < V * k / 2; i++) {
            G.addEdge(vertices[2 * i], vertices[2 * i + 1]);
        }
        return G;
    }

    /**
     * Returns a uniformly random tree on V vertices.
     */
    static Graph tree(const int V) {
        Graph G(V);

        // Special case
        if (V == 1) return G;

        // Generate Prufer sequence
        std::vector<int> prufer(V - 2);
        for (int i = 0; i < V - 2; i++) {
            prufer[i] = uniformInt(V);
        }

        // Calculate degrees
        std::vector<int> degree(V, 1);
        for (int i = 0; i < V - 2; i++) {
            degree[prufer[i]]++;
        }

        // Priority queue contains all vertices of degree 1
        std::priority_queue<int, std::vector<int>, std::greater<>> pq;
        for (int v = 0; v < V; v++) {
            if (degree[v] == 1) {
                pq.push(v);
            }
        }

        // Repeatedly remove minimum degree 1 vertex
        for (int i = 0; i < V - 2; i++) {
            const int v = pq.top();
            pq.pop();
            G.addEdge(v, prufer[i]);
            degree[v]--;
            degree[prufer[i]]--;
            if (degree[prufer[i]] == 1) {
                pq.push(prufer[i]);
            }
        }

        const int u = pq.top(); pq.pop();
        const int v = pq.top(); pq.pop();
        G.addEdge(u, v);

        return G;
    }
};

// Initialize static random number generator
std::mt19937 GraphGenerator::rng(std::random_device{}());
