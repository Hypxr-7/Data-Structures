#include <vector>
#include <stdexcept>
#include <queue>

template <typename Key, typename Value>
class ArrayST {
private:
    static const int INIT_SIZE = 8;

    std::vector<Key> m_keys;
    std::vector<Value> vals;
    int n = 0; // number of elements in symbol table

    // resize the parallel arrays to the given capacity
    void resize(int capacity) {
        std::vector<Key> tempk(capacity);
        std::vector<Value> tempv(capacity);
        for (int i = 0; i < n; i++) {
            tempk[i] = m_keys[i];
            tempv[i] = vals[i];
        }
        m_keys = std::move(tempk);
        vals = std::move(tempv);
    }

public:
    ArrayST() : m_keys(INIT_SIZE), vals(INIT_SIZE) {}

    // return the number of key-value pairs in the symbol table
    int size() const {
        return n;
    }

    // is the symbol table empty?
    bool isEmpty() const {
        return size() == 0;
    }

    // insert the key-value pair into the symbol table
    void put(const Key& key, const Value& val) {
        // to deal with duplicates
        deleteKey(key);

        // double size of arrays if necessary
        if (n >= vals.size()) resize(2 * n);

        // add new key and value at the end of array
        vals[n] = val;
        m_keys[n] = key;
        n++;
    }

    Value get(const Key& key) const {
        for (int i = 0; i < n; i++) {
            if (m_keys[i] == key) return vals[i];
        }
        throw std::runtime_error("Key not found");
    }

    std::vector<Key> keys() const {
        std::vector<Key> keyList;
        for (int i = 0; i < n; i++) {
            keyList.push_back(m_keys[i]);
        }
        return keyList;
    }

    // remove given key (and associated value)
    void deleteKey(const Key& key) {
        for (int i = 0; i < n; i++) {
            if (key == m_keys[i]) {
                m_keys[i] = m_keys[n - 1];
                vals[i] = vals[n - 1];
                m_keys[n - 1].~Key();
                vals[n - 1].~Value();
                n--;
                if (n > 0 && n == m_keys.size() / 4) resize(m_keys.size() / 2);
                return;
            }
        }
    }
};