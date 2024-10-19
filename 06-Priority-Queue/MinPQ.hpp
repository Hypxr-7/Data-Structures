#include <initializer_list>
#include <stdexcept>
#include <algorithm>

template <typename Item>
class MinPQ {
private:
    Item* pq;  // store items at indices 1 to n
    int n;     // number of items on priority queue
    int cap;   // capacity of the priority queue

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
        while (k > 1 && pq[k / 2] > pq[k]) {
            std::swap(pq[k / 2], pq[k]);
            k = k / 2;
        }
    }

    void sink(int k) {
        while (2 * k <= n) {
            int j = 2 * k;
            if (j < n && pq[j] > pq[j + 1]) ++j;
            if (pq[k] <= pq[j]) break;
            std::swap(pq[k], pq[j]);
            k = j;
        }
    }

public:
    MinPQ(int capacity = 8) : pq(new Item[capacity + 1]), n(0), cap(capacity) {}

    MinPQ(std::initializer_list<Item> keys) {
        n = keys.size();
        cap = n;
        pq = new Item[n + 1];
        int i = 1;
        for (const Item& key : keys) {
            pq[i++] = key;
        }
        for (int k = n / 2; k >= 1; --k) {
            sink(k);
        }
    }

    ~MinPQ() {
        delete[] pq;
    }

    bool isEmpty() const {
        return n == 0;
    }

    int size() const {
        return n;
    }

    Item min() const {
        if (isEmpty()) throw std::runtime_error("Priority queue underflow");
        return pq[1];
    }

    void insert(const Item& x) {
        if (n == cap) resize(2 * cap);
        pq[++n] = x;
        swim(n);
    }

    Item delMin() {
        if (isEmpty()) throw std::runtime_error("Priority queue underflow");
        Item min = pq[1];
        std::swap(pq[1], pq[n--]);
        sink(1);
        pq[n + 1] = Item();  // to avoid loitering 
        if ((n > 0) && (n == (cap - 1) / 4)) resize(cap / 2);
        return min;
    }
};