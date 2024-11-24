#pragma once

#include <vector>

class WeightedQuickUnionUF {
private:
    std::vector<int> id;
    std::vector<int> sz;
    int mCount;
    const int mSize;
public:
    WeightedQuickUnionUF() = default;

    WeightedQuickUnionUF(int n) : id(n), sz(n), mCount(n), mSize(n) {
        for (int i = 0; i < mSize; ++i) {
            id[i] = i;
            sz[i] = 1;
        }
    }

    int count() const { return mCount; }

    bool connected(int p, int q) const { return find(p) == find(q); }

    int find(int p) const {
        while (p != id[p]) p = id[p];
        return p;
    }

    void merge(int p, int q) {
        int i = find(p);
        int j = find(q);
        
        if (i == j) return;

        if (sz[i] < sz[j]) {
            id[i] = j;
            sz[j] += sz[i];
        } else {
            id[j] = i;
            sz[i] += sz[j];
        }
        --mCount;
    }
};