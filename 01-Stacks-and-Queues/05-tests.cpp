#include <iostream>
#include <cassert>
#include "05-LLQueue.hpp"

void testQueueSize() {
    Queue<int> queue;

    // Test size of an empty queue
    assert(queue.size() == 0);
    std::cout << "Test passed: size of an empty queue is 0." << std::endl;

    // Enqueue elements and test size
    queue.enqueue(1);
    assert(queue.size() == 1);
    std::cout << "Test passed: size after enqueueing 1 element is 1." << std::endl;

    queue.enqueue(2);
    assert(queue.size() == 2);
    std::cout << "Test passed: size after enqueueing 2 elements is 2." << std::endl;

    // Dequeue elements and test size
    queue.dequeue();
    assert(queue.size() == 1);
    std::cout << "Test passed: size after dequeueing 1 element is 1." << std::endl;

    queue.dequeue();
    assert(queue.size() == 0);
    std::cout << "Test passed: size after dequeueing all elements is 0." << std::endl;
}

void testQueueIsEmpty() {
    Queue<int> queue;

    // Test if the queue is empty
    assert(queue.isEmpty());
    std::cout << "Test passed: queue is empty initially." << std::endl;

    // Enqueue an element and test if the queue is not empty
    queue.enqueue(1);
    assert(!queue.isEmpty());
    std::cout << "Test passed: queue is not empty after enqueueing an element." << std::endl;

    // Dequeue the element and test if the queue is empty again
    queue.dequeue();
    assert(queue.isEmpty());
    std::cout << "Test passed: queue is empty after dequeueing all elements." << std::endl;
}

void testQueuePeek() {
    Queue<int> queue;

    // Test peek on an empty queue (should throw an exception)
    try {
        queue.peek();
        std::cerr << "Test failed: peek on an empty queue should throw an exception." << std::endl;
    } catch (const std::runtime_error&) {
        std::cout << "Test passed: peek on an empty queue throws an exception." << std::endl;
    }

    // Enqueue elements and test peek
    queue.enqueue(1);
    assert(queue.peek() == 1);
    std::cout << "Test passed: peek returns the first element (1)." << std::endl;

    queue.enqueue(2);
    assert(queue.peek() == 1);
    std::cout << "Test passed: peek still returns the first element (1) after enqueueing another element." << std::endl;

    // Dequeue an element and test peek
    queue.dequeue();
    assert(queue.peek() == 2);
    std::cout << "Test passed: peek returns the new first element (2) after dequeueing." << std::endl;
}

void testQueueEnqueueDequeue() {
    Queue<int> queue;

    // Enqueue and dequeue multiple elements
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    assert(queue.dequeue() == 1);
    std::cout << "Test passed: dequeue returns the first element (1)." << std::endl;

    assert(queue.dequeue() == 2);
    std::cout << "Test passed: dequeue returns the second element (2)." << std::endl;

    assert(queue.dequeue() == 3);
    std::cout << "Test passed: dequeue returns the third element (3)." << std::endl;

    // Test dequeue on an empty queue (should throw an exception)
    try {
        queue.dequeue();
        std::cerr << "Test failed: dequeue on an empty queue should throw an exception." << std::endl;
    } catch (const std::runtime_error&) {
        std::cout << "Test passed: dequeue on an empty queue throws an exception." << std::endl;
    }
}

int main() {
    testQueueSize();
    testQueueIsEmpty();
    testQueuePeek();
    testQueueEnqueueDequeue();
    return 0;
}