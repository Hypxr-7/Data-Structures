#pragma once

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

template <typename Key, typename Value>
class RedBlackBST {
private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        bool color;
        int size;

        Node(Key _key, Value _value, bool _color, int _size) :
            key(_key),
            value(_value),
            color(_color),
            size(_size),
            left(nullptr),
            right(nullptr)
            {}
    };

    static const bool RED = true;
    static const bool BLACK = false;

    Node* m_root;

public: 
    RedBlackBST() : m_root(nullptr) {}

    ~RedBlackBST() {
        clear(m_root);
    }

    void clear(Node* node) {
        if (node != nullptr) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

/**********************
 * Node helper methods
 * ********************/
private: 
    bool isRed(const Node* node) const {
        if (node == nullptr) return false;
        return node->color == RED;
    }

public:
    bool isEmpty() const { return m_root == nullptr; }

public:
    int size() const {
        return size(m_root);
    }

private:
    int size(const Node* node) const {
        if (node == nullptr) return 0;
        return node->size;
    }

/**********************
 * Standard BST search
 * ********************/
public:
    Value get(const Key& key) const {
        return get(m_root, key);
    }

private:
    Value get(Node* node, const Key& key) const {
        while (node != nullptr) {
            if (key < node->key) node = node->left;
            else if (key > node->key) node = node->right;
            else return node->value;
        }
        return Value{};
    }

public:
    bool contains(const Key& key) const {
        return get(key) != Value{};
    }

/**********************
 * Red-black tree insertion
 * ********************/
public:
    void put(const Key& key, const Value& value) {
        if (value == Value{}) {
            deleteKey(key); 
            return;
        }
        m_root = put(m_root, key, value);
        m_root->color = BLACK;

        assert(check());
    }

private:
    Node* put(Node* node, const Key& key, const Value& value) {
        if (node == nullptr) return new Node(key, value, RED, 1);

        if (key < node->key) node->left = put(node->left, key, value);
        else if (key > node->key) node->right = put(node->right, key, value);
        else node->value = value;

        if (isRed(node->right) && !isRed(node->left)) node = rotateLeft(node);
        if (isRed(node->left) && isRed(node->left->left)) node = rotateRight(node);
        if (isRed(node->left) && isRed(node->right)) flipColors(node);

        node->size = size(node->left) + size(node->right) + 1;
        return node;
    }

/**********************
 * Red-black tree deletion
 * ********************/
public:
    void deleteMin() {
        if (isEmpty()) throw std::underflow_error("BST underflow");
        if (!isRed(m_root->left) && !isRed(m_root->right)) m_root->color = RED;
        m_root = deleteMin(m_root);
        if (!isEmpty()) m_root->color = BLACK;
        else m_root = nullptr;

        assert(check());
    }

private:
    Node* deleteMin(Node* node) {
        if (node->left == nullptr) {
            delete node;
            return nullptr;
        }

        if (!isRed(node->left) && !isRed(node->left->left)) node = moveRedLeft(node);

        node->left = deleteMin(node->left);
        return balance(node);
    }

public:
    void deleteMax() {
        if (isEmpty()) throw std::underflow_error("BST underflow");
        if (!isRed(m_root->left) && !isRed(m_root->right)) m_root->color = RED;
        m_root = deleteMax(m_root);
        if (!isEmpty()) m_root->color = BLACK;

        assert(check());
    }

private:
    Node* deleteMax(Node* node) {
        if (isRed(node->left)) node = rotateRight(node);

        if (node->right == nullptr) {
            delete node;
            return nullptr;
        }
        if (!isRed(node->right) && !isRed(node->right->left)) node = moveRedRight(node);

        node->right = deleteMax(node->right);
        return balance(node);
    }

public:
    void deleteKey(const Key& key) {
        if (!contains(key)) return;
        if (!isRed(m_root->left) && !isRed(m_root->right)) m_root->color = RED;
        m_root = deleteKey(m_root, key);
        if (!isEmpty()) m_root->color = BLACK;
        else m_root = nullptr;

        assert(check());
    }

private:
    Node* deleteKey(Node* node, const Key& key) {
        if (key < node->key) {
            if (!isRed(node->left) && !isRed(node->left->left)) node = moveRedLeft(node);
            node->left = deleteKey(node->left, key);
        } else {
            if (isRed(node->left)) node = rotateRight(node);
            if (key == node->key && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            if (!isRed(node->right) && !isRed(node->right->left)) node = moveRedRight(node);
            if (key == node->key) {
                Node* temp = min(node->right);
                node->key = temp->key;
                node->value = temp->value;
                node->right = deleteMin(node->right);
            } else node->right = deleteKey(node->right, key);
        }
        return balance(node);
    }

/**********************
 * Red-black tree helper functions
 * ********************/
private:
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
        node->left->color = !node->left->color;
        node->right->color = !node->right->color;
    }

    Node* moveRedLeft(Node* node) {
        flipColors(node);
        if (isRed(node->right->left)) {
            node->right = rotateRight(node->right);
            node = rotateLeft(node);
            flipColors(node);
        }
        return node;
    }

    Node* moveRedRight(Node* node) {
        flipColors(node);
        if (isRed(node->left->left)) {
            node = rotateRight(node);
            flipColors(node);
        }
        return node;
    }

    Node* balance(Node* node) {
        if (isRed(node->right) && !isRed(node->left)) node = rotateLeft(node);
        if (isRed(node->left) && isRed(node->left->left)) node = rotateRight(node);
        if (isRed(node->left) && isRed(node->right)) flipColors(node);

        node->size = size(node->left) + size(node->right) + 1;
        return node;
    }

/**********************
 * Utility functions
 * ********************/
public:
    int height() const {
        return height(m_root);
    }

private:
    int height(const Node* node) const {
        if (node == nullptr) return -1;
        return 1 + std::max(height(node->left), height(node->right));
    }

/**********************
 * Ordered symbol table methods
 * ********************/
public:
    Key min() const {
        if (isEmpty()) throw std::underflow_error("calls min() with empty symbol table");
        return min(m_root)->key;
    }

private:
    Node* min(Node* node) const {
        if (node->left == nullptr) return node;
        else return min(node->left);
    }

public:
    Key max() const {
        if (isEmpty()) throw std::underflow_error("calls max() with empty symbol table");
        return max(m_root)->key;
    }

private:
    Node* max(Node* node) const {
        if (node->right == nullptr) return node;
        else return max(node->right);
    }

public:
    Key floor(const Key& key) const {
        if (isEmpty()) throw std::invalid_argument("calls floor with empty symbol table");
        Node* node = floor(m_root, key);
        if (node == nullptr) throw std::runtime_error("argument to floor() is too small");
        else return node->key;
    }

private:
    Node* floor(Node* node, const Key& key) const {
        if (node == nullptr) return nullptr;
        if (key == node->key) return node;
        if (key < node->key) return floor(node->left, key);
        Node* temp = floor(node->right, key);
        if (temp != nullptr) return temp;
        else return node;
    }

public:
    Key ceiling(const Key& key) const {
        if (isEmpty()) throw std::invalid_argument("calls ceiling with empty symbol table");
        Node* node = ceiling(m_root, key);
        if (node == nullptr) throw std::runtime_error("argument to ceiling() is too large");
        else return node->key;
    }

private:
    Node* ceiling(Node* node, const Key& key) const {
        if (node == nullptr) return nullptr;
        if (key == node->key) return node;
        if (key > node->key) return ceiling(node->right, key);
        Node* temp = ceiling(node->left, key);
        if (temp != nullptr) return temp;
        else return node;
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
    int rank(const Node* node, const Key& key) const {
        if (node == nullptr) return 0;
        if (key < node->key) return rank(node->left, key);
        else if (key > node->key) return 1 + size(node->left) + rank(node->right, key);
        else return size(node->left);
    }

/**********************
 * Range count and range search
 * ********************/
public:
    std::vector<Key> keys() const {
        if (isEmpty()) return std::vector<Key>{};
        return std::move(keys(min(), max()));
    }

    std::vector<Key> keys(const Key& lo, const Key& hi) const {
        std::vector<Key> arr;
        keys(m_root, arr, lo, hi);
        return std::move(arr);
    }

private:
    void keys(const Node* node, std::vector<Key>& arr, const Key& lo, const Key& hi) const {
        if (node == nullptr) return;
        if (lo < node->key) keys(node->left, arr, lo, hi);
        if (lo <= node->key && hi >= node->key) arr.push_back(node->key);
        if (hi > node->key) keys(node->right, arr, lo, hi);
    }

public:
    int size(const Key& lo, const Key& hi) const {
        if (lo > hi) return 0;
        if (contains(hi)) return rank(hi) - rank(lo) + 1;
        else return rank(hi) - rank(lo);
    }

/**********************
 * Check integrity of red-black tree data structure
 * ********************/
private:
    bool check() const {
        if (!isBST()) std::cerr << "Not in symmetric order\n";
        if (!isSizeConsistent()) std::cerr << "Subtree counts not consistent\n";
        if (!isRankConsistent()) std::cerr << "Ranks not consistent\n";
        if (!is23()) std::cerr << "Not a 2-3 tree\n";
        if (!isBalanced()) std::cerr << "Not balanced\n";

        return isBST() && isSizeConsistent() && isRankConsistent() && is23() && isBalanced(); 
    }

    bool isBST() const {
        return isBST(m_root, Key{}, Key{});
    }

    bool isBST(Node* node, const Key& min, const Key& max) const {
        if (node == nullptr) return true;
        if (min != Key{} && node->key <= min) return false;
        if (max != Key{} && node->key >= max) return false;

        return isBST(node->left, min, node->key) && isBST(node->right, node->key, max);
    }

    bool isSizeConsistent() const {
        return isSizeConsistent(m_root);
    }

    bool isSizeConsistent(Node* node) const {
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

    bool is23() const {
        return is23(m_root);
    }

    bool is23(Node* node) const {
        if (node == nullptr) return true;

        if (isRed(node->right)) return false;
        if (node != m_root && isRed(node) && isRed(node->left)) return false;
        return is23(node->left) && is23(node->right);
    }

    bool isBalanced() const {
        int black = 0;
        Node* node = m_root;
        while (node != nullptr) {
            if (!isRed(node)) ++black;
            node = node->left;
        }
        return isBalanced(m_root, black);
    }

    bool isBalanced(Node* node, int black) const {
        if (node == nullptr) return black == 0;
        if (!isRed(node)) --black;
        return isBalanced(node->left, black) && isBalanced(node->right, black);
    }
};