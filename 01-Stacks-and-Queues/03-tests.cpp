#include <iostream>
#include <cassert>
#include "03-RAQueue.hpp"

void test_enqueue_dequeue() {
    ResizingArrayQueue<int> queue;

    // Test initial state
    assert(queue.isEmpty());
    assert(queue.size() == 0);

    // Enqueue elements
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);
    queue.enqueue(4);
    queue.enqueue(5);
    queue.enqueue(6);
    queue.enqueue(7);
    queue.enqueue(8);

    // Test state after enqueues
    assert(!queue.isEmpty());
    assert(queue.size() == 8);

    // Dequeue elements and test
    assert(queue.dequeue() == 1);
    assert(queue.dequeue() == 2);
    assert(queue.dequeue() == 3);
    assert(queue.dequeue() == 4);

    // Test state after dequeues
    assert(queue.size() == 4);

    // Enqueue more elements to trigger resize
    queue.enqueue(9);
    queue.enqueue(10);
    queue.enqueue(11);
    queue.enqueue(12);

    // Test state after more enqueues
    assert(queue.size() == 8);

    // Dequeue all elements to trigger shrink
    assert(queue.dequeue() == 5);
    assert(queue.dequeue() == 6);
    assert(queue.dequeue() == 7);
    assert(queue.dequeue() == 8);
    assert(queue.dequeue() == 9);
    assert(queue.dequeue() == 10);
    assert(queue.dequeue() == 11);
    assert(queue.dequeue() == 12);

    // Test final state
    assert(queue.isEmpty());
    assert(queue.size() == 0);
}

void test_peek() {
    ResizingArrayQueue<int> queue;

    // Enqueue elements
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    // Test peek
    assert(queue.peek() == 1);
    queue.dequeue();
    assert(queue.peek() == 2);
    queue.dequeue();
    assert(queue.peek() == 3);
}

void test_exceptions() {
    ResizingArrayQueue<int> queue;

    // Test dequeue exception
    try {
        queue.dequeue();
        assert(false); // Should not reach here
    } catch (const std::out_of_range& e) {
        assert(true); // Expected exception
    }

    // Test peek exception
    try {
        queue.peek();
        assert(false); // Should not reach here
    } catch (const std::out_of_range& e) {
        assert(true); // Expected exception
    }
}

int main() {
    test_enqueue_dequeue();
    test_peek();
    test_exceptions();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}