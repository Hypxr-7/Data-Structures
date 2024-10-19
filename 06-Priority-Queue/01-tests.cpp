#include <cassert>
#include <iostream>
#include "MaxPQ.hpp"

void testInsert() {
    MaxPQ<int> pq;
    pq.insert(10);
    pq.insert(20);
    pq.insert(5);
    assert(pq.max() == 20);
}

void testDelMax() {
    MaxPQ<int> pq;
    pq.insert(10);
    pq.insert(20);
    pq.insert(5);
    assert(pq.delMax() == 20);
    assert(pq.delMax() == 10);
    assert(pq.delMax() == 5);
}

void testIsEmpty() {
    MaxPQ<int> pq;
    assert(pq.isEmpty());
    pq.insert(10);
    assert(!pq.isEmpty());
}

void testSize() {
    MaxPQ<int> pq;
    assert(pq.size() == 0);
    pq.insert(10);
    pq.insert(20);
    assert(pq.size() == 2);
}

int main() {
    testInsert();
    testDelMax();
    testIsEmpty();
    testSize();
    std::cout << "All tests passed\n";

    return 0;
}