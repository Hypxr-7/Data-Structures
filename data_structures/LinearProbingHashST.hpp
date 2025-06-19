#pragma once

#include <functional>
#include <optional>
#include <stdexcept>
#include <vector>

template <typename Key, typename Value>
class LinearProbingHashST {
    // Array of optional keys - nullopt represents empty slot
    std::vector<std::optional<Key>> keys_;

    // Array of optional values - nullopt represents empty slot
    std::vector<std::optional<Value>> vals_;

    int count;  // Number of key-value pairs in the symbol table
    int cap;    // Current capacity of the hash table

    int hash(const Key& key) const {
        return std::hash<Key>{}(key) % cap;
    }

    void resize(const int newCap) {
        // Create temporary hash table with new capacity
        LinearProbingHashST<Key, Value> temp(newCap);

        // Rehash all existing key-value pairs into new table
        for (int i = 0; i < cap; ++i) {
            if (keys_[i].has_value()) {
                temp.put(keys_[i].value(), vals_[i].value());
            }
        }

        // Replace current table with resized table
        cap = newCap;
        keys_ = std::move(temp.keys_);
        vals_ = std::move(temp.vals_);
    }

public:
    explicit LinearProbingHashST(const int cap = 8)
        : keys_(cap, std::nullopt), vals_(cap, std::nullopt), count(0), cap(cap) {}

    bool empty() const { return count == 0; }

    int size() const { return count; }

    bool contains(const Key& key) const {
        int i = hash(key);

        // Linear probe until empty slot or key found
        while (keys_[i].has_value()) {
            if (keys_[i].value() == key) return true;
            i = (i + 1) % cap;
        }

        return false;
    }

    Value get(const Key& key) const {
        int i = hash(key);

        // Linear probe until empty slot or key found
        while (keys_[i].has_value()) {
            if (keys_[i].value() == key) return vals_[i].value();
            i = (i + 1) % cap;
        }

        throw std::invalid_argument("Key not in ST");
    }

    void put(const Key& key, const Value& value) {
        // Resize if load factor becomes too high (≥ 50%)
        if (count >= cap / 2) resize(2 * cap);

        int i = hash(key);

        // Linear probe to find key or empty slot
        while (keys_[i].has_value()) {
            if (keys_[i].value() == key) break;  // Key found, will update
            i = (i + 1) % cap;
        }

        // If slot was empty, increment count
        if (!keys_[i].has_value()) ++count;

        // Store key-value pair
        keys_[i] = key;
        vals_[i] = value;
    }

    void remove(const Key& key) {
        if (!contains(key)) throw std::invalid_argument("Key not in ST");

        // Find position of key using linear probing
        int i = hash(key);
        while (keys_[i].value() != key) {
            i = (i + 1) % cap;
        }

        // Delete key and associated value
        keys_[i] = std::nullopt;
        vals_[i] = std::nullopt;

        // Rehash all keys in same cluster to maintain probe sequence integrity
        i = (i + 1) % cap;
        while (keys_[i].has_value()) {
            // Extract key-value pair to rehash
            Key keyToRehash = keys_[i].value();
            Value valToRehash = vals_[i].value();

            // Remove from current position
            keys_[i] = std::nullopt;
            vals_[i] = std::nullopt;
            --count;

            // Reinsert (put will increment count again)
            put(keyToRehash, valToRehash);

            i = (i + 1) % cap;
        }

        --count;

        // Resize down if load factor becomes too low (≤ 12.5%) and capacity > 8
        if (cap > 8 && count <= cap / 8) resize(cap / 2);
    }

    std::vector<Key> keys() const {
        std::vector<Key> returnKeys;
        returnKeys.reserve(count);

        for (int i = 0; i < cap; ++i) {
            if (keys_[i].has_value()) {
                returnKeys.push_back(keys_[i].value());
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
