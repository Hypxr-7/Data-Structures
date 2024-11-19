#pragma once

#include <functional>
#include <list>
#include <stdexcept>
#include <vector>

template <typename Key, typename Value>
class SeparateChainingHashST {
private:
    static const int INIT_CAPACITY = 4;

    struct Node {
        Key key;
        Value value;
        Node(Key k, Value v) : key(k), value(v) {}
    };

    int n;                            // number of key-value pairs
    int m;                            // hash table size
    std::vector<std::list<Node>> st;  // array of linked-list symbol tables

    void resize(int chains) {
        SeparateChainingHashST<Key, Value> temp(chains);
        for (int i = 0; i < m; i++) {
            for (const auto& node : st[i]) {
                temp.put(node.key, node.value);
            }
        }
        this->m = temp.m;
        this->n = temp.n;
        this->st = std::move(temp.st);
    }

    // hash function for keys - returns value between 0 and m-1
    int hash(const Key& key) const {
        std::hash<Key> hash_fn;
        return hash_fn(key) % m;
    }

public:
    SeparateChainingHashST() : SeparateChainingHashST(INIT_CAPACITY) {}

    SeparateChainingHashST(int m) : n(0), m(m), st(m) {}

    int size() const { return n; }

    bool isEmpty() const { return size() == 0; }

    bool contains(const Key& key) const {
        try {
            get(key);
        } catch (const std::out_of_range& e) {
            return false;
        }
        return true;
    }

    Value get(const Key& key) const {
        int i = hash(key);
        for (const auto& node : st[i]) {
            if (node.key == key)
                return node.value;
        }
        throw std::out_of_range("Key not in table");
    }

    void put(const Key& key, const Value& val) {
        // double table size if average length of list >= 10
        if (n >= 10 * m)
            resize(2 * m);

        int i = hash(key);
        for (auto& node : st[i]) {
            if (node.key == key) {
                node.value = val;
                return;
            }
        }
        st[i].emplace_back(key, val);
        n++;
    }

    void deleteKey(const Key& key) {
        int i = hash(key);
        auto& chain = st[i];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->key == key) {
                chain.erase(it);
                n--;
                break;
            }
        }

        // halve table size if average length of list <= 2
        if (m > INIT_CAPACITY && n <= 2 * m)
            resize(m / 2);
    }

    std::vector<Key> keys() const {
        std::vector<Key> queue;
        for (int i = 0; i < m; i++) {
            for (const auto& node : st[i]) {
                queue.push_back(node.key);
            }
        }
        return std::move(queue);
    }
};