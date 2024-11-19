
#include <cassert>
#include <iostream>

#include "LinearProbingHashST.hpp"

void test();

int main() {
    test();
    std::cout << "All tests passed\n";

    return 0;
}

void test() {
    LinearProbingHashST<int, std::string> st;

    // Test put and get
    st.put(1, "one");
    st.put(2, "two");
    st.put(3, "three");
    assert(st.get(1) == "one");
    assert(st.get(2) == "two");
    assert(st.get(3) == "three");

    // Test size
    assert(st.size() == 3);

    // Test contains
    assert(st.contains(1));
    assert(st.contains(2));
    assert(st.contains(3));
    assert(!st.contains(4));

    // Test remove
    st.deleteKey(2);
    assert(!st.contains(2));
    assert(st.size() == 2);

    // Test get after deleteKey
    assert(st.get(1) == "one");
    assert(st.get(3) == "three");

    // Test update value
    st.put(1, "ONE");
    assert(st.get(1) == "ONE");

    // Test empty
    st.deleteKey(1);
    st.deleteKey(3);
    assert(st.isEmpty());

    // Test put and get with more elements
    st.put(4, "four");
    st.put(5, "five");
    st.put(6, "six");
    assert(st.get(4) == "four");
    assert(st.get(5) == "five");
    assert(st.get(6) == "six");

    // Test size after adding more elements
    assert(st.size() == 3);

    // Test contains with more elements
    assert(st.contains(4));
    assert(st.contains(5));
    assert(st.contains(6));
    assert(!st.contains(7));

    // Test remove with more elements
    st.deleteKey(5);
    assert(!st.contains(5));
    assert(st.size() == 2);

    // Test get after deleteKey with more elements
    assert(st.get(4) == "four");
    assert(st.get(6) == "six");

    // Test update value with more elements
    st.put(4, "FOUR");
    assert(st.get(4) == "FOUR");

    // Test getKeys method
    auto keys = st.getKeys();
    std::vector<int> expectedKeys = {4, 6};
    assert(keys == expectedKeys);

    // Test empty with more elements
    st.deleteKey(4);
    st.deleteKey(6);
    assert(st.isEmpty());
}
