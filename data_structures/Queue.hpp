#pragma once

#include <stdexcept>

template <typename Item>
class Queue {
private:
    struct Node {
        Item item;   // The data stored in this node
        Node* next;  // Pointer to the next node in the list
    };

    Node* first;  // Pointer to the front of the queue (first node to be
                  // dequeued)
    Node* last;   // Pointer to the back of the queue (last node that was
                  // enqueued)
    int count;    // Number of elements currently in the queue

public:
    Queue() : first{nullptr}, last{nullptr}, count{0} {}

    ~Queue() {
        while (first != nullptr) {
            Node* temp = first;
            first = first->next;
            delete temp;
        }
    }

    bool empty() const { return count == 0; }

    int size() const { return count; }

    void enqueue(const Item& item) {
        Node* temp = last;
        last = new Node{item, nullptr};

        if (empty())
            first = last;
        else
            temp->next = last;

        ++count;
    }

    Item dequeue() {
        if (empty()) throw std::underflow_error("Queue is empty");

        Item item = first->item;
        Node* temp = first;
        first = first->next;
        delete temp;
        --count;

        if (empty()) last = nullptr;
        return item;
    }

    Item peek() const {
        if (empty()) throw std::underflow_error("Queue is empty");
        return first->item;
    }
};
