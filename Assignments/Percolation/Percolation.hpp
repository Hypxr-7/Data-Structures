#pragma once

#include <cassert>
#include <iostream>
#include <vector>

#include "WeightedQuickUnionUF.hpp"

class Percolation {
private:
    WeightedQuickUnionUF uf;
    std::vector<bool> sites;
    const int size;
    const int virtualTop;
    const int virtualBottom;

public:
    Percolation(const int n) : uf(n * n + 2), sites(n * n, false), size(n), virtualTop(n * n), virtualBottom(n * n + 1) {}

    void open(const int row, const int col) {
        sites[row * size + col] = true;

        if (row == 0)  // top row
            uf.merge(row * size + col, virtualTop);
        if (row == size - 1)  // bottom row
            uf.merge(row * size + col, virtualBottom);

        if (col >= 1 && isOpen(row, col - 1))  // left site
            uf.merge(row * size + col, row * size + col - 1);
        if (col < size - 1 && isOpen(row, col + 1))  // right size
            uf.merge(row * size + col, row * size + col + 1);
        if (row >= 1 && isOpen(row - 1, col))  // top site
            uf.merge(row * size + col, (row - 1) * size + col);
        if (row < size - 1 && isOpen(row + 1, col))  // bottom site
            uf.merge(row * size + col, (row + 1) * size + col);
    }

    bool isOpen(const int row, const int col) const { return sites[row * size + col]; }

    bool isFull(const int row, const int col) const { return uf.connected(row * size + col, virtualTop); }

    int numberOfSites() const { return size; }

    bool percolates() const { return uf.connected(virtualTop, virtualBottom); }
};