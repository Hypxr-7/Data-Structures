#include <iostream>
#include <cassert>
#include "02-RAStack.hpp"

void testResizingArrayStack() {
    ResizingArrayStack<int> stack;

    // Test isEmpty on a new stack
    assert(stack.isEmpty());
    assert(stack.size() == 0);

    // Test push and size
    stack.push(1);
    assert(!stack.isEmpty());
    assert(stack.size() == 1);
    assert(stack.peek() == 1);

    stack.push(2);
    assert(stack.size() == 2);
    assert(stack.peek() == 2);

    // Test pop
    int item = stack.pop();
    assert(item == 2);
    assert(stack.size() == 1);
    assert(stack.peek() == 1);

    item = stack.pop();
    assert(item == 1);
    assert(stack.isEmpty());
    assert(stack.size() == 0);

    // Test underflow
    try {
        stack.pop();
        assert(false);  // Should not reach here
    } catch (const std::out_of_range& e) {
        assert(true);  // Expected exception
    }

    // Test resizing
    for (int i = 0; i < 100; ++i) {
        stack.push(i);
    }
    assert(stack.size() == 100);
    assert(stack.peek() == 99);

    for (int i = 99; i >= 0; --i) {
        assert(stack.pop() == i);
    }
    assert(stack.isEmpty());
    assert(stack.size() == 0);

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    testResizingArrayStack();
    return 0;
}