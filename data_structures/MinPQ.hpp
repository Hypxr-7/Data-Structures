#pragma once

#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include <vector>

template <typename Key>
class MinPQ {
    std::vector<Key> pq;    // Binary heap storage (1-indexed)
    int count;              // Number of elements in the heap

    void resize(const int newCap) {
        pq.resize(newCap);
    }

    void swim(int k) {
        while (k > 1 && pq[k/2] > pq[k]) {
            std::swap(pq[k/2], pq[k]);
            k /= 2;
        }
    }

    void sink(int k) {
        while (2*k <= count) {
            int j = 2 * k;      // Left child
            // Choose smaller child if right child exists and is smaller
            if (j < count && pq[j] > pq[j+1]) ++j;
            // If parent is already smaller than both children, heap property satisfied
            if (pq[k] <= pq[j]) break;
            std::swap(pq[k], pq[j]);
            k = j;
        }
    }

    bool isMinHeapOrdered(int k = 1) const {
        if (k > count) return true;  // Base case: beyond heap bounds

        int left = 2 * k;
        int right = left + 1;

        // Check heap property with children (parent <= children for min heap)
        if (left <= count && pq[k] > pq[left]) return false;
        if (right <= count && pq[k] > pq[right]) return false;

        // Recursively check subtrees
        return isMinHeapOrdered(left) && isMinHeapOrdered(right);
    }

public:
    MinPQ() : pq(8), count(0) {}

    MinPQ(std::initializer_list<Key> items) : pq(items.size() * 2 + 1), count(static_cast<int>(items.size())) {
        // Copy items to heap (starting at index 1)
        auto it = items.begin();
        for (int i = 1; i <= count; ++i, ++it) {
            pq[i] = *it;
        }

        // Heapify: sink all non-leaf nodes from bottom to top
        for (int k = count/2; k >= 1; --k) {
            sink(k);
        }

        // assert(isMinHeapOrdered());
    }

    explicit MinPQ(const std::vector<Key>& items) : pq(items.size() * 2 + 1), count(static_cast<int>(items.size())) {
        // Copy items to heap (starting at index 1)
        for (int i = 0; i < count; ++i) {
            pq[i + 1] = items[i];
        }

        // Heapify
        for (int k = count/2; k >= 1; --k) {
            sink(k);
        }

        // assert(isMinHeapOrdered());
    }

    bool empty() const {
        return count == 0;
    }

    int size() const {
        return count;
    }

    const Key& min() const {
        if (empty()) throw std::underflow_error("Priority queue is empty");
        return pq[1];
    }

    void insert(const Key& item) {
        // Resize if needed (double capacity)
        if (count == static_cast<int>(pq.size()) - 1) {
            resize(2 * pq.size());
        }

        // Add to end and swim up
        pq[++count] = item;
        swim(count);

        // assert(isMinHeapOrdered());
    }

    Key deleteMin() {
        if (empty()) throw std::underflow_error("Priority queue is empty");

        Key min = pq[1];                    // Save minimum element
        std::swap(pq[1], pq[count--]);      // Swap with last element, then decrement
        sink(1);                            // Restore heap property

        // Shrink array if it's 1/4 full (prevent thrashing)
        if (count > 0 && count == (static_cast<int>(pq.size()) - 1) / 4) {
            resize(pq.size() / 2);
        }

        // assert(isMinHeapOrdered());
        return min;
    }

    std::vector<Key> getElements() const {
        std::vector<Key> result;
        result.reserve(count);
        for (int i = 1; i <= count; ++i) {
            result.push_back(pq[i]);
        }
        return result;
    }

    void clear() {
        count = 0;
        pq.resize(8);  // Reset to initial capacity
    }
};
