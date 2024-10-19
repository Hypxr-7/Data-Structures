#pragma once

template <typename Item>
class ResizingArrayStack {
private:
    Item* a;       // array of items
    int n;         // number of elements on stack
    int capacity;  // size of the array

    // resize the underlying array holding the elements
    void resize(int new_capacity) {
        Item* temp = new Item[new_capacity];
        for (int i = 0; i < n; ++i) {
            temp[i] = a[i];
        }
        delete[] a;
        a = temp;
        capacity = new_capacity;
    }

public:
    // Constructor to initialize an empty stack
    ResizingArrayStack() : n(0), capacity(8) {
        a = new Item[capacity];
    }

    // Destructor to free memory
    ~ResizingArrayStack() {
        delete[] a;
    }

    // Check if the stack is empty
    bool isEmpty() const {
        return n == 0;
    }

    // Return the number of items in the stack
    int size() const {
        return n;
    }

    // Add the item to this stack
    void push(const Item& item) {
        if (n == capacity) {
            resize(2 * capacity);  // double size of array if necessary
        }
        a[n++] = item;
    }

    // Remove and return the item most recently added to this stack
    Item pop() {
        if (isEmpty()) {
            throw std::out_of_range("Stack underflow");
        }
        Item item = a[--n];
        a[n] = Item();  // optional: avoid loitering
        if (n > 0 && n == capacity / 4) {
            resize(capacity / 2);  // shrink array size if necessary
        }
        return item;
    }

    // Return (but do not remove) the item most recently added to this stack
    Item peek() const {
        if (isEmpty()) {
            throw std::out_of_range("Stack underflow");
        }
        return a[n - 1];
    }
};
