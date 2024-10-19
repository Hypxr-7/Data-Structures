#include <algorithm>
#include <vector>

template <typename Item>
class Heap {
private:
    // This class should not be instantiated.
    Heap() {}

    // Helper function to sink an element in the heap
    static void sink(std::vector<Item>& pq, int k, int n) {
        while (2 * k <= n) {
            int j = 2 * k;
            if (j < n && pq[j - 1] < pq[j]) j++;
            if (!(pq[k - 1] < pq[j - 1])) break;
            std::swap(pq[k - 1], pq[j - 1]);
            k = j;
        }
    }

public:
     //Rearranges the array in ascending order, using the natural order.
    static void sort(std::vector<Item>& pq) {
        int n = pq.size();

        // heapify phase
        for (int k = n / 2; k >= 1; k--)
            sink(pq, k, n);

        // sort down phase
        int k = n;
        while (k > 1) {
            std::swap(pq[0], pq[k - 1]);
            k--;
            sink(pq, 1, k);
        }
    }
};
