#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <cassert>
#include <stdexcept>

// Include the IndexMaxPQ header
#include "../data_structures/IndexMaxPQ.hpp"

// Test helper functions
template <typename T>
bool isValidMaxHeap(const std::vector<int>& indices, const std::vector<T>& keys) {
    for (size_t i = 1; i < indices.size(); ++i) {
        size_t parent = i / 2;
        if (parent > 0 && keys[indices[parent]] < keys[indices[i]]) {
            return false;
        }
    }
    return true;
}

void test_constructor_and_basic_properties() {
    std::cout << "Testing constructor and basic properties... ";

    IndexMaxPQ<int> pq(10);

    assert(pq.empty());
    assert(pq.size() == 0);

    // Test invalid capacity
    try {
        IndexMaxPQ<int> invalid_pq(-1);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        // Expected exception
    }

    std::cout << "PASSED\n";
}

void test_single_element_operations() {
    std::cout << "Testing single element operations... ";

    IndexMaxPQ<double> pq(5);

    // Insert single element
    pq.insert(2, 3.14);

    assert(!pq.empty());
    assert(pq.size() == 1);
    assert(pq.contains(2));
    assert(!pq.contains(0));
    assert(!pq.contains(1));
    assert(pq.maxIndex() == 2);
    assert(pq.maxKey() == 3.14);
    assert(pq.keyOf(2) == 3.14);

    // Remove the element
    int removed = pq.removeMax();
    assert(removed == 2);
    assert(pq.empty());
    assert(!pq.contains(2));

    std::cout << "PASSED\n";
}

void test_basic_insert_and_max_operations() {
    std::cout << "Testing basic insert and max operations... ";

    IndexMaxPQ<int> pq(10);

    // Insert elements
    pq.insert(0, 10);
    pq.insert(1, 5);
    pq.insert(2, 20);
    pq.insert(3, 15);

    assert(pq.size() == 4);
    assert(pq.maxIndex() == 2);  // Index 2 has highest priority (20)
    assert(pq.maxKey() == 20);
    assert(pq.keyOf(2) == 20);

    // Remove max and check next max
    int max1 = pq.removeMax();
    assert(max1 == 2);
    assert(pq.maxIndex() == 3);  // Next highest is index 3 with priority 15
    assert(pq.maxKey() == 15);

    int max2 = pq.removeMax();
    assert(max2 == 3);
    assert(pq.maxIndex() == 0);  // Next is index 0 with priority 10

    int max3 = pq.removeMax();
    assert(max3 == 0);
    assert(pq.maxIndex() == 1);  // Last is index 1 with priority 5

    int max4 = pq.removeMax();
    assert(max4 == 1);
    assert(pq.empty());

    std::cout << "PASSED\n";
}

void test_change_key_operations() {
    std::cout << "Testing changeKey operations... ";

    IndexMaxPQ<double> pq(5);

    pq.insert(0, 1.0);
    pq.insert(1, 2.0);
    pq.insert(2, 3.0);

    assert(pq.maxIndex() == 2);  // Initially, index 2 has max priority

    // Change key of index 0 to highest priority
    pq.changeKey(0, 5.0);
    assert(pq.maxIndex() == 0);
    assert(pq.maxKey() == 5.0);

    // Change key of index 1 to even higher priority
    pq.changeKey(1, 10.0);
    assert(pq.maxIndex() == 1);
    assert(pq.maxKey() == 10.0);

    // Decrease a key
    pq.changeKey(1, 0.5);
    assert(pq.maxIndex() == 0);  // Now index 0 should be max again

    std::cout << "PASSED\n";
}

void test_increase_decrease_key() {
    std::cout << "Testing increaseKey and decreaseKey... ";

    IndexMaxPQ<int> pq(5);

    pq.insert(0, 10);
    pq.insert(1, 20);
    pq.insert(2, 30);

    assert(pq.maxIndex() == 2);

    // Increase key of index 0
    pq.increaseKey(0, 35);
    assert(pq.maxIndex() == 0);
    assert(pq.keyOf(0) == 35);

    // Decrease key of index 0
    pq.decreaseKey(0, 5);
    assert(pq.maxIndex() == 2);  // Index 2 should be max again
    assert(pq.keyOf(0) == 5);

    // Test error conditions for increaseKey
    try {
        pq.increaseKey(0, 5);  // Same key
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        pq.increaseKey(0, 3);  // Smaller key
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    // Test error conditions for decreaseKey
    try {
        pq.decreaseKey(1, 20);  // Same key
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        pq.decreaseKey(1, 25);  // Larger key
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "PASSED\n";
}

void test_remove_arbitrary_element() {
    std::cout << "Testing remove arbitrary element... ";

    IndexMaxPQ<int> pq(10);

    // Insert several elements
    for (int i = 0; i < 5; ++i) {
        pq.insert(i, i * 10);
    }

    assert(pq.size() == 5);
    assert(pq.contains(2));

    // Remove element from middle
    pq.remove(2);
    assert(pq.size() == 4);
    assert(!pq.contains(2));

    // Verify remaining elements maintain heap property
    assert(pq.maxIndex() == 4);  // Should still be the highest

    // Remove max element
    pq.remove(4);
    assert(pq.maxIndex() == 3);

    // Remove all remaining elements
    pq.remove(0);
    pq.remove(1);
    pq.remove(3);
    assert(pq.empty());

    std::cout << "PASSED\n";
}

void test_edge_cases_and_errors() {
    std::cout << "Testing edge cases and error conditions... ";

    IndexMaxPQ<int> pq(3);

    // Test operations on empty PQ
    try {
        pq.maxIndex();
        assert(false);
    } catch (const std::underflow_error&) {
        // Expected
    }

    try {
        pq.maxKey();
        assert(false);
    } catch (const std::underflow_error&) {
        // Expected
    }

    try {
        pq.removeMax();
        assert(false);
    } catch (const std::underflow_error&) {
        // Expected
    }

    // Test invalid indices
    try {
        pq.insert(-1, 10);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        pq.insert(3, 10);  // Index >= capacity
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    // Insert valid element
    pq.insert(0, 5);

    // Test duplicate insertion
    try {
        pq.insert(0, 10);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    // Test operations on non-existent indices
    try {
        pq.keyOf(1);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        pq.changeKey(1, 15);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        pq.remove(1);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "PASSED\n";
}

void test_different_data_types() {
    std::cout << "Testing different data types... ";

    // Test with strings
    IndexMaxPQ<std::string> string_pq(5);
    string_pq.insert(0, "apple");
    string_pq.insert(1, "zebra");
    string_pq.insert(2, "banana");

    assert(string_pq.maxIndex() == 1);  // "zebra" is lexicographically largest
    assert(string_pq.maxKey() == "zebra");

    // Test with custom struct
    struct Task {
        std::string name;
        int priority;

        Task() : name(""), priority(0) {}
        Task(const std::string& n, int p) : name(n), priority(p) {}

        bool operator<(const Task& other) const { return priority < other.priority; }
        bool operator>(const Task& other) const { return priority > other.priority; }
        bool operator==(const Task& other) const { return priority == other.priority; }
        bool operator>=(const Task& other) const { return priority >= other.priority; }
    };

    IndexMaxPQ<Task> task_pq(3);
    task_pq.insert(0, Task("Low", 1));
    task_pq.insert(1, Task("High", 10));
    task_pq.insert(2, Task("Medium", 5));

    assert(task_pq.maxIndex() == 1);  // High priority task
    assert(task_pq.maxKey().name == "High");

    std::cout << "PASSED\n";
}

void test_heap_property_maintenance() {
    std::cout << "Testing heap property maintenance... ";

    IndexMaxPQ<int> pq(20);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> index_dis(0, 19);
    std::uniform_int_distribution<int> key_dis(1, 100);

    // Perform many random operations
    std::vector<int> inserted_indices;

    for (int i = 0; i < 100; ++i) {
        int operation = i % 4;

        if (operation == 0 && inserted_indices.size() < 20) {
            // Insert
            int idx = index_dis(gen);
            if (!pq.contains(idx)) {
                int key = key_dis(gen);
                pq.insert(idx, key);
                inserted_indices.push_back(idx);
            }
        } else if (operation == 1 && !pq.empty()) {
            // Remove max
            int max_idx = pq.removeMax();
            inserted_indices.erase(
                std::find(inserted_indices.begin(), inserted_indices.end(), max_idx)
            );
        } else if (operation == 2 && !inserted_indices.empty()) {
            // Change key
            int idx = inserted_indices[gen() % inserted_indices.size()];
            int new_key = key_dis(gen);
            pq.changeKey(idx, new_key);
        } else if (operation == 3 && !inserted_indices.empty()) {
            // Remove arbitrary
            int idx = inserted_indices[gen() % inserted_indices.size()];
            pq.remove(idx);
            inserted_indices.erase(
                std::find(inserted_indices.begin(), inserted_indices.end(), idx)
            );
        }

        // Verify heap property is maintained
        if (!pq.empty()) {
            int max_idx = pq.maxIndex();
            int max_key = pq.maxKey();

            // Verify this is actually the maximum
            auto indices = pq.indices();
            bool is_actually_max = true;
            for (int idx : indices) {
                if (pq.keyOf(idx) > max_key) {
                    is_actually_max = false;
                    break;
                }
            }
            assert(is_actually_max);
        }
    }

    std::cout << "PASSED\n";
}

void test_dijkstra_simulation() {
    std::cout << "Testing Dijkstra-like algorithm simulation... ";

    // Simulate a simple graph with 5 vertices
    const int V = 5;
    IndexMaxPQ<int> pq(V);  // Use negative distances for max heap behavior

    // Initialize distances (using negative values since we have max heap)
    std::vector<int> dist(V, 1000);  // "Infinity" as large positive number
    dist[0] = 0;  // Start vertex

    // Insert all vertices with negative distances (so max heap acts like min heap)
    for (int i = 0; i < V; ++i) {
        pq.insert(i, -dist[i]);  // Negative for max heap to simulate min heap
    }

    // Simulate relaxing edges
    // Edge 0->1 with weight 4
    if (dist[1] > dist[0] + 4) {  // Standard distance comparison
        dist[1] = dist[0] + 4;
        pq.changeKey(1, -dist[1]);  // Update with negative distance
    }

    // Edge 0->2 with weight 2
    if (dist[2] > dist[0] + 2) {  // Standard distance comparison
        dist[2] = dist[0] + 2;
        pq.changeKey(2, -dist[2]);  // Update with negative distance
    }

    // Edge 0->3 with weight 7
    if (dist[3] > dist[0] + 7) {
        dist[3] = dist[0] + 7;
        pq.changeKey(3, -dist[3]);
    }

    // Process vertices in order of shortest distance
    std::vector<int> processed_order;
    std::vector<int> final_distances;

    while (!pq.empty()) {
        int u = pq.removeMax();  // Get vertex with minimum distance (max negative distance)
        processed_order.push_back(u);
        final_distances.push_back(dist[u]);

        // In real Dijkstra, we would relax all edges from u here
        // For this test, we'll just verify the order is correct
    }

    // Vertex 0 should be processed first (distance 0)
    assert(processed_order[0] == 0);
    assert(final_distances[0] == 0);

    // Vertex 2 should be processed second (distance 2)
    assert(processed_order[1] == 2);
    assert(final_distances[1] == 2);

    // Vertex 1 should be processed third (distance 4)
    assert(processed_order[2] == 1);
    assert(final_distances[2] == 4);

    // Vertex 3 should be processed fourth (distance 7)
    assert(processed_order[3] == 3);
    assert(final_distances[3] == 7);

    // Vertex 4 should be processed last (distance infinity/1000)
    assert(processed_order[4] == 4);
    assert(final_distances[4] == 1000);

    std::cout << "PASSED\n";
}

void test_large_dataset_performance() {
    std::cout << "Testing large dataset performance... ";

    const int N = 1000;  // Smaller for faster testing
    IndexMaxPQ<int> pq(N);

    auto start = std::chrono::high_resolution_clock::now();

    // Insert all elements
    for (int i = 0; i < N; ++i) {
        pq.insert(i, i);
    }

    // Perform many key changes
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> idx_dis(0, N-1);
    std::uniform_int_distribution<int> key_dis(0, N*2);

    for (int i = 0; i < 100; ++i) {  // Fewer operations for stability
        int idx = idx_dis(gen);
        int new_key = key_dis(gen);
        pq.changeKey(idx, new_key);
    }

    // Remove all elements
    int count = 0;
    while (!pq.empty()) {
        int idx = pq.removeMax();
        count++;
        // Don't try to access key after removal
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    assert(count == N);

    std::cout << "PASSED (took " << duration.count() << "ms)\n";
}

void test_utility_functions() {
    std::cout << "Testing utility functions... ";

    IndexMaxPQ<int> pq(5);

    pq.insert(0, 10);
    pq.insert(2, 30);
    pq.insert(4, 20);

    // Test indices() function
    auto indices = pq.indices();
    assert(indices.size() == 3);

    // Test keys_in_heap_order() function
    auto keys = pq.keys_in_heap_order();
    assert(keys.size() == 3);
    assert(keys[0] == 30);  // Root should have maximum key

    std::cout << "PASSED\n";
}

void test_priority_queue_applications() {
    std::cout << "Testing priority queue applications... ";

    // Event scheduling simulation
    struct Event {
        std::string name;
        double time;

        Event() : name(""), time(0.0) {}
        Event(const std::string& n, double t) : name(n), time(t) {}

        bool operator<(const Event& other) const { return time < other.time; }
        bool operator>(const Event& other) const { return time > other.time; }
        bool operator==(const Event& other) const { return time == other.time; }
        bool operator>=(const Event& other) const { return time >= other.time; }
    };

    IndexMaxPQ<Event> event_pq(10);

    // Schedule events (using negative time for max heap to simulate min heap)
    event_pq.insert(0, Event("Event A", -1.0));
    event_pq.insert(1, Event("Event B", -3.0));
    event_pq.insert(2, Event("Event C", -2.0));

    // Reschedule an event
    event_pq.changeKey(1, Event("Event B", -0.5));

    // Process events in chronological order (earliest first in our negative scheme)
    std::vector<std::string> processed_events;
    while (!event_pq.empty()) {
        int event_id = event_pq.removeMax();
        // In real application, we would process the event here
        processed_events.push_back("Event processed");
    }

    assert(processed_events.size() == 3);

    std::cout << "PASSED\n";
}

void performance_comparison() {
    std::cout << "\n=== Performance Analysis ===\n";

    const int N = 5000;  // Reduced size for stability
    IndexMaxPQ<int> pq(N);

    // Test insertion performance
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        pq.insert(i, i);
    }
    auto insert_time = std::chrono::high_resolution_clock::now();
    auto insert_duration = std::chrono::duration_cast<std::chrono::milliseconds>(insert_time - start);

    // Test changeKey performance
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, N-1);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {  // Reduced operations
        int idx = dis(gen);
        pq.changeKey(idx, dis(gen));
    }
    auto change_time = std::chrono::high_resolution_clock::now();
    auto change_duration = std::chrono::duration_cast<std::chrono::milliseconds>(change_time - start);

    // Test removal performance
    start = std::chrono::high_resolution_clock::now();
    while (!pq.empty()) {
        pq.removeMax();
    }
    auto remove_time = std::chrono::high_resolution_clock::now();
    auto remove_duration = std::chrono::duration_cast<std::chrono::milliseconds>(remove_time - start);

    std::cout << "Insert " << N << " elements: " << insert_duration.count() << "ms\n";
    std::cout << "1000 changeKey operations: " << change_duration.count() << "ms\n";
    std::cout << "Remove all elements: " << remove_duration.count() << "ms\n";
}

int main() {
    std::cout << "=== IndexMaxPQ Test Suite ===\n\n";

    try {
        test_constructor_and_basic_properties();
        test_single_element_operations();
        test_basic_insert_and_max_operations();
        test_change_key_operations();
        test_increase_decrease_key();
        test_remove_arbitrary_element();
        test_edge_cases_and_errors();
        test_different_data_types();
        test_heap_property_maintenance();
        test_dijkstra_simulation();
        test_large_dataset_performance();
        test_utility_functions();
        test_priority_queue_applications();

        std::cout << "\n=== All tests PASSED! ===\n";

        performance_comparison();

    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}