#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

template <typename Key>
class IndexMaxPQ {
private:
    int maxN;        // maximum number of elements on PQ
    int n;           // number of elements on PQ
    std::vector<int> pq;        // binary heap using 1-based indexing
    std::vector<int> qp;        // inverse of pq - qp[pq[i]] = pq[qp[i]] = i
    std::vector<Key> keys;      // keys[i] = priority of i

    // Validate if the index is within the valid range
    void validateIndex(int i) const {
        if (i < 0) throw std::invalid_argument("index is negative: " + std::to_string(i));
        if (i >= maxN) throw std::invalid_argument("index >= capacity: " + std::to_string(i));
    }

    // Restore the heap order by swimming up the element at index k
    void swim(int k) {
        while (k > 1 && keys[pq[k / 2]] < keys[pq[k]]) {
            std::swap(pq[k], pq[k / 2]);
            qp[pq[k]] = k;
            qp[pq[k / 2]] = k / 2;
            k = k / 2;
        }
    }

    // Restore the heap order by sinking down the element at index k
    void sink(int k) {
        while (2 * k <= n) {
            int j = 2 * k;
            if (j < n && keys[pq[j]] < keys[pq[j + 1]]) j++;
            if (!(keys[pq[k]] < keys[pq[j]])) break;
            std::swap(pq[k], pq[j]);
            qp[pq[k]] = k;
            qp[pq[j]] = j;
            k = j;
        }
    }

public:
    // Constructor to initialize an empty indexed priority queue with indices between 0 and maxN - 1
    IndexMaxPQ(int maxN) : maxN(maxN), n(0), pq(maxN + 1), qp(maxN + 1, -1), keys(maxN + 1) {
        if (maxN < 0) throw std::invalid_argument("maxN < 0");
    }

    // Check if the priority queue is empty
    bool isEmpty() const {
        return n == 0;
    }

    // Check if the index i is in the priority queue
    bool contains(int i) const {
        validateIndex(i);
        return qp[i] != -1;
    }

    // Return the number of keys in the priority queue
    int size() const {
        return n;
    }

    // Associate key with index i
    void insert(int i, Key key) {
        validateIndex(i);
        if (contains(i)) throw std::invalid_argument("index is already in the priority queue");
        n++;
        qp[i] = n;
        pq[n] = i;
        keys[i] = key;
        swim(n);
    }

    // Return an index associated with a maximum key
    int maxIndex() const {
        if (n == 0) throw std::out_of_range("Priority queue underflow");
        return pq[1];
    }

    // Return a maximum key
    Key maxKey() const {
        if (n == 0) throw std::out_of_range("Priority queue underflow");
        return keys[pq[1]];
    }

    // Remove a maximum key and return its associated index
    int delMax() {
        if (n == 0) throw std::out_of_range("Priority queue underflow");
        int max = pq[1];
        std::swap(pq[1], pq[n--]);
        qp[pq[1]] = 1;
        qp[pq[n + 1]] = n + 1;
        sink(1);
        assert(pq[n + 1] == max);
        qp[max] = -1;        // delete
        keys[max] = Key();   // to help with garbage collection
        pq[n + 1] = -1;      // not needed
        return max;
    }

    // Return the key associated with index i
    Key keyOf(int i) const {
        validateIndex(i);
        if (!contains(i)) throw std::out_of_range("index is not in the priority queue");
        return keys[i];
    }

    // Change the key associated with index i to the specified value
    void changeKey(int i, Key key) {
        validateIndex(i);
        if (!contains(i)) throw std::out_of_range("index is not in the priority queue");
        keys[i] = key;
        swim(qp[i]);
        sink(qp[i]);
    }

    // Increase the key associated with index i to the specified value
    void increaseKey(int i, Key key) {
        validateIndex(i);
        if (!contains(i)) throw std::out_of_range("index is not in the priority queue");
        if (keys[i] >= key) throw std::invalid_argument("Calling increaseKey() with a key less than or equal to the key in the priority queue");
        keys[i] = key;
        swim(qp[i]);
    }

    // Decrease the key associated with index i to the specified value
    void decreaseKey(int i, Key key) {
        validateIndex(i);
        if (!contains(i)) throw std::out_of_range("index is not in the priority queue");
        if (keys[i] <= key) throw std::invalid_argument("Calling decreaseKey() with a key greater than or equal to the key in the priority queue");
        keys[i] = key;
        sink(qp[i]);
    }

    // Remove the key on the priority queue associated with index i
    void deleteKey(int i) {
        validateIndex(i);
        if (!contains(i)) throw std::out_of_range("index is not in the priority queue");
        int index = qp[i];
        std::swap(pq[index], pq[n--]);
        qp[pq[index]] = index;
        qp[pq[n + 1]] = n + 1;
        swim(index);
        sink(index);
        keys[i] = Key();
        qp[i] = -1;
    }

    // Iterator class to iterate over the keys in the priority queue in descending order
    class Iterator {
    private:
        IndexMaxPQ copy;
    public:
        Iterator(const IndexMaxPQ& pq) : copy(pq.maxN) {
            for (int i = 1; i <= pq.n; i++) {
                copy.insert(pq.pq[i], pq.keys[pq.pq[i]]);
            }
        }

        bool hasNext() const { return !copy.isEmpty(); }
        int next() { return copy.delMax(); }
    };

    Iterator begin() const { return Iterator(*this); }
    Iterator end() const { return Iterator(IndexMaxPQ(0)); }
};
