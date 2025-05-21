#pragma once

#include <vector>

#include "Digraph.hpp"
#include ""

class Topological {
    std::vector<int> _order;
    std::vector<int> _rank;

public:
    Topological(const Digraph& g);

    std::vector<int> order() const;

    bool hasOrder() const;

    int rank(const int v) const;


};