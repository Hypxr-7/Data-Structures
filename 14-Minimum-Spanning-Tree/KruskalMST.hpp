#pragma once

#include <algorithm>
#include <queue>
#include <vector>

#include "../07-Union-Find/WeightedQuickUnionUF.hpp"
#include "Edge.hpp"
#include "EdgeWeightedGraph.hpp"

class KruskalMST {
    std::queue<Edge> mst;

public:
    explicit KruskalMST(const EdgeWeightedGraph& g) {
        std::vector<Edge> edges(g.getEdges());
        std::sort(edges.begin(), edges.end());

        WeightedQuickUnionUF uf(g.V());

        for (const Edge& edge : edges) {
            if (mst.size() == g.V() - 1) break;

            int v = edge.either();
            int w = edge.other(v);

            if (!uf.connected(v, w)) {
                uf.merge(v, w);
                mst.push(edge);
            }
        }
    }

    std::queue<Edge> edges() const { return mst; }
};
