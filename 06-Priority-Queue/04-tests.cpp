#include <iostream>
#include <cassert>
#include "IndexMaxPQ.hpp"

void testInsertAndMax() {
    IndexMaxPQ<int> pq(10);
    pq.insert(0, 10);
    pq.insert(1, 20);
    pq.insert(2, 15);

    assert(pq.maxIndex() == 1);
    assert(pq.maxKey() == 20);
    std::cout << "testInsertAndMax passed!" << std::endl;
}

void testDelMax() {
    IndexMaxPQ<int> pq(10);
    pq.insert(0, 10);
    pq.insert(1, 20);
    pq.insert(2, 15);

    assert(pq.delMax() == 1);
    assert(pq.maxIndex() == 2);
    assert(pq.maxKey() == 15);
    std::cout << "testDelMax passed!" << std::endl;
}

void testChangeKey() {
    IndexMaxPQ<int> pq(10);
    pq.insert(0, 10);
    pq.insert(1, 20);
    pq.insert(2, 15);

    pq.changeKey(0, 25);
    assert(pq.maxIndex() == 0);
    assert(pq.maxKey() == 25);
    std::cout << "testChangeKey passed!" << std::endl;
}

void testIncreaseKey() {
    IndexMaxPQ<int> pq(10);
    pq.insert(0, 10);
    pq.insert(1, 20);
    pq.insert(2, 15);

    pq.increaseKey(2, 30);
    assert(pq.maxIndex() == 2);
    assert(pq.maxKey() == 30);
    std::cout << "testIncreaseKey passed!" << std::endl;
}

void testDecreaseKey() {
    IndexMaxPQ<int> pq(10);
    pq.insert(0, 10);
    pq.insert(1, 20);
    pq.insert(2, 15);

    pq.decreaseKey(1, 5);
    assert(pq.maxIndex() == 2);
    assert(pq.maxKey() == 15);
    std::cout << "testDecreaseKey passed!" << std::endl;
}

void testDeleteKey() {
    IndexMaxPQ<int> pq(10);
    pq.insert(0, 10);
    pq.insert(1, 20);
    pq.insert(2, 15);

    pq.deleteKey(1);
    assert(!pq.contains(1));
    assert(pq.maxIndex() == 2);
    assert(pq.maxKey() == 15);
    std::cout << "testDeleteKey passed!" << std::endl;
}

int main() {
    testInsertAndMax();
    testDelMax();
    testChangeKey();
    testIncreaseKey();
    testDecreaseKey();
    testDeleteKey();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}