#pragma once

#include <stdexcept>
#include <vector>

template <typename Key>
class IndexMaxPQ {
    std::vector<int> pq;      // Binary heap of indices (1-indexed)
    std::vector<int> qp;      // Inverse: qp[i] = position of index i in pq[]
    std::vector<Key> keys;    // keys[i] = priority of index i
    int cap;                  // Maximum capacity
    int count;                // Number of elements in PQ

    void validateIndex(const int i) const {
        if (i < 0) throw std::invalid_argument("Index is negative");
        if (i >= cap) throw std::invalid_argument("Index is out of range");
    }

    void swim(int k) {
        while (k > 1 && keys[pq[k/2]] < keys[pq[k]]) {
            exch(k/2, k);
            k /= 2;
        }
    }

    void sink(int k) {
        while (2*k <= count) {
            int j = 2 * k;  // Left child
            // Choose larger child
            if (j < count && keys[pq[j]] < keys[pq[j+1]]) ++j;
            // If parent is already larger than both children, done
            if (keys[pq[k]] >= keys[pq[j]]) break;
            exch(k, j);
            k = j;
        }
    }

    void exch(const int i, const int j) {
        const int swap = pq[i];
        pq[i] = pq[j];
        pq[j] = swap;
        qp[pq[i]] = i;
        qp[pq[j]] = j;
    }

public:
    explicit IndexMaxPQ(const int maxCap)
        : pq(maxCap + 1), qp(maxCap + 1, -1), keys(maxCap + 1), cap(maxCap), count(0) {
        if (maxCap < 0) throw std::invalid_argument("Capacity cannot be negative");
    }

    bool empty() const {
        return count == 0;
    }

    bool contains(const int i) const {
        validateIndex(i);
        return qp[i] != -1;
    }

    int size() const {
        return count;
    }

    void insert(const int i, const Key& key) {
        validateIndex(i);
        if (contains(i)) throw std::invalid_argument("Index already in PQ");

        ++count;
        qp[i] = count;      // Position of index i in heap
        pq[count] = i;      // Index at position count
        keys[i] = key;      // Priority key for index i
        swim(count);        // Restore heap order
    }

    int maxIndex() const {
        if (empty()) throw std::underflow_error("PQ is empty");
        return pq[1];  // Root of heap has maximum priority
    }

    Key maxKey() const {
        if (empty()) throw std::underflow_error("PQ is empty");
        return keys[pq[1]];
    }

    int removeMax() {
        if (empty()) throw std::underflow_error("PQ is empty");

        const int max = pq[1];        // Save max index
        exch(1, count--);       // Move last element to root, decrease size
        sink(1);                // Restore heap order
        qp[max] = -1;          // Mark as not in PQ

        return max;
    }

    Key keyOf(const int i) const {
        validateIndex(i);
        if (!contains(i)) throw std::invalid_argument("Index not in PQ");
        return keys[i];
    }


    void changeKey(const int i, const Key& key) {
        validateIndex(i);
        if (!contains(i)) throw std::invalid_argument("Index not in PQ");

        keys[i] = key;

        // Try both swim and sink to restore heap order
        swim(qp[i]);
        sink(qp[i]);
    }

    void increaseKey(const int i, const Key& key) {
        validateIndex(i);
        if (!contains(i)) throw std::invalid_argument("Index not in PQ");

        if (keys[i] == key)
            throw std::invalid_argument("Calling increaseKey() with a key equal to the key in the PQ");
        if (keys[i] > key)
            throw std::invalid_argument("Calling increaseKey() with a key that is not greater than the key in the PQ");

        keys[i] = key;
        swim(qp[i]);  // Only need to swim up since key increased
    }

    void decreaseKey(const int i, const Key& key) {
        validateIndex(i);
        if (!contains(i)) throw std::invalid_argument("Index not in PQ");

        if (keys[i] == key)
            throw std::invalid_argument("Calling decreaseKey() with a key equal to the key in the PQ");
        if (keys[i] < key)
            throw std::invalid_argument("Calling decreaseKey() with a key that is not smaller than the key in the PQ");

        keys[i] = key;
        sink(qp[i]);  // Only need to sink down since key decreased
    }

    void remove(const int i) {
        validateIndex(i);
        if (!contains(i)) throw std::invalid_argument("Index not in PQ");

        const int index = qp[i];          // Position of i in heap
        exch(index, count--);       // Move last element to position of i
        swim(index);                // Restore heap order (try both directions)
        sink(index);
        qp[i] = -1;                // Mark as not in PQ
    }

    std::vector<int> indices() const {
        std::vector<int> result;
        result.reserve(count);
        for (int i = 1; i <= count; ++i) {
            result.push_back(pq[i]);
        }
        return result;
    }

    std::vector<Key> keys_in_heap_order() const {
        std::vector<Key> result;
        result.reserve(count);
        for (int i = 1; i <= count; ++i) {
            result.push_back(keys[pq[i]]);
        }
        return result;
    }
};
