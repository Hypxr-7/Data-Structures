/*
 *  Solves the Josephus problem
 */

#include <iostream>
#include <sstream>
#include <string>
#include "05-LLQueue.hpp"

void josephus(int n, int m);

// Helper function to capture the output of josephus function
std::string captureJosephusOutput(int n, int m);
void testJosephus();

int main() {
    testJosephus();

    return 0;
}

void josephus(int n, int m) {
    Queue<int> queue;

    // initialize
    for (int i = 0; i < n; ++i)
        queue.enqueue(i);

    // eliminate
    while (!queue.isEmpty()) {
        for (int i = 0; i < m - 1; ++i)
            queue.enqueue(queue.dequeue());
        std::cout << queue.dequeue() << ' ';
    }
    std::cout << '\n';
}

std::string captureJosephusOutput(int n, int m) {
    std::ostringstream oss;
    std::streambuf* oldCoutBuf = std::cout.rdbuf(oss.rdbuf());
    josephus(n, m);
    std::cout.rdbuf(oldCoutBuf);
    return oss.str();
}

void testJosephus() {
    // Test case 1: n = 7, m = 3
    std::string result = captureJosephusOutput(7, 3);
    std::string expected = "2 5 1 6 4 0 3 \n";
    if (result == expected) {
        std::cout << "Test case 1 passed.\n";
    } else {
        std::cout << "Test case 1 failed. Expected: " << expected << " but got: " << result << "\n";
    }

    // Test case 2: n = 5, m = 2
    result = captureJosephusOutput(5, 2);
    expected = "1 3 0 4 2 \n";
    if (result == expected) {
        std::cout << "Test case 2 passed.\n";
    } else {
        std::cout << "Test case 2 failed. Expected: " << expected << " but got: " << result << "\n";
    }

    // Test case 3: n = 6, m = 4
    result = captureJosephusOutput(6, 4);
    expected = "3 1 0 2 5 4 \n";
    if (result == expected) {
        std::cout << "Test case 3 passed.\n";
    } else {
        std::cout << "Test case 3 failed. Expected: " << expected << " but got: " << result << "\n";
    }
}