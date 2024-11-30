#pragma once

#include <stack>
#include <vector>

#include "Graph.hpp"

class NonRecursiveDFS {
private:
    std::vector<bool> marked;

public:
    NonRecursiveDFS(const Graph& graph, const int source) : marked(graph.V(), false) {
        std::vector currentIndex(marked.size(), 0);
        std::stack<int> stk;
        marked[source] = true;
        stk.push(source);


        while (!stk.empty()) {
            const int v = stk.top();
            if (const std::vector<int>& neighbors = graph.adj(v); currentIndex[v] < neighbors.size()) {
                int w = neighbors[currentIndex[v]];
                ++currentIndex[v];
                if (!marked[w]) {
                    marked[w] = true;
                    stk.push(w);
                }
            } else {
                stk.pop();
            }
        }
    }


    bool isMarked(const int v) const {
        return marked[v];
    }
};
