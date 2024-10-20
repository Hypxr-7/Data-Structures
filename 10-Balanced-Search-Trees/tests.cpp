#include <cassert>
#include <iostream>
#include <string>

#include "RedBlackBST.hpp"

void testRedBlackBST();

int main() {
    testRedBlackBST();

    std::cout << "All tests passed\n";

    return 0;
}

void testRedBlackBST() {
    RedBlackBST<int, std::string> bst;

    assert(bst.isEmpty());
    assert(bst.size() == 0);

    assert(bst.get(0) == std::string{});
    assert(bst.get(4) == std::string{});

    bst.put(1, "foo");

    assert(!bst.isEmpty());
    assert(bst.size() == 1);
    assert(bst.get(1) == "foo");

    bst.put(2, "bar");

    assert(!bst.isEmpty());
    assert(bst.size() == 2);
    assert(bst.get(2) == "bar");

    bst.put(3, "baz");

    assert(!bst.isEmpty());
    assert(bst.size() == 3);
    assert(bst.get(3) == "baz");

    bst.deleteMin();

    assert(bst.size() == 2);
    assert(bst.get(1) == std::string{});

    bst.deleteMin();
    bst.deleteMin();

    try {
        bst.deleteMin();
    } catch (std::underflow_error& e) {
        assert(std::string(e.what()) == "BST underflow");
    }

    bst.put(1, "foo");
    bst.put(3, "baz");
    bst.put(2, "bar");

    bst.deleteMax();

    assert(bst.size() == 2);
    assert(bst.get(3) == std::string{});

    bst.deleteMax();
    bst.deleteMax();

    try {
        bst.deleteMax();
    } catch (std::underflow_error& e) {
        assert(std::string(e.what()) == "BST underflow");
    }

    bst.put(3, "baz");
    bst.put(1, "foo");
    bst.put(2, "bar");

    bst.deleteKey(2);
    assert(bst.get(2) == std::string{});

    bst.deleteKey(1);
    assert(bst.get(1) == std::string{});

    assert(bst.get(3) == "baz");
    bst.deleteKey(3);   
    assert(bst.get(3) == std::string{});

    bst.put(3, "baz");
    bst.put(1, "foo");
    bst.put(2, "bar");

    assert(bst.min() == 1);

    bst.deleteMin();
    assert(bst.min() == 2);

    bst.deleteMin();
    assert(bst.min() == 3);

    try {
        bst.min();
    } catch (std::runtime_error& e) {
        assert(std::string(e.what()) == "calls min() with empty symbol table");
    }

    bst.put(3, "baz");
    bst.put(1, "foo");
    bst.put(2, "bar");

    assert(bst.max() == 3);

    bst.deleteMax();
    assert(bst.max() == 2);

    bst.deleteMax();
    assert(bst.max() == 1);

    try {
        bst.max();
    } catch (std::runtime_error& e) {
        assert(std::string(e.what()) == "calls max() with empty symbol table");
    }

    bst.put(1, "baz");
    bst.put(5, "foo");
    bst.put(7, "bar");

    assert(bst.floor(2) == 1);
    assert(bst.floor(5) == 5);
    assert(bst.floor(4) == 1);

    try {
        bst.floor(0);
    } catch (std::runtime_error& e) {
        assert(std::string(e.what()) == "argument to floor() is too small");
    }

    assert(bst.ceiling(1) == 1);
    assert(bst.ceiling(2) == 5);
    assert(bst.ceiling(6) == 7);

    try {
        bst.ceiling(8);
    } catch (std::runtime_error& e) {
        assert(std::string(e.what()) == "argument to ceiling() is too large");
    }

    assert(bst.select(0) == 1);
    assert(bst.select(1) == 5);
    assert(bst.select(2) == 7);

    try {
        bst.select(3);
    } catch (std::invalid_argument& e) {
        assert(std::string(e.what()) == "argument to select() is invalid");
    }
    assert(bst.rank(1) == 0);
    assert(bst.rank(7) == 2);
    assert(bst.rank(10) == 3);

    auto items = bst.keys();
    assert((items == std::vector<int>{1, 5, 7}));

    assert(bst.height() == 1);
}