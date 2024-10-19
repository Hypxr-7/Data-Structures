#include <cassert>
#include <iostream>
#include "IndexMinPQ.hpp"

void testInsertAndMin() {
    IndexMinPQ<int> pq(10);
    pq.insert(0, 5);
    pq.insert(1, 3);
    pq.insert(2, 9);
    assert(pq.minIndex() == 1);
    assert(pq.minKey() == 3);
    std::cout << "testInsertAndMin passed!" << std::endl;
}

void testDelMin() {
    IndexMinPQ<int> pq(10);
    pq.insert(0, 5);
    pq.insert(1, 3);
    pq.insert(2, 9);
    assert(pq.delMin() == 1);
    assert(pq.minIndex() == 0);
    assert(pq.minKey() == 5);
    std::cout << "testDelMin passed!" << std::endl;
}

void testChangeKey() {
    IndexMinPQ<int> pq(10);
    pq.insert(0, 5);
    pq.insert(1, 3);
    pq.insert(2, 9);
    pq.changeKey(2, 1);
    assert(pq.minIndex() == 2);
    assert(pq.minKey() == 1);
    std::cout << "testChangeKey passed!" << std::endl;
}

void testDecreaseKey() {
    IndexMinPQ<int> pq(10);
    pq.insert(0, 5);
    pq.insert(1, 3);
    pq.insert(2, 9);
    pq.decreaseKey(2, 2);
    assert(pq.minIndex() == 2);
    assert(pq.minKey() == 2);
    std::cout << "testDecreaseKey passed!" << std::endl;
}

void testIncreaseKey() {
    IndexMinPQ<int> pq(10);
    pq.insert(0, 5);
    pq.insert(1, 3);
    pq.insert(2, 9);
    pq.increaseKey(1, 10);
    assert(pq.minIndex() == 0);
    assert(pq.minKey() == 5);
    std::cout << "testIncreaseKey passed!" << std::endl;
}

void testDeleteKey() {
    IndexMinPQ<int> pq(10);
    pq.insert(0, 5);
    pq.insert(1, 3);
    pq.insert(2, 9);
    pq.deleteKey(1);
    assert(!pq.contains(1));
    assert(pq.minIndex() == 0);
    assert(pq.minKey() == 5);
    std::cout << "testDeleteKey passed!" << std::endl;
}

void testIterator() {
    IndexMinPQ<int> pq(10);
    pq.insert(0, 5);
    pq.insert(1, 3);
    pq.insert(2, 9);
    auto it = pq.begin();
    assert(it.hasNext());
    assert(it.next() == 1);
    assert(it.hasNext());
    assert(it.next() == 0);
    assert(it.hasNext());
    assert(it.next() == 2);
    assert(!it.hasNext());
    std::cout << "testIterator passed!" << std::endl;
}

int main() {
    testInsertAndMin();
    testDelMin();
    testChangeKey();
    testDecreaseKey();
    testIncreaseKey();
    testDeleteKey();
    testIterator();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}