#pragma once

#include <stdexcept>

class WeightedQuickUnionUF {
private:
    int* parent;    // Array where parent[i] is the parent of element i
    int* size;      // Array storing the size of each tree (valid only for roots)
    int count_;     // Number of connected components
    int cap;        // Total capacity (number of elements)

    void validate(const int p) const {
        if (p < 0 || p >= cap) throw std::invalid_argument("p not in range");
    }

public:
    explicit WeightedQuickUnionUF(const int n)
        : parent{new int[n]},
          size{new int[n]},
          count_{n},
          cap{n} 
    {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
            size[i] = 1;
        }
    }

    ~WeightedQuickUnionUF() { 
        delete[] parent; 
        delete[] size;
    }

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

        // Weighted union: attach smaller tree to larger tree
        if (size[rootP] < size[rootQ]) {
            parent[rootP] = rootQ;
            size[rootQ] += size[rootP];
        }
        else {
            parent[rootQ] = rootP;
            size[rootP] += size[rootQ];
        }

        --count_;
    }
};
