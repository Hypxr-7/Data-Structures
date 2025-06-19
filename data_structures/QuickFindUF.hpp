#pragma once

#include <stdexcept>


class QuickFindUF {
private:
    int* id;     // Array storing component identifier for each element
    int count_;  // Number of connected components
    int cap;     // Total capacity (number of elements)

    void validate(const int p) const {
        if (p < 0 || p >= cap) throw std::invalid_argument("p not in range");
    }

public:
    explicit QuickFindUF(const int n) : id{new int[n]}, count_{n}, cap{n} {
        for (int i = 0; i < n; ++i)
            id[i] = i;
    }

    ~QuickFindUF() { delete[] id; }

    int count() const { return count_; }

    int find(const int p) const {
        validate(p);
        return id[p];
    }

    bool connected(const int p, const int q) const {
        validate(p);
        validate(q);
        return id[p] == id[q];
    }

    void unite(const int p, const int q) {
        validate(p);
        validate(q);

        int pID = id[p];
        int qID = id[q];

        // Already in same component
        if (pID == qID) return;

        // Change all entries with pID to qID
        for (int i = 0; i < cap; ++i)
            if (id[i] == pID) id[i] = qID;

        --count_;
    }
};
