#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
#include <queue>
#include <stdexcept>
#include <vector>

template <typename Key, typename Value>
class BST {
    struct Node {
        Key key;        // The key stored in this node
        Value value;    // The value associated with the key
        Node* left;     // Pointer to left child (smaller keys)
        Node* right;    // Pointer to right child (larger keys)
        int size;       // Number of nodes in subtree rooted at this node

        Node(const Key& key, const Value& value, const int size) :
            key(key),
            value(value),
            left(nullptr),
            right(nullptr),
            size(size) {}
    };

    Node* root;  // Pointer to the root node of the BST

public:
    BST() : root(nullptr) {}

    ~BST() { clear(root); }

private:
    void clear(Node* node) {
        if (node != nullptr) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

public:
    bool empty() const { return size() == 0; }

    int size() const { return size(root); }

    int size(const Key& lo, const Key& hi) const {
        if (lo > hi) return 0;
        return contains(hi) ? rank(hi) - rank(lo) + 1 : rank(hi) - rank(lo);
    }

private:
    int size(const Node* node) const { return node ? node->size : 0; }

public:
    bool contains(const Key& key) const {
        Node* node = root;
        while (node != nullptr) {
            if (key < node->key) node = node->left;
            else if (key > node->key) node = node->right;
            else return true;
        }
        return false;
    }

    Value get(const Key& key) const {
        Node* node = root;
        while (node != nullptr) {
            if (key < node->key) node = node->left;
            else if (key > node->key) node = node->right;
            else return node->value;
        }
        throw std::invalid_argument("Key not in BST");
    }

    void put(const Key& key, const Value& value) {
        root = put(root, key, value);
        // assert(check());
    }

private:
    Node* put(Node* node, const Key& key, const Value& value) {
        if (node == nullptr) return new Node(key, value, 1);

        if (key < node->key) node->left = put(node->left, key, value);
        else if (key > node->key) node->right = put(node->right, key, value);
        else node->value = value;

        node->size = size(node->left) + size(node->right) + 1;
        return node;
    }

public:
    void removeMin() {
        if (empty()) throw std::underflow_error("BST is empty");
        root = removeMin(root);
        // assert(check());
    }

private:
    Node* removeMin(Node* node) {
        if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        node->left = removeMin(node->left);
        node->size = size(node->left) + size(node->right) + 1;
        return node;
    }

public:
    void removeMax() {
        if (empty()) throw std::underflow_error("BST is empty");
        root = removeMax(root);
        // assert(check());
    }

private:
    Node* removeMax(Node* node) {
        if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }
        node->right = removeMax(node->right);
        node->size = size(node->left) + size(node->right) + 1;
        return node;
    }

public:
    void remove(const Key& key) {
        if (!contains(key)) throw std::invalid_argument("Key not in BST");
        root = remove(root, key);
        // assert(check());
    }

private:
    Node* remove(Node* node, const Key& key) {
        if (node == nullptr) return nullptr;

        if (key < node->key) {
            node->left = remove(node->left, key);
        }
        else if (key > node->key) {
            node->right = remove(node->right, key);
        }
        else {
            // Node to be deleted found
            if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }

            // Node has two children: replace with successor
            Node* successor = min(node->right);  // Find minimum in right subtree
            node->key = successor->key;
            node->value = successor->value;

            // Remove the successor from right subtree
            node->right = removeMin(node->right);
        }
        node->size = size(node->left) + size(node->right) + 1;
        return node;
    }

    Node* min(Node* node) const {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

public:
    Key min() const {
        if (empty()) throw std::underflow_error("BST is empty");
        Node* node = root;
        while (node->left != nullptr)
            node = node->left;
        return node->key;
    }

    Key max() const {
        if (empty()) throw std::underflow_error("BST is empty");
        Node* node = root;
        while (node->right != nullptr)
            node = node->right;
        return node->key;
    }

    Key floor(const Key& key) const {
        if (empty()) throw std::underflow_error("BST is empty");
        Node* node = floor(root, key);
        if (node == nullptr) throw std::invalid_argument("Key is too small");
        return node->key;
    }

private:
    Node* floor(Node* node, const Key& key) const {
        if (node == nullptr) return nullptr;
        if (key == node->key) return node;
        if (key < node->key) return floor(node->left, key);
        Node* temp = floor(node->right, key);
        return temp ? temp : node;
    }

public:
    Key ceiling(const Key& key) const {
        if (empty()) throw std::underflow_error("BST is empty");
        Node* node = ceiling(root, key);
        if (node == nullptr) throw std::invalid_argument("Key is too large");
        return node->key;
    }

private:
    Node* ceiling(Node* node, const Key& key) const {
        if (node == nullptr) return nullptr;
        if (key == node->key) return node;
        if (key > node->key) return ceiling(node->right, key);
        Node* temp = ceiling(node->left, key);
        return temp ? temp : node;
    }

public:
    Key select(const int i) const {
        if (i < 0 || i >= size()) throw std::invalid_argument("Rank out of range");
        return select(root, i);
    }

private:
    Key select(const Node* node, const int i) const {
        if (node == nullptr) throw std::invalid_argument("Rank out of range");
        const int leftSize = size(node->left);
        if (leftSize > i) return select(node->left, i);
        if (leftSize < i) return select(node->right, i - leftSize - 1);
        return node->key;
    }

public:
    int rank(const Key& key) const {
        return rank(root, key);
    }

private:
    int rank(const Node* node, const Key& key) const {
        if (node == nullptr) return 0;
        if (key < node->key) return rank(node->left, key);
        if (key > node->key) return 1 + size(node->left) + rank(node->right, key);
        return size(node->left);
    }

public:
    std::vector<Key> keys() const {
        if (empty()) return std::vector<Key>{};
        return keys(min(), max());
    }

    std::vector<Key> keys(const Key& lo, const Key& hi) const {
        std::vector<Key> returnKeys;
        keys(root, returnKeys, lo, hi);
        return std::move(returnKeys);
    }

private:
    void keys(const Node* node, std::vector<Key>& arr, const Key& lo, const Key& hi) const {
        if (node == nullptr) return;

        if (lo < node->key) keys(node->left, arr, lo, hi);
        if (lo <= node->key && hi >= node->key) arr.push_back(node->key);
        if (hi > node->key) keys(node->right, arr, lo, hi);
    }

public:
    int height() const { return height(root); }

private:
    int height(const Node* node) const {
        if (node == nullptr) return -1;
        return 1 + std::max(height(node->left), height(node->right));
    }

public:
    std::vector<Key> levelOrder() const {
        std::vector<Key> keys;
        if (root == nullptr) return keys;

        std::queue<Node*> nodes;
        nodes.push(root);
        while (!nodes.empty()) {
            Node* node = nodes.front();
            nodes.pop();
            if (node == nullptr) continue;
            keys.push_back(node->key);
            if (node->left) nodes.push(node->left);
            if (node->right) nodes.push(node->right);
        }
        return std::move(keys);
    }

private:
    bool check() const {
        if (!isBST()) std::cerr << "Not in symmetric order\n";
        if (!isSizeConsistent()) std::cerr << "Subtree counts not consistent\n";
        if (!isRankConsistent()) std::cerr << "Ranks not consistent\n";

        return isBST() && isSizeConsistent() && isRankConsistent();
    }

    bool isBST() const {
        return isBST(root, std::nullopt, std::nullopt);
    }

    bool isBST(const Node* node, std::optional<Key> min, std::optional<Key> max) const {
        if (!node) return true;

        if (min && node->key <= *min) return false;
        if (max && node->key >= *max) return false;

        return isBST(node->left, min, node->key) &&
               isBST(node->right, node->key, max);
    }

    bool isSizeConsistent() const { return isSizeConsistent(root); }

    bool isSizeConsistent(const Node* node) const {
        if (node == nullptr) return true;
        if (node->size != size(node->left) + size(node->right) + 1) return false;
        return isSizeConsistent(node->left) && isSizeConsistent(node->right);
    }

    bool isRankConsistent() const {
        for (int i = 0; i < size(); ++i)
            if (i != rank(select(i))) return false;

        for (const auto& key : keys())
            if (key != select(rank(key))) return false;

        return true;
    }
};
