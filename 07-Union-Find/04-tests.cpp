#include "QuickUnionPathCompressionUF.hpp"
#include <cassert>
#include <iostream>

void testQuickUnionUF()
{
    QuickUnionPathCompressionUF uf(10);

    // Initial count should be 10
    assert(uf.count() == 10);

    // Initially, no elements are connected
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            assert((i == j) == uf.connected(i, j));

    // Connect some elements
    uf.merge(4, 3);
    uf.merge(3, 8);
    uf.merge(6, 5);
    uf.merge(9, 4);
    uf.merge(2, 1);

    // Test connections
    assert(uf.connected(4, 3));
    assert(uf.connected(3, 8));
    assert(uf.connected(4, 8));
    assert(uf.connected(9, 8));
    assert(uf.connected(6, 5));
    assert(!uf.connected(0, 1));
    assert(!uf.connected(5, 4));

    // Test count after some merges
    assert(uf.count() == 5);

    // Connect more elements
    uf.merge(5, 0);
    uf.merge(7, 2);
    uf.merge(6, 1);
    uf.merge(7, 3);

    // Test final connections
    assert(uf.connected(0, 5));
    assert(uf.connected(1, 7));
    assert(uf.connected(2, 7));
    assert(uf.connected(3, 4));
    assert(uf.connected(4, 9));
    assert(uf.connected(5, 6));
    assert(uf.connected(6, 1));
    assert(uf.connected(7, 8));
    assert(uf.connected(8, 9));

    // Test final count
    assert(uf.count() == 1);

    std::cout << "All tests passed!" << std::endl;
}

int main()
{
    testQuickUnionUF();
    return 0;
}