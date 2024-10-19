#include <vector>
#include <cassert>
#include "heapsort.hpp"

void test_heap_sort() {
    // Test case 1: Basic test
    std::vector<int> vec1 = {4, 10, 3, 5, 1};
    Heap<int>::sort(vec1);
    assert((vec1 == std::vector<int>{1, 3, 4, 5, 10}));

    // Test case 2: Already sorted array
    std::vector<int> vec2 = {1, 2, 3, 4, 5};
    Heap<int>::sort(vec2);
    assert((vec2 == std::vector<int>{1, 2, 3, 4, 5}));

    // Test case 3: Reverse sorted array
    std::vector<int> vec3 = {5, 4, 3, 2, 1};
    Heap<int>::sort(vec3);
    assert((vec3 == std::vector<int>{1, 2, 3, 4, 5}));

    // Test case 4: Array with duplicate elements
    std::vector<int> vec4 = {4, 1, 3, 4, 2, 1};
    Heap<int>::sort(vec4);
    assert((vec4 == std::vector<int>{1, 1, 2, 3, 4, 4}));

    // Test case 5: Array with one element
    std::vector<int> vec5 = {1};
    Heap<int>::sort(vec5);
    assert((vec5 == std::vector<int>{1}));

    // Test case 6: Empty array
    std::vector<int> vec6 = {};
    Heap<int>::sort(vec6);
    assert((vec6 == std::vector<int>{}));

    // Test case 7: Array with negative numbers
    std::vector<int> vec7 = {3, -1, -4, 2, 0};
    Heap<int>::sort(vec7);
    assert((vec7 == std::vector<int>{-4, -1, 0, 2, 3}));
}

int main() {
    test_heap_sort();

    return 0;
}