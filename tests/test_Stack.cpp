#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../data_structures/Stack.hpp"

void test_constructor() {
    Stack<int> stack;
    assert(stack.empty());
    assert(stack.size() == 0);
    std::cout << "✓ Constructor test passed\n";
}

void test_push_and_size() {
    Stack<int> stack;

    stack.push(10);
    assert(!stack.empty());
    assert(stack.size() == 1);

    stack.push(20);
    stack.push(30);
    assert(stack.size() == 3);

    std::cout << "✓ Push and size test passed\n";
}

void test_peek() {
    Stack<int> stack;

    stack.push(100);
    assert(stack.peek() == 100);
    assert(stack.size() == 1);  // peek shouldn't modify size

    stack.push(200);
    assert(stack.peek() == 200);
    assert(stack.size() == 2);

    std::cout << "✓ Peek test passed\n";
}

void test_pop() {
    Stack<int> stack;

    stack.push(1);
    stack.push(2);
    stack.push(3);

    assert(stack.pop() == 3);
    assert(stack.size() == 2);

    assert(stack.pop() == 2);
    assert(stack.size() == 1);

    assert(stack.pop() == 1);
    assert(stack.empty());

    std::cout << "✓ Pop test passed\n";
}

void test_pop_memory_management() {
    Stack<int> stack;

    // Push and pop many times to test memory management
    for (int i = 0; i < 100; ++i) {
        stack.push(i);
    }

    assert(stack.size() == 100);

    for (int i = 99; i >= 0; --i) {
        assert(stack.pop() == i);
    }

    assert(stack.empty());
    std::cout << "✓ Pop memory management test passed\n";
}

void test_empty_stack_exceptions() {
    Stack<int> stack;

    // Test pop on empty stack
    try {
        stack.pop();
        assert(false);  // Should not reach here
    } catch (const std::underflow_error& e) {
        std::string msg = e.what();
        assert(msg == "Stack is empty");
    }

    // Test peek on empty stack
    try {
        stack.peek();
        assert(false);  // Should not reach here
    } catch (const std::underflow_error& e) {
        std::string msg = e.what();
        assert(msg == "Stack is empty");
    }

    std::cout << "✓ Empty stack exceptions test passed\n";
}

void test_string_type() {
    Stack<std::string> stack;

    stack.push("hello");
    stack.push("world");
    stack.push("!");

    assert(stack.peek() == "!");
    assert(stack.pop() == "!");
    assert(stack.pop() == "world");
    assert(stack.pop() == "hello");
    assert(stack.empty());

    std::cout << "✓ String type test passed\n";
}

void test_lifo_behavior() {
    Stack<char> stack;

    std::string input = "HELLO";
    std::string expected = "OLLEH";

    // Push all characters
    for (char c : input) {
        stack.push(c);
    }

    // Pop all characters and verify LIFO order
    std::string result;
    while (!stack.empty()) {
        result += stack.pop();
    }

    assert(result == expected);
    std::cout << "✓ LIFO behavior test passed\n";
}

void test_single_element() {
    Stack<double> stack;

    stack.push(3.14);
    assert(stack.size() == 1);
    assert(!stack.empty());
    assert(stack.peek() == 3.14);

    double value = stack.pop();
    assert(value == 3.14);
    assert(stack.empty());
    assert(stack.size() == 0);

    std::cout << "✓ Single element test passed\n";
}

void test_push_pop_sequence() {
    Stack<int> stack;

    // Mixed push/pop operations
    stack.push(1);
    stack.push(2);
    assert(stack.pop() == 2);

    stack.push(3);
    stack.push(4);
    assert(stack.pop() == 4);
    assert(stack.pop() == 3);
    assert(stack.pop() == 1);
    assert(stack.empty());

    std::cout << "✓ Push/pop sequence test passed\n";
}

void test_large_stack() {
    Stack<int> stack;
    const int N = 10000;

    // Push many elements
    for (int i = 0; i < N; ++i) {
        stack.push(i);
    }

    assert(stack.size() == N);
    assert(!stack.empty());

    // Pop all elements in reverse order
    for (int i = N - 1; i >= 0; --i) {
        assert(stack.peek() == i);
        assert(stack.pop() == i);
        assert(stack.size() == i);
    }

    assert(stack.empty());
    std::cout << "✓ Large stack test passed\n";
}

void test_destructor_cleanup() {
    // This test verifies destructor is called properly
    // Memory leaks would be detected by tools like valgrind
    {
        Stack<int> stack;
        for (int i = 0; i < 1000; ++i) {
            stack.push(i);
        }
        // Stack destructor should be called here
    }

    std::cout << "✓ Destructor cleanup test passed\n";
}

void test_const_correctness() {
    Stack<int> stack;
    stack.push(42);
    stack.push(100);

    const Stack<int>& const_ref = stack;
    assert(const_ref.empty() == false);
    assert(const_ref.size() == 2);
    assert(const_ref.peek() == 100);

    std::cout << "✓ Const correctness test passed\n";
}

int main() {
    std::cout << "Running Stack (linked-list) tests...\n\n";

    test_constructor();
    test_push_and_size();
    test_peek();
    test_pop();
    test_pop_memory_management();
    test_empty_stack_exceptions();
    test_string_type();
    test_lifo_behavior();
    test_single_element();
    test_push_pop_sequence();
    test_large_stack();
    test_destructor_cleanup();
    test_const_correctness();

    std::cout << "\n🎉 All tests passed!\n";
    std::cout << "Note: Run with valgrind to check for memory leaks\n";
    return 0;
}