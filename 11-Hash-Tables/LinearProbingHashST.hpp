#pragma once

#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

template <typename Key, typename Value>
class LinearProbingHashST {
private:
    static const int INIT_CAPACITY = 4;

    int n;                    // number of key-value pairs in the symbol table
    int m;                    // size of linear probing table
    std::vector<Key> keys;    // the keys
    std::vector<Value> vals;  // the values

    int hash(const Key& key) const {
        std::hash<Key> hash_fn;
        return hash_fn(key) % m;
    }

    void resize(int capacity) {
        LinearProbingHashST<Key, Value> temp(capacity);
        for (int i = 0; i < m; i++) {
            if (keys[i] != Key()) {
                temp.put(keys[i], vals[i]);
            }
        }
        keys = std::move(temp.keys);
        vals = std::move(temp.vals);
        m = temp.m;
    }

    bool check() const {
        if (m < 2 * n) {
            std::cerr << "Hash table size m = " << m << "; array size n = " << n << std::endl;
            return false;
        }

        // check that each key in table can be found by get()
        for (int i = 0; i < m; i++) {
            if (keys[i] == Key())
                continue;
            else if (get(keys[i]) != vals[i]) {
                std::cerr << "get[" << keys[i] << "] = " << get(keys[i]) << "; vals[i] = " << vals[i] << std::endl;
                return false;
            }
        }
        return true;
    }

public:
    LinearProbingHashST() : LinearProbingHashST(INIT_CAPACITY) {}

    LinearProbingHashST(int capacity) : n(0), m(capacity), keys(capacity, Key{}), vals(capacity) {}

    int size() const {
        return n;
    }

    bool isEmpty() const {
        return size() == 0;
    }

    bool contains(const Key& key) const {
        if (key == Key())
            throw std::invalid_argument("argument to contains() is null");
        return get(key) != Value();
    }

    Value get(const Key& key) const {
        if (key == Key())
            throw std::invalid_argument("argument to get() is null");
        for (int i = hash(key); keys[i] != Key(); i = (i + 1) % m)
            if (keys[i] == key)
                return vals[i];
        return Value();
    }

    void put(const Key& key, const Value& val) {
        if (key == Key())
            throw std::invalid_argument("first argument to put() is null");

        if (val == Value()) {
            deleteKey(key);
            return;
        }

        if (n >= m / 2)
            resize(2 * m);

        int i;
        for (i = hash(key); keys[i] != Key(); i = (i + 1) % m) {
            if (keys[i] == key) {
                vals[i] = val;
                return;
            }
        }
        keys[i] = key;
        vals[i] = val;
        n++;
    }

    void deleteKey(const Key& key) {
        if (key == Key())
            throw std::invalid_argument("argument to delete() is null");
        if (!contains(key))
            return;

        // find position i of key
        int i = hash(key);
        while (!(key == keys[i])) {
            i = (i + 1) % m;
        }

        // delete key and associated value
        keys[i] = Key();
        vals[i] = Value();

        // rehash all keys in same cluster
        i = (i + 1) % m;
        while (keys[i] != Key()) {
            // delete keys[i] and vals[i] and reinsert
            Key keyToRehash = keys[i];
            Value valToRehash = vals[i];
            keys[i] = Key();
            vals[i] = Value();
            n--;
            put(keyToRehash, valToRehash);
            i = (i + 1) % m;
        }

        n--;

        if (n > 0 && n <= m / 8)
            resize(m / 2);

        // assert(check());
    }

    std::vector<Key> getKeys() const {
        std::vector<Key> queue;
        for (int i = 0; i < m; i++)
            if (keys[i] != Key())
                queue.push_back(keys[i]);
        return queue;
    }
};


// Adding comment here for no reason
