#pragma once

class Edge {
    int v, w;
    double weight;

public:
    Edge(const int v, const int w, const double weight) : v(v), w(w), weight(weight) {}

    int either() const { return v; }

    int other(const int vertex) const {
        return vertex == v ? w : v;
    }

    double getWeight() const { return weight; }

    bool operator==(const Edge& other) const { return weight == other.weight; }
    bool operator<(const Edge& other) const { return weight < other.weight; }
    bool operator>(const Edge& other) const { return weight > other.weight; }
};
