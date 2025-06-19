#pragma once

#include <stdexcept>

class QuickUnionUF {
private:
    int* parent;  // Array where parent[i] is the parent of element i
    int count_;   // Number of connected components
    int cap;      // Total capacity (number of elements)

    void validate(const int p) const {
        if (p < 0 || p >= cap) throw std::invalid_argument("p not in range");
    }

public:
    explicit QuickUnionUF(const int n) : parent{new int[n]}, count_{n}, cap{n} {
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }

    ~QuickUnionUF() { delete[] parent; }

    int count() const { return count_; }

    int find(int p) {
        validate(p);

        while (p != parent[p])
            p = parent[p];
        return p;
    }

    bool connected(const int p, const int q) {
        validate(p);
        validate(q);

        return find(p) == find(q);
    }

    void unite(const int p, const int q) {
        validate(p);
        validate(q);

        int rootP = find(p);
        int rootQ = find(q);

        // Already in same component
        if (rootP == rootQ) return;

        // Make rootP point to rootQ (merge trees)
        parent[rootP] = rootQ;

        --count_;
    }
};
