#pragma once

#include <iostream>
#include <list>
#include <stdexcept>
#include <vector>

class Graph {
    const int V_;  // Number of vertices in the graph (immutable after construction)
    int E_;        // Number of edges in the graph (mutable, increases with addEdge)

    // Adjacency lists representation: vector of lists where adj_[v] contains all vertices adjacent to v
    std::vector<std::list<int>> adj_;

    void validateVertex(const int v) const {
        if (v < 0 || v >= V()) {
            throw std::invalid_argument("Vertex " + std::to_string(v) +
                                      " is out of range [0, " + std::to_string(V() - 1) + "]");
        }
    }

public:
    explicit Graph(const int v) : V_(v), E_(0), adj_(v) {
        if (v < 0) {
            throw std::invalid_argument("Number of vertices must be non-negative");
        }
    }

    int V() const { return V_; }

    int E() const { return E_; }

    void addEdge(const int v, const int w) {
        validateVertex(v);
        validateVertex(w);

        // Add w to v's adjacency list and v to w's adjacency list
        adj_[v].push_front(w);
        adj_[w].push_front(v);
        ++E_;
    }

    std::vector<int> adj(const int v) const {
        validateVertex(v);

        std::vector<int> result;

        for (const int neighbor : adj_[v]) {
            result.push_back(neighbor);
        }

        return result;
    }

    int degree(const int v) const {
        validateVertex(v);
        return static_cast<int>(adj_[v].size());
    }

    friend std::ostream& operator<<(std::ostream& os, const Graph& graph) {
        os << "Number of Vertices: " << graph.V() << '\n';
        os << "Number of Edges: " << graph.E() << '\n';

        for (int v = 0; v < graph.V(); ++v) {
            os << "V: " << v << "\tE: ";

            // Get adjacency list for this vertex
            std::vector<int> neighbors = graph.adj(v);
            for (const int neighbor : neighbors) {
                os << neighbor << ' ';
            }
            os << '\n';
        }
        return os;
    }

    int totalDegree() const {
        int total = 0;
        for (int v = 0; v < V(); ++v) {
            total += degree(v);
        }
        return total;
    }

    bool hasEdge(const int v, const int w) const {
        validateVertex(v);
        validateVertex(w);

        // Search through v's adjacency list for w
        return std::any_of(adj_[v].begin(), adj_[v].end(), [w](const int neighbor) {
            return neighbor == w;
        });
    }

    int maxDegree() const {
        int max_deg = 0;
        for (int v = 0; v < V(); ++v) {
            max_deg = std::max(max_deg, degree(v));
        }
        return max_deg;
    }

    double averageDegree() const {
        if (V() == 0) return 0.0;
        return 2.0 * static_cast<double>(E()) / static_cast<double>(V());
    }

    int numberOfSelfLoops() const {
        int count = 0;
        for (int v = 0; v < V(); ++v) {
            for (const int neighbor : adj_[v]) {
                if (neighbor == v) {
                    count++;
                }
            }
        }
        return count / 2;  // Each self-loop is counted twice in adjacency lists
    }
};