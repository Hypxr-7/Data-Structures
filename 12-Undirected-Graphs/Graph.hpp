#pragma once

#include <vector>
#include <forward_list>

class Graph {
private:
    const int m_V;
    int m_E;
    std::vector<std::forward_list<int>> m_adj;

public:
    explicit Graph(const int vertices) : m_V(vertices), m_E(0), m_adj(vertices) {}

    int V() const { return m_V; }

    int E() const { return m_E; }

    void addEdge(const int v, const int w) {
        m_adj[v].push_front(w);
        m_adj[w].push_front(v);
        ++m_E;
    }

    std::vector<int> adj(const int v) const {
        std::vector<int> res;
        for (const int edge : m_adj[v])
            res.push_back(edge);
        return std::move(res);
    }

    std::string toString() const {
        std::string s = "Vertices: " + std::to_string(m_V) + " , Edges: " + std::to_string(m_E) + '\n';
        for (int v = 0; v < V(); ++v) {
            s += "V: " + std::to_string(v) + "\tE: ";
            for (const int edge : m_adj[v])
                s += std::to_string(edge) + ' ';
            s += '\n';
        }
        return s;
    }

    int degree(const int v) const {
        int deg = 0;
        for (const int edge : m_adj[v])
            ++deg;
        return deg;
    }

    int maxDegree() const {
        int max = 0;
        for (int v = 0; v < V(); ++v)
            if (degree(v) > max)
                max = degree(v);
        return max;
    }

    int minDegree() const {
        int min = 999999;
        for (int v = 0; v < V(); ++v)
            if (degree(v) < min)
                min = degree(v);
        return min;
    }

    int avgDegree() const { return 2 * E() / V(); }

    int numberOfSelfLoops() const {
        int count = 0;
        for (int v = 0; v < V(); ++v)
            for (const int w : m_adj[v])
                if (v == w) ++count;
        return count;
    }
};
