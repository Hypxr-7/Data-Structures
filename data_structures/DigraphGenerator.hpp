#pragma once

#include <algorithm>
#include <chrono>
#include <random>
#include <set>
#include <stdexcept>
#include <vector>

#include "Digraph.hpp"

/**
 * The DigraphGenerator class provides static methods for creating
 * various digraphs, including Erdos-Renyi random digraphs, random DAGs,
 * random rooted trees, random tournaments, and other specialized digraphs.
 */
class DigraphGenerator {
    // Edge class for tracking directed edges
    struct Edge {
        int v, w;

        Edge(const int v, const int w) : v(v), w(w) {}

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

    static int uniformInt(const int min, const int max) {
        std::uniform_int_distribution<int> dist(min, max - 1);
        return dist(rng);
    }

    static bool bernoulli(const double p) {
        std::bernoulli_distribution dist(p);
        return dist(rng);
    }

    static void shuffle(std::vector<int>& arr) {
        std::shuffle(arr.begin(), arr.end(), rng);
    }


public:
    // This class cannot be instantiated
    DigraphGenerator() = delete;

    /**
     * Returns a random simple digraph containing V vertices and E edges.
     */
    static Digraph simple(const int V, const int E) {
        if (E > static_cast<long long>(V) * (V - 1)) {
            throw std::invalid_argument("Too many edges");
        }
        if (E < 0) {
            throw std::invalid_argument("Too few edges");
        }

        Digraph G(V);
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
     * Returns a random simple digraph on V vertices, with an edge between
     * any two vertices with probability p (Erdos-Renyi model).
     */
    static Digraph simple(const int V, const double p) {
        if (p < 0.0 || p > 1.0) {
            throw std::invalid_argument("Probability must be between 0 and 1");
        }

        Digraph G(V);
        for (int v = 0; v < V; v++) {
            for (int w = 0; w < V; w++) {
                if (v != w && bernoulli(p)) {
                    G.addEdge(v, w);
                }
            }
        }
        return G;
    }

    /**
     * Returns the complete digraph on V vertices.
     */
    static Digraph complete(const int V) {
        Digraph G(V);
        for (int v = 0; v < V; v++) {
            for (int w = 0; w < V; w++) {
                if (v != w) {
                    G.addEdge(v, w);
                }
            }
        }
        return G;
    }

    /**
     * Returns a random simple DAG containing V vertices and E edges.
     */
    static Digraph dag(const int V, const int E) {
        if (E > static_cast<long long>(V) * (V - 1) / 2) {
            throw std::invalid_argument("Too many edges");
        }
        if (E < 0) {
            throw std::invalid_argument("Too few edges");
        }

        Digraph G(V);
        std::set<Edge> edgeSet;
        std::vector<int> vertices(V);
        for (int i = 0; i < V; i++) {
            vertices[i] = i;
        }
        shuffle(vertices);

        while (G.E() < E) {
            const int v = uniformInt(V);
            const int w = uniformInt(V);

            if (Edge e(v, w); v < w && edgeSet.find(e) == edgeSet.end()) {
                edgeSet.insert(e);
                G.addEdge(vertices[v], vertices[w]);
            }
        }
        return G;
    }

    /**
     * Returns a random tournament digraph on V vertices.
     */
    static Digraph tournament(const int V) {
        Digraph G(V);
        for (int v = 0; v < V; v++) {
            for (int w = v + 1; w < V; w++) {
                if (bernoulli(0.5)) {
                    G.addEdge(v, w);
                } else {
                    G.addEdge(w, v);
                }
            }
        }
        return G;
    }

    /**
     * Returns a complete rooted-in DAG on V vertices.
     */
    static Digraph completeRootedInDAG(const int V) {
        Digraph G(V);
        std::vector<int> vertices(V);
        for (int i = 0; i < V; i++) {
            vertices[i] = i;
        }
        shuffle(vertices);

        for (int i = 0; i < V; i++) {
            for (int j = i + 1; j < V; j++) {
                G.addEdge(vertices[i], vertices[j]);
            }
        }
        return G;
    }

    /**
     * Returns a random rooted-in DAG on V vertices and E edges.
     */
    static Digraph rootedInDAG(const int V, const int E) {
        if (E > static_cast<long long>(V) * (V - 1) / 2) {
            throw std::invalid_argument("Too many edges");
        }
        if (E < V - 1) {
            throw std::invalid_argument("Too few edges");
        }

        Digraph G(V);
        std::set<Edge> edgeSet;

        // Fix a topological order
        std::vector<int> vertices(V);
        for (int i = 0; i < V; i++) {
            vertices[i] = i;
        }
        shuffle(vertices);

        // One edge pointing from each vertex, other than the root
        for (int v = 0; v < V - 1; v++) {
            const int w = uniformInt(v + 1, V);
            Edge e(v, w);
            edgeSet.insert(e);
            G.addEdge(vertices[v], vertices[w]);
        }

        while (G.E() < E) {
            const int v = uniformInt(V);
            const int w = uniformInt(V);

            if (Edge e(v, w); v < w && edgeSet.find(e) == edgeSet.end()) {
                edgeSet.insert(e);
                G.addEdge(vertices[v], vertices[w]);
            }
        }
        return G;
    }

    /**
     * Returns a complete rooted-out DAG on V vertices.
     */
    static Digraph completeRootedOutDAG(const int V) {
        Digraph G(V);
        std::vector<int> vertices(V);
        for (int i = 0; i < V; i++) {
            vertices[i] = i;
        }
        shuffle(vertices);

        for (int i = 0; i < V; i++) {
            for (int j = i + 1; j < V; j++) {
                G.addEdge(vertices[j], vertices[i]);
            }
        }
        return G;
    }

    /**
     * Returns a random rooted-out DAG on V vertices and E edges.
     */
    static Digraph rootedOutDAG(const int V, const int E) {
        if (E > static_cast<long long>(V) * (V - 1) / 2) {
            throw std::invalid_argument("Too many edges");
        }
        if (E < V - 1) {
            throw std::invalid_argument("Too few edges");
        }

        Digraph G(V);
        std::set<Edge> edgeSet;

        // Fix a topological order
        std::vector<int> vertices(V);
        for (int i = 0; i < V; i++) {
            vertices[i] = i;
        }
        shuffle(vertices);

        // One edge pointing to each vertex, other than the root
        for (int v = 0; v < V - 1; v++) {
            const int w = uniformInt(v + 1, V);
            Edge e(w, v);
            edgeSet.insert(e);
            G.addEdge(vertices[w], vertices[v]);
        }

        while (G.E() < E) {
            const int v = uniformInt(V);
            const int w = uniformInt(V);

            if (Edge e(w, v); v < w && edgeSet.find(e) == edgeSet.end()) {
                edgeSet.insert(e);
                G.addEdge(vertices[w], vertices[v]);
            }
        }
        return G;
    }

    /**
     * Returns a random rooted-in tree on V vertices.
     */
    static Digraph rootedInTree(const int V) {
        return rootedInDAG(V, V - 1);
    }

    /**
     * Returns a random rooted-out tree on V vertices.
     */
    static Digraph rootedOutTree(const int V) {
        return rootedOutDAG(V, V - 1);
    }

    /**
     * Returns a path digraph on V vertices.
     */
    static Digraph path(const int V) {
        Digraph G(V);
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
     * Returns a complete binary tree digraph on V vertices.
     */
    static Digraph binaryTree(const int V) {
        Digraph G(V);
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
     * Returns a cycle digraph on V vertices.
     */
    static Digraph cycle(const int V) {
        Digraph G(V);
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
     * Returns an Eulerian cycle digraph on V vertices.
     */
    static Digraph eulerianCycle(const int V, const int E) {
        if (E <= 0) {
            throw std::invalid_argument("An Eulerian cycle must have at least one edge");
        }
        if (V <= 0) {
            throw std::invalid_argument("An Eulerian cycle must have at least one vertex");
        }

        Digraph G(V);
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
     * Returns an Eulerian path digraph on V vertices.
     */
    static Digraph eulerianPath(const int V, const int E) {
        if (E < 0) {
            throw std::invalid_argument("negative number of edges");
        }
        if (V <= 0) {
            throw std::invalid_argument("An Eulerian path must have at least one vertex");
        }

        Digraph G(V);
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
     * Returns a random digraph with strong components.
     */
    static Digraph strong(const int V, const int E, const int c) {
        if (c >= V || c <= 0) {
            throw std::invalid_argument("Number of components must be between 1 and V");
        }
        if (E <= 2 * (V - c)) {
            throw std::invalid_argument("Number of edges must be at least 2(V-c)");
        }
        if (E > static_cast<long long>(V) * (V - 1) / 2) {
            throw std::invalid_argument("Too many edges");
        }

        Digraph G(V);
        std::set<Edge> edgeSet;

        std::vector<int> label(V);
        for (int v = 0; v < V; v++) {
            label[v] = uniformInt(c);
        }

        // Make all vertices with same label a strong component
        for (int i = 0; i < c; i++) {
            // Count vertices in component i
            int count = 0;
            for (int v = 0; v < V; v++) {
                if (label[v] == i) count++;
            }

            if (count == 0) continue;

            std::vector<int> vertices(count);
            int j = 0;
            for (int v = 0; v < V; v++) {
                if (label[v] == i) vertices[j++] = v;
            }
            shuffle(vertices);

            // Rooted-in tree with root = vertices[count-1]
            for (int v = 0; v < count - 1; v++) {
                const int w = uniformInt(v + 1, count);
                Edge e(w, v);
                edgeSet.insert(e);
                G.addEdge(vertices[w], vertices[v]);
            }

            // Rooted-out tree with root = vertices[count-1]
            for (int v = 0; v < count - 1; v++) {
                const int w = uniformInt(v + 1, count);
                Edge e(v, w);
                edgeSet.insert(e);
                G.addEdge(vertices[v], vertices[w]);
            }
        }

        while (G.E() < E) {
            const int v = uniformInt(V);
            const int w = uniformInt(V);

            if (Edge e(v, w); edgeSet.find(e) == edgeSet.end() && v != w && label[v] <= label[w]) {
                edgeSet.insert(e);
                G.addEdge(v, w);
            }
        }

        return G;
    }
};

// Initialize static random number generator
std::mt19937 DigraphGenerator::rng(std::random_device{}());
