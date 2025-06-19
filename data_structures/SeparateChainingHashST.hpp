#pragma once

#include <functional>
#include <list>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename Key, typename Value>
class SeparateChainingHashST {
private:
    // Hash table: vector of linked lists containing key-value pairs
    std::vector<std::list<std::pair<Key, Value>>> st;

    int count;  // Number of key-value pairs in the symbol table
    int cap;    // Current capacity (number of chains) of the hash table

    void resize(const int chains) {
        // Create temporary hash table with new capacity
        SeparateChainingHashST<Key, Value> temp(chains);

        // Rehash all existing key-value pairs into new table
        for (int i = 0; i < cap; ++i) {
            for (const auto& [key, value] : st[i]) {
                temp.put(key, value);
            }
        }

        // Replace current table with resized table
        cap = chains;
        st = std::move(temp.st);
    }

    int hash(const Key& key) const {
        return std::hash<Key>{}(key) % cap;
    }

public:
    explicit SeparateChainingHashST(const int cap = 8) : st(cap), count(0), cap(cap) {}

    bool empty() const { return count == 0; }

    int size() const { return count; }

    bool contains(const Key& key) const {
        int i = hash(key);
        for (const auto& [key_, value] : st[i]) {
            if (key == key_) return true;
        }
        return false;
    }

    Value get(const Key& key) const {
        int i = hash(key);
        for (const auto& [key_, value] : st[i]) {
            if (key == key_) return value;
        }
        throw std::invalid_argument("Key not in ST");
    }

    void put(const Key& key, const Value& value) {
        // Resize if load factor becomes too high
        if (count >= 10 * cap) resize(2 * cap);

        int i = hash(key);

        // Check if key already exists and update value
        for (auto& [key_, value_] : st[i]) {
            if (key_ == key) {
                value_ = value;  // Update existing value
                return;
            }
        }

        // Key doesn't exist, add new pair
        st[i].emplace_back(key, value);
        ++count;
    }

    void remove(const Key& key) {
        if (!contains(key)) throw std::invalid_argument("Key not in ST");

        int i = hash(key);
        auto& chain = st[i];

        // Find and remove the key-value pair
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->first == key) {
                chain.erase(it);
                --count;
                break;
            }
        }

        // Resize down if load factor becomes too low
        if (cap > 8 && count <= 2 * cap) resize(cap / 2);
    }

    std::vector<Key> keys() const {
        std::vector<Key> returnKeys;
        returnKeys.reserve(count);

        for (int i = 0; i < cap; ++i) {
            for (const auto& [key, value] : st[i]) {
                returnKeys.push_back(key);
            }
        }

        return returnKeys;
    }

    double loadFactor() const {
        return cap > 0 ? static_cast<double>(count) / cap : 0.0;
    }

    int capacity() const {
        return cap;
    }
};
