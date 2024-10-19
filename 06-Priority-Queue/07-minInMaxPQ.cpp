#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iostream>
#include <stdexcept>

template <typename Item>
class MaxPQ {
private:
    Item* pq;  // store items at indices 1 to n
    int n;     // number of items on priority queue
    int cap;   // capacity of the priority queue
    Item minItem; // store the minimum item

    void resize(int capacity) {
        cap = capacity;
        Item* temp = new Item[capacity + 1];
        for (int i = 1; i <= n; ++i) {
            temp[i] = pq[i];
        }
        delete[] pq;
        pq = temp;
    }

    void swim(int k) {
        while (k > 1 && pq[k / 2] < pq[k]) {
            std::swap(pq[k / 2], pq[k]);
            k = k / 2;
        }
    }

    void sink(int k) {
        while (2 * k <= n) {
            int j = 2 * k;
            if (j < n && pq[j] < pq[j + 1]) ++j;
            if (pq[k] >= pq[j]) break;
            std::swap(pq[k], pq[j]);
            k = j;
        }
    }

public:
    MaxPQ(int capacity = 8) : pq(new Item[capacity + 1]), n(0), cap(capacity), minItem(Item()) {}

    MaxPQ(std::initializer_list<Item> keys) {
        n = keys.size();
        cap = n;
        pq = new Item[n + 1];
        int i = 1;
        for (const Item& key : keys) {
            pq[i++] = key;
            if (i == 2 || key < minItem) {
                minItem = key;
            }
        }
        for (int k = n / 2; k >= 1; --k) {
            sink(k);
        }
    }

    ~MaxPQ() {
        delete[] pq;
    }

    bool isEmpty() const {
        return n == 0;
    }

    int size() const {
        return n;
    }

    Item max() const {
        if (isEmpty()) throw std::runtime_error("Priority queue underflow");
        return pq[1];
    }

    Item min() const {
        if (isEmpty()) throw std::runtime_error("Priority queue underflow");
        return minItem;
    }

    void insert(const Item& x) {
        if (n == cap) resize(2 * cap);
        pq[++n] = x;
        swim(n);
        if (n == 1 || x < minItem) {
            minItem = x;
        }
    }

    Item delMax() {
        if (isEmpty()) throw std::runtime_error("Priority queue underflow");
        Item max = pq[1];
        std::swap(pq[1], pq[n--]);
        sink(1);
        pq[n + 1] = Item();  // to avoid loitering 
        if ((n > 0) && (n == (cap - 1) / 4)) resize(cap / 2);
        if (n == 0) {
            minItem = Item(); // reset minItem if the priority queue is empty
        }
        return max;
    }
};

void testMinInEmptyPQ() {
    MaxPQ<int> pq;
    try {
        pq.min();
        assert(false); // Should not reach here
    } catch (const std::runtime_error& e) {
        assert(std::string(e.what()) == "Priority queue underflow");
    }
}

void testMinInSingleElementPQ() {
    MaxPQ<int> pq;
    pq.insert(42);
    assert(pq.min() == 42);
}

void testMinInMultipleElementsPQ() {
    MaxPQ<int> pq;
    pq.insert(42);
    pq.insert(15);
    pq.insert(23);
    pq.insert(8);
    pq.insert(16);
    assert(pq.min() == 8);
}

void testMinAfterDelMax() {
    MaxPQ<int> pq;
    pq.insert(42);
    pq.insert(15);
    pq.insert(23);
    pq.insert(8);
    pq.insert(16);
    pq.delMax();
    assert(pq.min() == 8);
}

void testMinWithInitializerList() {
    MaxPQ<int> pq = {42, 15, 23, 8, 16};
    assert(pq.min() == 8);
}

int main() {
    testMinInEmptyPQ();
    testMinInSingleElementPQ();
    testMinInMultipleElementsPQ();
    testMinAfterDelMax();
    testMinWithInitializerList();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}