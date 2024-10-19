#pragma once

template<typename Item>
class ResizingArrayQueue {
private:
    Item* q;            // Queue elements (array)
    int n;              // Number of elements in the queue
    int first;          // Index of the first element of the queue
    int last;           // Index of the next available slot
    int capacity;       // Size of the underlying array

    // Resizes the underlying array to the given capacity
    void resize(int new_capacity) {
        Item* temp = new Item[new_capacity];
        for (int i = 0; i < n; i++) {
            temp[i] = q[(first + i) % capacity];
        }
        delete[] q;
        q = temp;
        first = 0;
        last = n;
        capacity = new_capacity;
    }

public:
    // Constructor
    ResizingArrayQueue() : n(0), first(0), last(0), capacity(8) {
        q = new Item[capacity];
    }

    // Destructor to free allocated memory
    ~ResizingArrayQueue() {
        delete[] q;
    }

    // Check if the queue is empty
    bool isEmpty() const {
        return n == 0;
    }

    // Return the number of items in the queue
    int size() const {
        return n;
    }

    // Adds an item to the queue
    void enqueue(const Item& item) {
        if (n == capacity) {
            resize(2 * capacity);   // Double the size of the array if necessary
        }
        q[last++] = item;
        if (last == capacity) last = 0;  // Wrap-around
        n++;
    }

    // Removes and returns the item that was least recently added
    Item dequeue() {
        if (isEmpty()) {
            throw std::out_of_range("Queue underflow");
        }
        Item item = q[first];
        q[first] = Item();  // Optional: to avoid loitering
        first++;
        n--;
        if (first == capacity) first = 0;  // Wrap-around
        if (n > 0 && n == capacity / 4) {
            resize(capacity / 2);  // Shrink array size if necessary
        }
        return item;
    }

    // Returns the item least recently added (front of the queue)
    Item peek() const {
        if (isEmpty()) {
            throw std::out_of_range("Queue underflow");
        }
        return q[first];
    }


};

