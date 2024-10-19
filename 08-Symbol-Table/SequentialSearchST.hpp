#pragma once

#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <queue>

template <typename Key, typename Value>
class SequentialSearchST {
private:
    struct Node {
        Key key;
        Value value;
        std::unique_ptr<Node> next;

        Node(Key k, Value v, std::unique_ptr<Node> n) : key(k), value(v), next(std::move(n)) {}
    };

    int m_size;
    std::unique_ptr<Node> m_first;

    Node* deleteNode(Node* node, const Key& key) {
        if (node == nullptr) return nullptr;
        if (key == node->key) {
            m_size--;
            return node->next.release();
        }
        node->next.reset(deleteNode(node->next.release(), key));
        return node;
    }

public:
    SequentialSearchST() : m_size(0), m_first(nullptr) {}

    ~SequentialSearchST() {
        while (m_first) {
            m_first = std::move(m_first->next);
        }
    }

    int size() const {
        return m_size;
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
        for (Node* x = m_first.get(); x != nullptr; x = x->next.get()) {
            if (key == x->key)
                return &x->value;
        }
        return nullptr;
    }

    void put(const Key& key, const Value& val) {
        if (key == Key()) throw std::invalid_argument("first argument to put() is null");
        if (val == Value()) {
            deleteKey(key);
            return;
        }

        for (Node* x = m_first.get(); x != nullptr; x = x->next.get()) {
            if (key == x->key) {
                x->value = val;
                return;
            }
        }
        m_first = std::make_unique<Node>(key, val, std::move(m_first));
        m_size++;
    }

    void deleteKey(const Key& key) {
        if (key == Key()) throw std::invalid_argument("argument to delete() is null");
        m_first.reset(deleteNode(m_first.release(), key));
    }

    std::queue<Key> keys() const {
        std::queue<Key> queue;
        for (Node* x = m_first.get(); x != nullptr; x = x->next.get())
            queue.push(x->key);
        return queue;
    }
};
