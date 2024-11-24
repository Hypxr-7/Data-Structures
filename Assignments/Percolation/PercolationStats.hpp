#pragma once

#include "Percolation.hpp"

class PercolationStats {
private:
public:
    PercolationStats(const int n, const int trials);

    double mean() const;

    double stddev() const;

    double confidenceLow() const;

    double confidenceHigh() const;
};