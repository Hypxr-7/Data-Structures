#include <limits>
#include <queue>
#include <vector>

template <class T>
class MedianPQ {
private:
    // Values greater than the median sorted so that smallest is on top
    std::priority_queue<T, std::vector<T>, std::greater<T>> upper;
    // Values smaller than the median sorted so that greatest is on top
    std::priority_queue<T, std::vector<T>, std::less<T>> lower;

public:
    MedianPQ() {
        // Seed the queues
        upper.push(std::numeric_limits<T>::max());
        lower.push(std::numeric_limits<T>::min());
    }

    void push(T val) {
        // If number is greater than the least upper number, it is an upper number
        if (val >= upper.top())
            upper.push(val);
        else // Otherwise it is a lower number
            lower.push(val);

        // Rebalance
        // If the queues sizes differ by 2, they need to be rebalanced so that they
        // differ by 1.
        if (upper.size() - lower.size() == 2) { // Upper queue is larger
            lower.push(upper.top()); // Move value from upper to lower
            upper.pop();             // Remove same value from upper
        }
        else if (lower.size() - upper.size() == 2) { // Lower queue is larger
            upper.push(lower.top()); // Move value from lower to upper
            lower.pop();             // Remove same value
        }
    }

    T getMedian() const {
        if (upper.size() == lower.size())                  // Upper and lower are same size
            return (upper.top() + lower.top()) / ((T)2.0); // so median is average of least upper and greatest lower
        else if (upper.size() > lower.size())              // Upper size greater
            return upper.top();
        else                                               // Lower size greater
            return lower.top();
    }
    
    void removeMedian() {
        if (upper.size() == lower.size()) {
            // If both heaps are of the same size, remove the top element from the upper heap
            upper.pop();
        } else if (upper.size() > lower.size()) {
            // If the upper heap is larger, remove the top element from the upper heap
            upper.pop();
        } else {
            // If the lower heap is larger, remove the top element from the lower heap
            lower.pop();
        }
    }
};