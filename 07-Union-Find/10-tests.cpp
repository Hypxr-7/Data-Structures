#include <iostream>
#include <cassert>
#include "WeightedQuickUnionRAUF.hpp"

void testInitialization() {
    WeightedQuickUnionRAUF uf(10);
    for (int i = 0; i < 10; ++i) {
        assert(uf.find(i) == i);
    }
    assert(uf.count() == 10);
    std::cout << "testInitialization passed!" << std::endl;
}

void testFind() {
    WeightedQuickUnionRAUF uf(10);
    assert(uf.find(0) == 0);
    assert(uf.find(9) == 9);
    std::cout << "testFind passed!" << std::endl;
}

void testConnected() {
    WeightedQuickUnionRAUF uf(10);
    assert(!uf.connected(0, 1));
    uf.merge(0, 1);
    assert(uf.connected(0, 1));
    std::cout << "testConnected passed!" << std::endl;
}

void testMerge() {
    WeightedQuickUnionRAUF uf(10);
    uf.merge(0, 1);
    assert(uf.connected(0, 1));
    assert(uf.find(0) == uf.find(1));

    uf.merge(1, 2);
    assert(uf.connected(0, 2));
    assert(uf.find(0) == uf.find(2));
    std::cout << "testMerge passed!" << std::endl;
}

void testNewSite() {
    WeightedQuickUnionRAUF uf(10);
    int newSiteId = uf.newSite();
    assert(newSiteId == 10);
    assert(uf.find(newSiteId) == newSiteId);
    assert(uf.count() == 11);
    std::cout << "testNewSite passed!" << std::endl;
}

int main() {
    testInitialization();
    testFind();
    testConnected();
    testMerge();
    testNewSite();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}