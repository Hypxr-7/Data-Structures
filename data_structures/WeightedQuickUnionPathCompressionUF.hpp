#pragma once

#include <stdexcept>

class WeightedQuickUnionPathCompressionUF {
private:
    int* parent;    // Array where parent[i] is the parent of element i
    int* size;      // Array storing the size of each tree (valid only for roots)
    int count_;     // Number of connected components
    int cap;        // Total capacity (number of elements)

    void validate(const int p) const {
        if (p < 0 || p >= cap) throw std::invalid_argument("p not in range");
    }

public:
    explicit WeightedQuickUnionPathCompressionUF(const int n)
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

    ~WeightedQuickUnionPathCompressionUF() { 
        delete[] parent; 
        delete[] size;
    }

    int count() const { return count_; }

    int find(int p) {
        validate(p);

        // First pass: find root
        int root = p;
        while (root != parent[root])
            root = parent[root];
        
        // Second pass: path compression - make every node point to root
        while (p != root) {
            int newp = parent[p];
            parent[p] = root;
            p = newp;
        }
        return root;
    }

    bool connected(const int p, const int q) {
        validate(p);
        validate(q);

        return find(p) == find(q);
    }

    void unite(const int p, const int q) {
        validate(p);
        validate(q);

        int rootP = find(p);  // Path compression applied here
        int rootQ = find(q);  // Path compression applied here

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
