#pragma once

#include <stdexcept>
#include <vector>

template <typename Key, typename Value>
class SequentialSearchST {
    struct Node {
        Key key;        // The key part of the key-value pair
        Value value;    // The value associated with the key
        Node* next;     // Pointer to the next node in the linked list
    };

    Node* first;    // Pointer to the first node in the linked list
    int count;      // Number of key-value pairs currently stored

public:
    SequentialSearchST() : first(nullptr), count(0) {}

    ~SequentialSearchST() {
        while (first != nullptr) {
            const Node* temp = first;
            first = first->next;
            delete temp;
        }
    }

    bool empty() const { return count == 0; }

    int size() const { return count; }

    bool contains(const Key& key) const {
        Node* temp = first;
        while (temp != nullptr) {
            if (temp->key == key) return true;
            temp = temp->next;
        }
        return false;
    }

    Value get(const Key& key) {
        Node* temp = first;
        while (temp != nullptr) {
            if (temp->key == key) return temp->value;
            temp = temp->next;
        }
        throw std::invalid_argument("Key not in ST");
    }

    void put(const Key& key, const Value& value) {
        Node* temp = first;
        while (temp != nullptr) {
            if (temp->key == key) {
                temp->value = value;  // Update existing key
                return;
            }
            temp = temp->next;
        }
        // Key not found, insert new node at front
        first = new Node{key, value, first};
        ++count;
    }

    void remove(const Key& key) {
        if (!contains(key)) throw std::invalid_argument("Key not in ST");
        first = remove(first, key);
    }

private:
    Node* remove(Node* node, const Key& key) {
        if (node->key == key) {
            --count;
            Node* temp = node->next;
            delete node;
            return temp;
        }
        node->next = remove(node->next, key);
        return node;
    }

public:
    std::vector<Key> keys() const {
        std::vector<Key> st;
        st.reserve(count);
        for (Node* node = first; node != nullptr; node = node->next) {
            st.push_back(node->key);
        }
        return std::move(st);
    }
};
