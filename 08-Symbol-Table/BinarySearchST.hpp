#include <iostream>
#include <stdexcept>
#include <vector>
#include <queue>
#include <cassert>

template <typename Key, typename Value>
class BinarySearchST {
private:
    static const int INIT_CAPACITY = 2;
    Key* m_keys;
    Value* m_vals;
    int n;
    int capacity;

    void resize(int new_capacity) {
        assert(new_capacity >= n);
        Key* tempk = new Key[new_capacity];
        Value* tempv = new Value[new_capacity];
        for (int i = 0; i < n; i++) {
            tempk[i] = m_keys[i];
            tempv[i] = m_vals[i];
        }
        delete[] m_keys;
        delete[] m_vals;
        m_keys = tempk;
        m_vals = tempv;
        capacity = new_capacity;
    }

    bool isSorted() const {
        for (int i = 1; i < size(); i++)
            if (m_keys[i] < m_keys[i-1]) return false;
        return true;
    }

    bool rankCheck() const {
        for (int i = 0; i < size(); i++)
            if (i != rank(select(i))) return false;
        for (int i = 0; i < size(); i++)
            if (m_keys[i] != select(rank(m_keys[i]))) return false;
        return true;
    }

    bool check() const {
        return isSorted() && rankCheck();
    }

public:
    BinarySearchST() : BinarySearchST(INIT_CAPACITY) {}

    BinarySearchST(int capacity) : n(0), capacity(capacity) {
        m_keys = new Key[capacity];
        m_vals = new Value[capacity];
    }

    ~BinarySearchST() {
        delete[] m_keys;
        delete[] m_vals;
    }

    int size() const {
        return n;
    }

    bool isEmpty() const {
        return size() == 0;
    }

    bool contains(const Key& key) const {
        if (key == Key()) throw std::invalid_argument("argument to contains() is null");
        return get(key) != nullptr;
    }

    Value* get(const Key& key) const {
        if (key == Key()) throw std::invalid_argument("argument to get() is null");
        if (isEmpty()) return nullptr;
        int i = rank(key);
        if (i < n && m_keys[i] == key) return &m_vals[i];
        return nullptr;
    }

    int rank(const Key& key) const {
        if (key == Key()) throw std::invalid_argument("argument to rank() is null");

        int lo = 0, hi = n - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (key < m_keys[mid]) hi = mid - 1;
            else if (key > m_keys[mid]) lo = mid + 1;
            else return mid;
        }
        return lo;
    }

    void put(const Key& key, const Value& val) {
        if (key == Key()) throw std::invalid_argument("first argument to put() is null");

        if (val == Value()) {
            deleteKey(key);
            return;
        }

        int i = rank(key);

        if (i < n && m_keys[i] == key) {
            m_vals[i] = val;
            return;
        }

        if (n == capacity) resize(2 * capacity);

        for (int j = n; j > i; j--) {
            m_keys[j] = m_keys[j - 1];
            m_vals[j] = m_vals[j - 1];
        }
        m_keys[i] = key;
        m_vals[i] = val;
        n++;

        assert(check());
    }

    void deleteKey(const Key& key) {
        if (key == Key()) throw std::invalid_argument("argument to delete() is null");
        if (isEmpty()) return;

        int i = rank(key);

        if (i == n || m_keys[i] != key) {
            return;
        }

        for (int j = i; j < n - 1; j++) {
            m_keys[j] = m_keys[j + 1];
            m_vals[j] = m_vals[j + 1];
        }

        n--;
        m_keys[n] = Key();
        m_vals[n] = Value();

        if (n > 0 && n == capacity / 4) resize(capacity / 2);

        assert(check());
    }

    void deleteMin() {
        if (isEmpty()) throw std::out_of_range("Symbol table underflow error");
        deleteKey(min());
    }

    void deleteMax() {
        if (isEmpty()) throw std::out_of_range("Symbol table underflow error");
        deleteKey(max());
    }

    Key min() const {
        if (isEmpty()) throw std::out_of_range("called min() with empty symbol table");
        return m_keys[0];
    }

    Key max() const {
        if (isEmpty()) throw std::out_of_range("called max() with empty symbol table");
        return m_keys[n - 1];
    }

    Key select(int k) const {
        if (k < 0 || k >= size()) {
            throw std::invalid_argument("called select() with invalid argument: " + std::to_string(k));
        }
        return m_keys[k];
    }

    Key floor(const Key& key) const {
        if (key == Key()) throw std::invalid_argument("argument to floor() is null");
        int i = rank(key);
        if (i < n && m_keys[i] == key) return m_keys[i];
        if (i == 0) throw std::out_of_range("argument to floor() is too small");
        else return m_keys[i - 1];
    }

    Key ceiling(const Key& key) const {
        if (key == Key()) throw std::invalid_argument("argument to ceiling() is null");
        int i = rank(key);
        if (i == n) throw std::out_of_range("argument to ceiling() is too large");
        else return m_keys[i];
    }

    int size(const Key& lo, const Key& hi) const {
        if (lo == Key()) throw std::invalid_argument("first argument to size() is null");
        if (hi == Key()) throw std::invalid_argument("second argument to size() is null");

        if (lo > hi) return 0;
        if (contains(hi)) return rank(hi) - rank(lo) + 1;
        else return rank(hi) - rank(lo);
    }

    std::queue<Key> keys() const {
        return keys(min(), max());
    }

    std::queue<Key> keys(const Key& lo, const Key& hi) const {
        if (lo == Key()) throw std::invalid_argument("first argument to m_keys() is null");
        if (hi == Key()) throw std::invalid_argument("second argument to m_keys() is null");

        std::queue<Key> queue;
        if (lo > hi) return queue;
        for (int i = rank(lo); i < rank(hi); i++)
            queue.push(m_keys[i]);
        if (contains(hi)) queue.push(m_keys[rank(hi)]);
        return queue;
    }
};
