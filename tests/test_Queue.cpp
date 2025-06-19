#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../data_structures/Queue.hpp"

void test_constructor() {
    Queue<int> queue;
    assert(queue.empty());
    assert(queue.size() == 0);
    std::cout << "✓ Constructor test passed\n";
}

void test_enqueue_and_size() {
    Queue<int> queue;

    queue.enqueue(10);
    assert(!queue.empty());
    assert(queue.size() == 1);

    queue.enqueue(20);
    queue.enqueue(30);
    assert(queue.size() == 3);

    std::cout << "✓ Enqueue and size test passed\n";
}

void test_peek() {
    Queue<int> queue;

    queue.enqueue(100);
    assert(queue.peek() == 100);
    assert(queue.size() == 1);  // peek shouldn't modify size

    queue.enqueue(200);
    assert(queue.peek() == 100);  // First element should still be at front
    assert(queue.size() == 2);

    std::cout << "✓ Peek test passed\n";
}

void test_dequeue() {
    Queue<int> queue;

    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    assert(queue.dequeue() == 1);  // FIFO - first in, first out
    assert(queue.size() == 2);

    assert(queue.dequeue() == 2);
    assert(queue.size() == 1);

    assert(queue.dequeue() == 3);
    assert(queue.empty());

    std::cout << "✓ Dequeue test passed\n";
}

void test_fifo_behavior() {
    Queue<char> queue;

    std::string input = "HELLO";
    std::string expected = "HELLO";  // Same order for FIFO

    // Enqueue all characters
    for (char c : input) {
        queue.enqueue(c);
    }

    // Dequeue all characters and verify FIFO order
    std::string result;
    while (!queue.empty()) {
        result += queue.dequeue();
    }

    assert(result == expected);
    std::cout << "✓ FIFO behavior test passed\n";
}

void test_empty_queue_exceptions() {
    Queue<int> queue;

    // Test dequeue on empty queue
    try {
        queue.dequeue();
        assert(false);  // Should not reach here
    } catch (const std::underflow_error& e) {
        std::string msg = e.what();
        assert(msg == "Queue is empty");
    }

    // Test peek on empty queue
    try {
        queue.peek();
        assert(false);  // Should not reach here
    } catch (const std::underflow_error& e) {
        std::string msg = e.what();
        assert(msg == "Queue is empty");
    }

    std::cout << "✓ Empty queue exceptions test passed\n";
}

void test_single_element() {
    Queue<double> queue;

    queue.enqueue(3.14);
    assert(queue.size() == 1);
    assert(!queue.empty());
    assert(queue.peek() == 3.14);

    double value = queue.dequeue();
    assert(value == 3.14);
    assert(queue.empty());
    assert(queue.size() == 0);

    std::cout << "✓ Single element test passed\n";
}

void test_enqueue_dequeue_sequence() {
    Queue<int> queue;

    // Mixed enqueue/dequeue operations
    queue.enqueue(1);
    queue.enqueue(2);
    assert(queue.dequeue() == 1);

    queue.enqueue(3);
    queue.enqueue(4);
    assert(queue.dequeue() == 2);
    assert(queue.dequeue() == 3);
    assert(queue.dequeue() == 4);
    assert(queue.empty());

    std::cout << "✓ Enqueue/dequeue sequence test passed\n";
}

void test_string_type() {
    Queue<std::string> queue;

    queue.enqueue("first");
    queue.enqueue("second");
    queue.enqueue("third");

    assert(queue.peek() == "first");
    assert(queue.dequeue() == "first");
    assert(queue.dequeue() == "second");
    assert(queue.dequeue() == "third");
    assert(queue.empty());

    std::cout << "✓ String type test passed\n";
}

void test_large_queue() {
    Queue<int> queue;
    const int N = 10000;

    // Enqueue many elements
    for (int i = 0; i < N; ++i) {
        queue.enqueue(i);
    }

    assert(queue.size() == N);
    assert(!queue.empty());

    // Dequeue all elements in FIFO order
    for (int i = 0; i < N; ++i) {
        assert(queue.peek() == i);
        assert(queue.dequeue() == i);
        assert(queue.size() == N - i - 1);
    }

    assert(queue.empty());
    std::cout << "✓ Large queue test passed\n";
}

void test_alternating_operations() {
    Queue<int> queue;

    // Test alternating enqueue and dequeue operations
    for (int i = 0; i < 100; ++i) {
        queue.enqueue(i);
        queue.enqueue(i + 100);

        assert(queue.dequeue() == i);
        // Queue should now have one element (i + 100)
        assert(queue.size() == 1);
        assert(queue.dequeue() == i + 100);

        // Queue should now have zero elements
        assert(queue.empty());
    }

    std::cout << "✓ Alternating operations test passed\n";
}

void test_queue_after_empty() {
    Queue<int> queue;

    // Fill and empty the queue multiple times
    for (int round = 0; round < 5; ++round) {
        // Fill queue
        for (int i = 0; i < 10; ++i) {
            queue.enqueue(i + round * 10);
        }

        assert(queue.size() == 10);

        // Empty queue
        for (int i = 0; i < 10; ++i) {
            assert(queue.dequeue() == i + round * 10);
        }

        assert(queue.empty());
    }

    std::cout << "✓ Queue after empty test passed\n";
}

void test_memory_management() {
    Queue<int> queue;

    // Push and pop many times to test memory management
    for (int i = 0; i < 1000; ++i) {
        queue.enqueue(i);
    }

    assert(queue.size() == 1000);

    for (int i = 0; i < 1000; ++i) {
        assert(queue.dequeue() == i);
    }

    assert(queue.empty());
    std::cout << "✓ Memory management test passed\n";
}

void test_const_correctness() {
    Queue<int> queue;
    queue.enqueue(42);
    queue.enqueue(100);

    const Queue<int>& const_ref = queue;
    assert(const_ref.empty() == false);
    assert(const_ref.size() == 2);
    assert(const_ref.peek() == 42);

    std::cout << "✓ Const correctness test passed\n";
}

int main() {
    std::cout << "Running Queue (linked-list) tests...\n\n";

    test_constructor();
    test_enqueue_and_size();
    test_peek();
    test_dequeue();
    test_fifo_behavior();
    test_empty_queue_exceptions();
    test_single_element();
    test_enqueue_dequeue_sequence();
    test_string_type();
    test_large_queue();
    test_alternating_operations();
    test_queue_after_empty();
    test_memory_management();
    test_const_correctness();

    std::cout << "\n🎉 All tests passed!\n";
    std::cout << "Note: Run with valgrind to check for memory leaks\n";
    return 0;
}