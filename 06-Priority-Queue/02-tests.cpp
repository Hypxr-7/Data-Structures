#include "MinPQ.hpp"
#include <cassert>
#include <iostream>

void testInsert() {
    MinPQ<int> pq;
    pq.insert(5);
    pq.insert(3);
    pq.insert(8);
    assert(pq.min() == 3);
    std::cout << "testInsert passed!" << std::endl;
}

void testDelMin() {
    MinPQ<int> pq;
    pq.insert(5);
    pq.insert(3);
    pq.insert(8);
    assert(pq.delMin() == 3);
    assert(pq.min() == 5);
    std::cout << "testDelMin passed!" << std::endl;
}

void testIsEmpty() {
    MinPQ<int> pq;
    assert(pq.isEmpty());
    pq.insert(1);
    assert(!pq.isEmpty());
    std::cout << "testIsEmpty passed!" << std::endl;
}

void testSize() {
    MinPQ<int> pq;
    assert(pq.size() == 0);
    pq.insert(1);
    pq.insert(2);
    assert(pq.size() == 2);
    std::cout << "testSize passed!" << std::endl;
}

int main() {
    testInsert();
    testDelMin();
    testIsEmpty();
    testSize();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}