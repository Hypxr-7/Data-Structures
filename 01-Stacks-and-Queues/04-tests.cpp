#include <iostream>
#include <cassert>
#include "04-LLStack.hpp"

void testStack() {
    Stack<int> stack;

    // Test isEmpty on an empty stack
    assert(stack.isEmpty() == true);
    assert(stack.size() == 0);

    // Test push
    stack.push(1);
    assert(stack.isEmpty() == false);
    assert(stack.size() == 1);
    assert(stack.peek() == 1);

    stack.push(2);
    assert(stack.size() == 2);
    assert(stack.peek() == 2);

    stack.push(3);
    assert(stack.size() == 3);
    assert(stack.peek() == 3);

    // Test pop
    assert(stack.pop() == 3);
    assert(stack.size() == 2);
    assert(stack.peek() == 2);

    assert(stack.pop() == 2);
    assert(stack.size() == 1);
    assert(stack.peek() == 1);

    assert(stack.pop() == 1);
    assert(stack.isEmpty() == true);
    assert(stack.size() == 0);

    // Test pop on an empty stack (should throw an exception)
    try {
        stack.pop();
        assert(false); // Should not reach here
    } catch (const std::runtime_error& e) {
        assert(true); // Exception was thrown as expected
    }

    // Test peek on an empty stack (should throw an exception)
    try {
        stack.peek();
        assert(false); // Should not reach here
    } catch (const std::runtime_error& e) {
        assert(true); // Exception was thrown as expected
    }

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    testStack();
    return 0;
}