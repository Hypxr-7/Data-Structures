#pragma once

#include <stdexcept>


template <typename Item>
class Stack {
private:
    struct Node {
        Item item;   // The data stored in this node
        Node* next;  // Pointer to the next node in the list
    };

    Node* first;  // Pointer to the top of the stack (first node)
    int count;    // Number of elements currently in the stack

public:
    Stack() : first{nullptr}, count{0} {}

    ~Stack() {
        while (first != nullptr) {
            Node* temp = first;
            first = first->next;
            delete temp;
        }
    }

    bool empty() const { return count == 0; }

    int size() const { return count; }

    void push(const Item& item) {
        Node* temp = first;
        first = new Node{item, temp};

        ++count;
    }

    Item pop() {
        if (empty()) throw std::underflow_error("Stack is empty");

        Item temp = first->item;
        Node* old_first = first;
        first = first->next;
        delete old_first;
        --count;
        return temp;
    }

    Item peek() const {
        if (empty()) throw std::underflow_error("Stack is empty");
        return first->item;
    }
};
