#include "DepthFirstOrder.hpp"
#include "Digraph.hpp"
#include <cassert>
#include <iostream>
#include <vector>

void testDepthFirstOrder() {
    // Create a digraph
    Digraph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 5);

    // Instantiate DepthFirstOrder
    DepthFirstOrder dfo(g);

    // Test preorder
    std::queue<int> preorder = dfo.pre();
    std::vector<int> expectedPreorder = {0, 2, 4, 5, 1, 3};
    for (int i : expectedPreorder) {
        assert(!preorder.empty());
        assert(preorder.front() == i);
        preorder.pop();
    }
    assert(preorder.empty());

    // Test postorder
    std::queue<int> postorder = dfo.post();
    std::vector<int> expectedPostorder = {5, 4, 2, 3, 1, 0};
    for (int i : expectedPostorder) {
        assert(!postorder.empty());
        assert(postorder.front() == i);
        postorder.pop();
    }
    assert(postorder.empty());

    // Test reverse postorder
    std::stack<int> reversePostorder = dfo.reversePost();
    std::vector<int> expectedReversePostorder = {0, 1, 3, 2, 4, 5};
    for (int i : expectedReversePostorder) {
        assert(!reversePostorder.empty());
        assert(reversePostorder.top() == i);
        reversePostorder.pop();
    }
    assert(reversePostorder.empty());

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    testDepthFirstOrder();
    return 0;
}