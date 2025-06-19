#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <vector>

template <typename Key, typename Value>
class RedBlackBST {
private:
    /// Color constants for red-black tree nodes
    static constexpr bool RED = true;
    static constexpr bool BLACK = false;

    struct Node {
        Key key;        // The key stored in this node
        Value value;    // The value associated with the key
        Node* left;     // Pointer to left child (smaller keys)
        Node* right;    // Pointer to right child (larger keys)
        bool color;     // Color of the node (RED or BLACK)
        int size;       // Number of nodes in subtree rooted at this node

        Node(const Key& key, const Value& value, const bool color, const int size) :
            key(key),
            value(value),
            left(nullptr),
            right(nullptr),
            color(color),
            size(size) {}
    };

    Node* root;  // Pointer to the root node of the red-black tree

public:
    RedBlackBST() : root(nullptr) {}

    ~RedBlackBST() { clear(root); }

private:
    void clear(Node* node) {
        if (node != nullptr) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    bool isRed(const Node* node) const {
        return node ? node->color == RED : false;
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
        root->color = BLACK;  // Root is always black
        // assert(check());
    }

private:
    Node* put(Node* node, const Key& key, const Value& value) {
        if (node == nullptr) return new Node(key, value, RED, 1);

        if (key < node->key) node->left = put(node->left, key, value);
        else if (key > node->key) node->right = put(node->right, key, value);
        else node->value = value;

        // Red-black tree balancing operations
        if (isRed(node->right) && !isRed(node->left)) node = rotateLeft(node);
        if (isRed(node->left) && isRed(node->left ? node->left->left : nullptr)) node = rotateRight(node);
        if (isRed(node->left) && isRed(node->right)) flipColors(node);

        node->size = size(node->left) + size(node->right) + 1;
        return node;
    }

public:
    void removeMin() {
        if (empty()) throw std::underflow_error("BST is empty");

        if (!isRed(root->left) && !isRed(root->right)) root->color = RED;
        root = removeMin(root);
        if (!empty()) root->color = BLACK;

        // assert(check());
    }

private:
    Node* removeMin(Node* node) {
        if (node->left == nullptr) {
            delete node;
            return nullptr;
        }

        if (!isRed(node->left) && !isRed(node->left ? node->left->left : nullptr))
            node = moveRedLeft(node);

        node->left = removeMin(node->left);
        return balance(node);
    }

public:
    void removeMax() {
        if (empty()) throw std::underflow_error("BST is empty");
        if (!isRed(root->left) && !isRed(root->right)) root->color = RED;
        root = removeMax(root);
        if (!empty()) root->color = BLACK;

        // assert(check());
    }

private:
    Node* removeMax(Node* node) {
        if (isRed(node->left)) node = rotateRight(node);

        if (node->right == nullptr) {
            delete node;
            return nullptr;
        }
        if (!isRed(node->right) && !isRed(node->right ? node->right->left : nullptr))
            node = moveRedRight(node);

        node->right = removeMax(node->right);
        return balance(node);
    }

public:
    void remove(const Key& key) {
        if (!contains(key)) throw std::invalid_argument("Key not in BST");
        if (!isRed(root->left) && !isRed(root->right)) root->color = RED;
        root = remove(root, key);
        if (!empty()) root->color = BLACK;

        // assert(check());
    }

private:
    Node* remove(Node* node, const Key& key) {
        if (key < node->key) {
            if (!isRed(node->left) && !isRed(node->left ? node->left->left : nullptr))
                node = moveRedLeft(node);
            node->left = remove(node->left, key);
        } else {
            if (isRed(node->left)) node = rotateRight(node);
            if (key == node->key && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            if (!isRed(node->right) && !isRed(node->right ? node->right->left : nullptr))
                node = moveRedRight(node);
            if (key == node->key) {
                Node* temp = min(node->right);
                node->key = temp->key;
                node->value = temp->value;
                node->right = removeMin(node->right);
            } else
                node->right = remove(node->right, key);
        }
        return balance(node);
    }

    Node* min(Node* node) const {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    Node* rotateRight(Node* node) {
        Node* temp = node->left;
        node->left = temp->right;
        temp->right = node;
        temp->color = node->color;
        node->color = RED;
        temp->size = node->size;
        node->size = size(node->left) + size(node->right) + 1;
        return temp;
    }

    Node* rotateLeft(Node* node) {
        Node* temp = node->right;
        node->right = temp->left;
        temp->left = node;
        temp->color = node->color;
        node->color = RED;
        temp->size = node->size;
        node->size = size(node->left) + size(node->right) + 1;
        return temp;
    }

    void flipColors(Node* node) {
        node->color = !node->color;
        if (node->left) node->left->color = !node->left->color;
        if (node->right) node->right->color = !node->right->color;
    }

    Node* moveRedLeft(Node* node) {
        flipColors(node);
        if (node->right && isRed(node->right->left)) {
            node->right = rotateRight(node->right);
            node = rotateLeft(node);
            flipColors(node);
        }
        return node;
    }

    Node* moveRedRight(Node* node) {
        flipColors(node);
        if (node->left && isRed(node->left->left)) {
            node = rotateRight(node);
            flipColors(node);
        }
        return node;
    }

    Node* balance(Node* node) {
        if (isRed(node->right) && !isRed(node->left)) node = rotateLeft(node);
        if (isRed(node->left) && isRed(node->left ? node->left->left : nullptr))
            node = rotateRight(node);
        if (isRed(node->left) && isRed(node->right)) flipColors(node);

        node->size = size(node->left) + size(node->right) + 1;
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

    bool check() const {
        if (!isBST()) std::cerr << "Not in symmetric order\n";
        if (!isSizeConsistent()) std::cerr << "Subtree counts not consistent\n";
        if (!isRankConsistent()) std::cerr << "Ranks not consistent\n";
        if (!is23()) std::cerr << "Not a 2-3 tree\n";
        if (!isBalanced()) std::cerr << "Not balanced\n";

        return isBST() && isSizeConsistent() && isRankConsistent() && is23() && isBalanced();
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

    bool is23() const { return is23(root); }

    bool is23(Node* node) const {
        if (node == nullptr) return true;

        if (isRed(node->right)) return false;
        if (node != root && isRed(node) && isRed(node->left)) return false;
        return is23(node->left) && is23(node->right);
    }

    bool isBalanced() const {
        int black = 0;
        Node* node = root;
        while (node != nullptr) {
            if (!isRed(node)) ++black;
            node = node->left;
        }
        return isBalanced(root, black);
    }

    bool isBalanced(Node* node, int black) const {
        if (node == nullptr) return black == 0;
        if (!isRed(node)) --black;
        return isBalanced(node->left, black) && isBalanced(node->right, black);
    }
};
