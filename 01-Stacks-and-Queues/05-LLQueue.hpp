#pragma once

template <typename Item>
class Queue {
private:
    // Node structure representing each item in the queue
    struct Node {
        Item item;
        Node* next;
    };

    Node* first;  // pointer to the first node (front of queue)
    Node* last;   // pointer to the last node (end of queue)
    int n;        // number of elements in the queue

public:
    // Constructor to initialize an empty queue
    Queue() : first(nullptr), last(nullptr), n(0) {}

    // Destructor to clean up memory used by the queue
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Check if the queue is empty
    bool isEmpty() const {
        return first == nullptr;
    }

    // Get the number of items in the queue
    int size() const {
        return n;
    }

    // Peek at the first item in the queue without removing it
    Item peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Queue underflow");
        }
        return first->item;
    }

    // Add an item to the end of the queue
    void enqueue(const Item& item) {
        Node* oldLast = last;
        last = new Node;
        last->item = item;
        last->next = nullptr;
        if (isEmpty()) {
            first = last;
        } else {
            oldLast->next = last;
        }
        n++;
    }

    // Remove and return the item from the front of the queue
    Item dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Queue underflow");
        }
        Item item = first->item;
        Node* oldFirst = first;
        first = first->next;
        delete oldFirst;
        n--;
        if (isEmpty()) {
            last = nullptr;  // to avoid loitering
        }
        return item;
    }
};
