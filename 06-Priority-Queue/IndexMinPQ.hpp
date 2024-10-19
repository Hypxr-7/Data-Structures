#include <algorithm> 
#include <cassert>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

template <typename Key>
class IndexMinPQ {
private:
    int maxN;        // maximum number of elements on PQ
    int n;           // number of elements on PQ
    std::vector<int> pq;        // binary heap using 1-based indexing
    std::vector<int> qp;        // inverse of pq - qp[pq[i]] = pq[qp[i]] = i
    std::vector<Key> keys;      // keys[i] = priority of i

    // check if the given index exists
    void validateIndex(int i) const {
        if (i < 0) throw std::invalid_argument("index is negative: " + std::to_string(i));
        if (i >= maxN) throw std::invalid_argument("index >= capacity: " + std::to_string(i));
    }

    void swim(int k) {
        while (k > 1 && keys[pq[k / 2]] > keys[pq[k]]) {
            std::swap(pq[k], pq[k / 2]);
            qp[pq[k]] = k;
            qp[pq[k / 2]] = k / 2;
            k = k / 2;
        }
    }

    void sink(int k) {
        while (2 * k <= n) {
            int j = 2 * k;
            if (j < n && keys[pq[j]] > keys[pq[j + 1]]) j++;
            if (!(keys[pq[k]] > keys[pq[j]])) break;
            std::swap(pq[k], pq[j]);
            qp[pq[k]] = k;
            qp[pq[j]] = j;
            k = j;
        }
    }

public:
    IndexMinPQ(int maxN) : maxN(maxN), n(0), pq(maxN + 1), qp(maxN + 1, -1), keys(maxN + 1) {
        if (maxN < 0) throw std::invalid_argument("maxN < 0");
    }

    bool isEmpty() const {
        return n == 0;
    }

    bool contains(int i) const {
        validateIndex(i);
        return qp[i] != -1;
    }

    int size() const {
        return n;
    }

    void insert(int i, Key key) {
        validateIndex(i);
        if (contains(i)) throw std::invalid_argument("index is already in the priority queue");
        n++;
        qp[i] = n;
        pq[n] = i;
        keys[i] = key;
        swim(n);
    }

    int minIndex() const {
        if (n == 0) throw std::out_of_range("Priority queue underflow");
        return pq[1];
    }

    Key minKey() const {
        if (n == 0) throw std::out_of_range("Priority queue underflow");
        return keys[pq[1]];
    }

    int delMin() {
        if (n == 0) throw std::out_of_range("Priority queue underflow");
        int min = pq[1];
        std::swap(pq[1], pq[n--]);
        qp[pq[1]] = 1;
        qp[pq[n + 1]] = n + 1;
        sink(1);
        assert(min == pq[n + 1]);
        qp[min] = -1;        // delete
        keys[min] = Key();   
        pq[n + 1] = -1;      // not needed
        return min;
    }

    Key keyOf(int i) const {
        validateIndex(i);
        if (!contains(i)) throw std::out_of_range("index is not in the priority queue");
        return keys[i];
    }

    void changeKey(int i, Key key) {
        validateIndex(i);
        if (!contains(i)) throw std::out_of_range("index is not in the priority queue");
        keys[i] = key;
        swim(qp[i]);
        sink(qp[i]);
    }

    void decreaseKey(int i, Key key) {
        validateIndex(i);
        if (!contains(i)) throw std::out_of_range("index is not in the priority queue");
        if (keys[i] <= key) throw std::invalid_argument("Calling decreaseKey() with a key greater than or equal to the key in the priority queue");
        keys[i] = key;
        swim(qp[i]);
    }

    void increaseKey(int i, Key key) {
        validateIndex(i);
        if (!contains(i)) throw std::out_of_range("index is not in the priority queue");
        if (keys[i] >= key) throw std::invalid_argument("Calling increaseKey() with a key less than or equal to the key in the priority queue");
        keys[i] = key;
        sink(qp[i]);
    }

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

    class Iterator {
    private:
        IndexMinPQ copy;
    public:
        Iterator(const IndexMinPQ& pq) : copy(pq.maxN) {
            for (int i = 1; i <= pq.n; i++) {
                copy.insert(pq.pq[i], pq.keys[pq.pq[i]]);
            }
        }

        bool hasNext() const { return !copy.isEmpty(); }
        int next() { return copy.delMin(); }
    };

    Iterator begin() const { return Iterator(*this); }
    Iterator end() const { return Iterator(IndexMinPQ(0)); }
};
