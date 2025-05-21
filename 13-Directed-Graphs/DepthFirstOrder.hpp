#pragma once

#include <vector>
#include <queue>
#include <stack>
#include "Digraph.hpp"

class DepthFirstOrder {
private:
    std::vector<bool> marked;          // marked[v] = has v been marked in dfs?
    std::vector<int> _pre;              // pre[v]    = preorder number of v
    std::vector<int> _post;             // post[v]   = postorder number of v
    std::queue<int> _preorder;          // vertices in preorder
    std::queue<int> _postorder;         // vertices in postorder
    int preCounter;                    // counter for preorder numbering
    int postCounter;                   // counter for postorder numbering

    // Run DFS in digraph G from vertex v and compute preorder/postorder
    void dfs(const Digraph& G, int v) {
        marked[v] = true;
        _pre[v] = preCounter++;
        _preorder.push(v);
        for (int w : G.adj(v)) {
            if (!marked[w]) {
                dfs(G, w);
            }
        }
        _postorder.push(v);
        _post[v] = postCounter++;
    }

public:
    // Constructor for Digraph
    explicit DepthFirstOrder(const Digraph& G)
        : marked(G.V(), false),
          _pre(G.V(), 0),
          _post(G.V(), 0),
          preCounter(0),
          postCounter(0) {
        for (int v = 0; v < G.V(); ++v) {
            if (!marked[v]) {
                dfs(G, v);
            }
        }
    }

    // Returns the preorder number of vertex v
    int pre(int v) const {
        return _pre[v];
    }

    // Returns the postorder number of vertex v
    int post(int v) const {
        return _post[v];
    }

    // Returns the vertices in postorder
    std::queue<int> post() const {
        return _postorder;
    }

    // Returns the vertices in preorder
    std::queue<int> pre() const {
        return _preorder;
    }

    // Returns the vertices in reverse postorder
    std::stack<int> reversePost() const {
        std::stack<int> reverse;
        std::queue<int> tempPostorder = _postorder;
        while (!tempPostorder.empty()) {
            reverse.push(tempPostorder.front());
            tempPostorder.pop();
        }
        return reverse;
    }
};
