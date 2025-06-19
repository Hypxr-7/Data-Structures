#pragma once

#include <cassert>
#include <stdexcept>
#include <vector>

template <typename Key, typename Value>
class BinarySearchST {
    std::vector<Key> keys_;      // Sorted array of keys_
    std::vector<Value> values;  // Parallel array of values corresponding to keys_
    int count;                  // Number of key-value pairs currently stored

    void resize(const int newCap) {
        keys_.resize(newCap);
        values.resize(newCap);
    }

public:
    BinarySearchST() : keys_(8), values(8), count(0) {}

    bool empty() const { return count == 0; }

    int size() const { return count; }

    bool contains(const Key& key) const {
        int i = rank(key);
        return i < count && keys_[i] == key;
    }

    Value get(const Key& key) const {
        if (empty()) throw std::underflow_error("ST is empty");
        int i = rank(key);
        if (i < count && keys_[i] == key) return values[i];
        throw std::invalid_argument("Key not in ST");
    }

    void put(const Key& key, const Value& value) {
        int i = rank(key);

        // Update existing key
        if (i < count && keys_[i] == key) {
            values[i] = value;
            return;
        }

        // Resize if necessary (double the capacity)
        if (keys_.size() == count) resize(2 * count);

        // Shift elements to make room for new key
        for (int j = count; j > i; --j) {
            keys_[j] = keys_[j-1];
            values[j] = values[j-1];
        }

        // Insert new key-value pair
        keys_[i] = key;
        values[i] = value;
        ++count;

        assert(check());
    }

    void remove(const Key& key) {
        if (empty()) throw std::underflow_error("ST is empty");

        int i = rank(key);

        // Key not found - do nothing
        if (i == count || keys_[i] != key) return;

        // Shift elements left to fill the gap
        for (int j = i; j < count - 1; ++j) {
            keys_[j] = keys_[j + 1];
            values[j] = values[j + 1];
        }
        --count;

        // Resize down if usage is too low (avoid memory waste)
        if (count > 0 && count == keys_.size() / 4) resize(keys_.size() / 2);

        assert(check());
    }

    void removeMin() {
        if (empty()) throw std::underflow_error("ST is empty");
        remove(min());
    }

    void removeMax() {
        if (empty()) throw std::underflow_error("ST is empty");
        remove(max());
    }

    Key min() const {
        if (empty()) throw std::underflow_error("ST is empty");
        return keys_[0];
    }

    Key max() const {
        if (empty()) throw std::underflow_error("ST is empty");
        return keys_[count - 1];
    }

    Key select(const int k) const {
        if (k < 0 || k >= size()) throw std::invalid_argument("k is out of range");
        return keys_[k];
    }

    int rank(const Key& key) const {
        int lo = 0;
        int hi = count - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if      (key < keys_[mid]) hi = mid - 1;
            else if (key > keys_[mid]) lo = mid + 1;
            else return mid;
        }
        return lo;
    }

    Key floor(const Key& key) const {
        if (empty()) throw std::underflow_error("ST is empty");
        int i = rank(key);
        if (i < count && keys_[i] == key) return keys_[i];
        if (i == 0) throw std::invalid_argument("key is too small");
        return keys_[i - 1];
    }

    Key ceiling(const Key& key) const {
        if (empty()) throw std::underflow_error("ST is empty");
        int i = rank(key);
        if (i == count) throw std::invalid_argument("key is too large");
        return keys_[i];
    }

    int size(const Key& lo, const Key& hi) const {
        if (empty()) return 0;
        if (lo > hi) return 0;
        if (contains(hi)) return rank(hi) - rank(lo) + 1;
        return rank(hi) - rank(lo);
    }

    std::vector<Key> keys() const {
        if (empty()) return std::vector<Key>{};
        return keys(min(), max());
    }

    std::vector<Key> keys(const Key& lo, const Key& hi) const {
        std::vector<Key> returnKeys;
        if (empty() || lo > hi) return returnKeys;

        const int n = rank(hi);
        for (int i = rank(lo); i < n; ++i)
            returnKeys.push_back(keys_[i]);
        if (contains(hi)) returnKeys.push_back(keys_[n]);
        return returnKeys;
    }

private:
    bool check() const {
        return isSorted() && rankCheck();
    }

    bool isSorted() const {
        for (int i = 1; i < count; ++i)
            if (keys_[i] < keys_[i - 1]) return false;
        return true;
    }

    bool rankCheck() const {
        for (int i = 0; i < count; ++i) {
            if (i != rank(select(i))) return false;
        }
        for (int i = 0; i < count; ++i) {
            if (keys_[i] != select(rank(keys_[i]))) return false;
        }
        return true;
    }
};
