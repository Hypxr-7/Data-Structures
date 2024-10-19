#pragma once

template <typename Item>
class Stack {
private:
    // Node structure representing each item in the stack
    struct Node {
        Item item;
        Node* next;
    };

    Node* first; // pointer to the first node (top of stack)
    int n;       // size of the stack

public:
    // Constructor to initialize an empty stack
    Stack() : first(nullptr), n(0) {}

    // Destructor to clean up the memory used by the stack
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    // Check if the stack is empty
    bool isEmpty() const {
        return first == nullptr;
    }

    // Get the number of items in the stack
    int size() const {
        return n;
    }

    // Add an item to the top of the stack
    void push(const Item& item) {
        Node* oldFirst = first;
        first = new Node;
        first->item = item;
        first->next = oldFirst;
        n++;
    }

    // Remove and return the item from the top of the stack
    Item pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack underflow");
        }
        Item item = first->item;
        Node* oldFirst = first;
        first = first->next;
        delete oldFirst;
        n--;
        return item;
    }

    // Peek at the item at the top of the stack without removing it
    Item peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack underflow");
        }
        return first->item;
    }
};
