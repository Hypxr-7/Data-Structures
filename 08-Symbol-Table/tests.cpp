#include <cassert>
#include <iostream>

#include "ArrayST.hpp"
#include "BinarySearchST.hpp"
#include "SequentialSearchST.hpp"

void testSequentialSearchST();
void testBinarySearchST();
void testArrayST();

int main() {
    // testSequentialSearchST();
    // testBinarySearchST();
    // testArrayST();

    std::cout << "All tests passed!" << std::endl;

    return 0;
}

void testSequentialSearchST() {
    SequentialSearchST<std::string, int> st;

    // Test empty symbol table
    assert(st.size() == 0);
    assert(st.isEmpty() == true);
    assert(st.contains("key1") == false);
    assert(st.get("key1") == nullptr);

    // Test put and get
    st.put("key1", 1);
    assert(st.size() == 1);
    assert(st.isEmpty() == false);
    assert(st.contains("key1") == true);
    assert(*st.get("key1") == 1);

    // Test overwrite value
    st.put("key1", 2);
    assert(st.size() == 1);
    assert(*st.get("key1") == 2);

    // Test delete
    st.deleteKey("key1");
    assert(st.size() == 0);
    assert(st.isEmpty() == true);
    assert(st.contains("key1") == false);
    assert(st.get("key1") == nullptr);

    // Test delete non-existing key
    st.deleteKey("key2");
    assert(st.size() == 0);
    assert(st.contains("key2") == false);
    assert(st.get("key2") == nullptr);

    // Test iterator
    st.put("key1", 1);
    st.put("key2", 2);
    st.put("key3", 3);

    int sum = 0;
    auto keys = st.keys();
    while (!keys.empty()) {
        std::string k = keys.front();
        keys.pop();
        sum += *st.get(k);
    }
    assert(sum == 6);

    // Test delete non-existing key
    st.deleteKey("key4");
    assert(st.size() == 3);

    // Test delete all keys
    st.deleteKey("key1");
    st.deleteKey("key2");
    st.deleteKey("key3");
    assert(st.size() == 0);
    assert(st.isEmpty() == true);
}

void testBinarySearchST() {
    BinarySearchST<std::string, int> st;

    // Test empty symbol table
    assert(st.size() == 0);
    assert(st.isEmpty() == true);
    assert(st.contains("key1") == false);
    assert(st.get("key1") == nullptr);

    // Test put and get
    st.put("key1", 1);
    assert(st.size() == 1);
    assert(st.isEmpty() == false);
    assert(st.contains("key1") == true);
    assert(*st.get("key1") == 1);

    // Test overwrite value
    st.put("key1", 2);
    assert(st.size() == 1);
    assert(*st.get("key1") == 2);

    // Test delete
    st.deleteKey("key1");
    assert(st.size() == 0);
    assert(st.isEmpty() == true);
    assert(st.contains("key1") == false);
    assert(st.get("key1") == nullptr);

    // Test delete non-existing key
    st.deleteKey("key2");
    assert(st.size() == 0);
    assert(st.contains("key2") == false);
    assert(st.get("key2") == nullptr);

    // Test iterator
    st.put("key1", 1);
    st.put("key2", 2);
    st.put("key3", 3);

    int sum = 0;
    auto keys = st.keys();
    while (!keys.empty()) {
        std::string k = keys.front();
        keys.pop();
        sum += *st.get(k);
    }
    assert(sum == 6);

    // Test delete non-existing key
    st.deleteKey("key4");
    assert(st.size() == 3);

    // Test delete all keys
    st.deleteKey("key1");
    st.deleteKey("key2");
    st.deleteKey("key3");
    assert(st.size() == 0);
    assert(st.isEmpty() == true);

    // Test min and max
    st.put("key1", 1);
    st.put("key2", 2);
    st.put("key3", 3);
    assert(st.min() == "key1");
    assert(st.max() == "key3");

    // Test floor and ceiling
    assert(st.floor("key2") == "key2");
    assert(st.ceiling("key2") == "key2");

    // Test select
    assert(st.select(0) == "key1");
    assert(st.select(1) == "key2");
    assert(st.select(2) == "key3");

    // Test size with range
    assert(st.size("key1", "key3") == 3);
    assert(st.size("key1", "key2") == 2);

    // Test deleteMin and deleteMax
    st.deleteMin();
    assert(st.min() == "key2");
    st.deleteMax();
    assert(st.max() == "key2");
}

void testArrayST() {
    ArrayST<std::string, int> st;

    // Test isEmpty and size
    assert(st.isEmpty());
    assert(st.size() == 0);

    // Test put and get
    st.put("one", 1);
    assert(!st.isEmpty());
    assert(st.size() == 1);
    assert(st.get("one") == 1);

    st.put("two", 2);
    assert(st.size() == 2);
    assert(st.get("two") == 2);

    // Test updating value for existing key
    st.put("one", 10);
    assert(st.size() == 2);
    assert(st.get("one") == 10);

    // Test keys
    std::vector<std::string> keys = st.keys();
    assert(keys.size() == 2);
    assert((keys[0] == "one" && keys[1] == "two") || (keys[0] == "two" && keys[1] == "one"));

    // Test deleteKey
    st.deleteKey("one");
    assert(st.size() == 1);
    assert(st.get("two") == 2);
    try {
        st.get("one");
        assert(false);  // Should not reach here
    } catch (const std::runtime_error&) {
        // Expected exception
    }

    st.deleteKey("two");
    assert(st.isEmpty());
    assert(st.size() == 0);

    // Test resizing
    for (int i = 0; i < 100; i++) {
        st.put("key" + std::to_string(i), i);
    }
    assert(st.size() == 100);
    for (int i = 0; i < 100; i++) {
        assert(st.get("key" + std::to_string(i)) == i);
    }

    for (int i = 0; i < 100; i++) {
        st.deleteKey("key" + std::to_string(i));
    }
    assert(st.isEmpty());
    assert(st.size() == 0);
}
