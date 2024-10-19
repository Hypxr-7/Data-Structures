#pragma once

#include <vector>
#include <queue>

template <typename Key, typename Value>
class BinarySearchTree {
private:
    struct Node {
        Key key;
        Value value;
        Node* left = nullptr;
        Node* right = nullptr;
        int size;

        Node(const Key& _key, const Value& _value, const int _size) : 
            key(_key), 
            value(_value), 
            size(_size) {}
    };

    Node* m_root = nullptr;

public:
    BinarySearchTree() = default;

    // ~BinarySearchTree();

    bool isEmpty() const { return size() == 0; }

    bool contains(const Key& key) const { return get(key) != Value{}; }

    int size() const { return size(m_root); }

    int size(const Key& lo, const Key& hi) const {
        if (lo > hi) return 0;
        if (contains(hi)) return rank(hi) - rank(lo) + 1;
        else return rank(hi) - rank(lo);
    }

private:
    int size(const Node* node) const {return node != nullptr ? node->size : 0; }

public:
    Value get(const Key& key) const { return get(m_root, key); }

private:
    Value get(const Node* node, const Key& key) const {
        if (node == nullptr) return Value{};
        if (key < node->key) return get(node->left, key);
        else if (key > node->key) return get(node->right, key);
        else return node->value;
    }

public:
    void put(const Key& key, const Value& value) {
        if (value == Value{}) {
            deleteKey(key);
            return;
        }
        m_root = put(m_root, key, value);
    }

private:
    Node* put(Node* node, const Key& key, const Value& value) {
        if (node == nullptr) return new Node(key, value, 1);
        if (key < node->key) node->left = put(node->left, key, value);
        else if (key > node->key) node->right = put(node->right, key, value);
        else node->value = value;

        node->size = 1 + size(node->left) + size(node->right);
        return node; 
    }

public:
    void deleteMin() {
        if (isEmpty()) throw std::runtime_error("Symbol table underflow");
        m_root = deleteMin(m_root);
    }

private:
    Node* deleteMin(Node* node) {
        if (node->left == nullptr) return node->right;
        node->left = deleteMin(node->left);
        node->size = size(node->left) + size(node->right) + 1;
        return node;
    }

public:
    void deleteMax() {
        if (isEmpty()) throw std::runtime_error("Symbol table underflow");
        m_root = deleteMax(m_root);
    }

private:
    Node* deleteMax(Node* node) {
        if (node->right == nullptr) return node->left;
        node->right = deleteMax(node->right);
        node->size = size(node->left) + size(node->right) + 1;
        return node;
    }

public:
    void deleteKey(const Key& key) {
        m_root = deleteKey(m_root, key);
    }

private:
    Node* deleteKey(Node* node,const  Key& key) {
        if (node == nullptr) return nullptr;
        if (key < node->key) node->left = deleteKey(node->left, key);
        else if (key > node->key) node->right = deleteKey(node->right, key);
        else {
            if (node->right == nullptr) return node->left;
            if (node->left == nullptr) return node->right;
            Node* temp = node;
            node = min(temp->right);
            node->right = deleteMin(temp->right);
            node->left = temp->left;
        }
        node->size = size(node->left) + size(node->right) + 1;
        return node;
    }

public:
    Key min() const {
        if (isEmpty()) throw std::runtime_error("calls min() with empty symbol table");
        return min(m_root)->key;
    }

private:
    Node* min(Node* node) const {
        return node->left != nullptr ? min(node->left) : node;
    }

public:
    Key max() const {
        if (isEmpty()) throw std::runtime_error("calls max() with empty symbol table");
        return max(m_root)->key;
    }

private:
    Node* max(Node* node) const {
        return node->right != nullptr ? max(node->right) : node;
    }

public:
    Key floor(const Key& key) const {
        Node* node = floor(m_root, key);
        if (node == nullptr) throw std::runtime_error("argument to floor() is too small");
        return node->key;
    }

private:
    Node* floor(Node* node, const Key& key) const {
        if (node == nullptr) return nullptr;
        if (key == node->key) return node;
        if (key < node->key) return floor(node->left, key);
        Node* temp = floor(node->right, key);
        return temp != nullptr ? temp : node; 
    }

public:
    Key ceiling(const Key& key) const {
        Node* node = ceiling(m_root, key);
        if (node == nullptr) throw std::runtime_error("argument to ceiling() is too large");
        return node->key;
    }

private:
    Node* ceiling(Node* node, const Key& key) const {
        if (node == nullptr) return nullptr;
        if (key == node->key) return node;
        if (key > node->key) return ceiling(node->right, key);
        Node* temp = ceiling(node->left, key);
        return temp != nullptr ? temp : node; 
    }

public:
    Key select(const int rank) const {
        if (rank < 0 || rank >= size()) throw std::invalid_argument("argument to select() is invalid");
        return select(m_root, rank)->key;
    }

private:
    Node* select(Node* node, const int rank) const {
        if (node == nullptr) return nullptr;
        int leftSize = size(node->left);
        if (leftSize > rank) return select(node->left, rank);
        else if (leftSize < rank) return select(node->right, rank - leftSize - 1);
        else return node;
    }

public:
    int rank(const Key& key) const {
        return rank(m_root, key);
    }

private:
    int rank(Node* node, const Key& key) const {
        if (node == nullptr) return 0;
        if (key < node->key) return rank(node->left, key);
        else if (key > node->key) return 1 + size(node->left) + rank(node->right, key);
        else return size(node->left);
    }

public:
    std::vector<Key> keys() const {
        if (isEmpty()) return std::vector<Key>{};
        return std::move(keys(min(), max()));
    }

    std::vector<Key> keys(const Key& lo, const Key& hi) const {
        std::vector<Key> list;
        keys(m_root, list, lo, hi);
        return std::move(list);
    }

private:
    void keys(Node* node, std::vector<Key>& list, const Key& lo, const Key& hi) const {
        if (node == nullptr) return;
        if (lo < node->key) keys(node->left, list, lo, hi);
        if (lo <= node->key && hi >= node->key) list.push_back(node->key);
        if (hi > node->key) keys(node->right, list, lo, hi);
    }

public:
    int height() const {
        return height(m_root);
    }

private:
    int height(Node* node) const {
        if (node == nullptr) return -1;
        return 1 + std::max(height(node->left), height(node->right));
    }

public:
    std::vector<Key> levelOrder() const {
        if (m_root == nullptr) return std::vector<Key>{};
        std::vector<Key> keys;
        std::queue<Node*> q;
        q.push(m_root);
        while (!q.empty()) {
            Node* node = q.front();
            q.pop();
            if (node != nullptr) {
                keys.push_back(node->key);
                q.push(node->left);
                q.push(node->right);
            }
        }
        return keys;
    }

private:
    bool isBST() { return isBST(m_root, Key{}, Key{}); }

    bool isBST(Node* node, const Key& min, const Key& max) {
        if (node == nullptr) return true;
        if (min != Key{} && node->key <= min) return false;
        if (max != Key{} && node->key >= max) return false;
        return isBST(node->left, min, node->key) && isBST(node->right, node->key, max); 
    }
};