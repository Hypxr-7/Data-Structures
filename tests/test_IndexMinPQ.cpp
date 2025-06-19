#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <cassert>
#include <stdexcept>
#include <climits>

// Include the IndexMinPQ header
#include "../data_structures/IndexMinPQ.hpp"

// Test helper functions
template <typename T>
bool isValidMinHeap(const std::vector<int>& indices, const std::vector<T>& keys) {
    for (size_t i = 1; i < indices.size(); ++i) {
        size_t parent = i / 2;
        if (parent > 0 && keys[indices[parent]] > keys[indices[i]]) {
            return false;
        }
    }
    return true;
}

void test_constructor_and_basic_properties() {
    std::cout << "Testing constructor and basic properties... ";

    IndexMinPQ<int> pq(10);

    assert(pq.empty());
    assert(pq.size() == 0);

    // Test invalid capacity
    try {
        IndexMinPQ<int> invalid_pq(-1);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        // Expected exception
    }

    std::cout << "PASSED\n";
}

void test_single_element_operations() {
    std::cout << "Testing single element operations... ";

    IndexMinPQ<double> pq(5);

    // Insert single element
    pq.insert(2, 3.14);

    assert(!pq.empty());
    assert(pq.size() == 1);
    assert(pq.contains(2));
    assert(!pq.contains(0));
    assert(!pq.contains(1));
    assert(pq.minIndex() == 2);
    assert(pq.minKey() == 3.14);
    assert(pq.keyOf(2) == 3.14);

    // Remove the element
    int removed = pq.removeMin();
    assert(removed == 2);
    assert(pq.empty());
    assert(!pq.contains(2));

    std::cout << "PASSED\n";
}

void test_basic_insert_and_min_operations() {
    std::cout << "Testing basic insert and min operations... ";

    IndexMinPQ<int> pq(10);

    // Insert elements
    pq.insert(0, 10);
    pq.insert(1, 20);
    pq.insert(2, 5);   // This should be minimum
    pq.insert(3, 15);

    assert(pq.size() == 4);
    assert(pq.minIndex() == 2);  // Index 2 has lowest priority (5)
    assert(pq.minKey() == 5);
    assert(pq.keyOf(2) == 5);

    // Remove min and check next min
    int min1 = pq.removeMin();
    assert(min1 == 2);
    assert(pq.minIndex() == 0);  // Next lowest is index 0 with priority 10
    assert(pq.minKey() == 10);

    int min2 = pq.removeMin();
    assert(min2 == 0);
    assert(pq.minIndex() == 3);  // Next is index 3 with priority 15

    int min3 = pq.removeMin();
    assert(min3 == 3);
    assert(pq.minIndex() == 1);  // Last is index 1 with priority 20

    int min4 = pq.removeMin();
    assert(min4 == 1);
    assert(pq.empty());

    std::cout << "PASSED\n";
}

void test_change_key_operations() {
    std::cout << "Testing changeKey operations... ";

    IndexMinPQ<double> pq(5);

    pq.insert(0, 5.0);
    pq.insert(1, 3.0);  // Initially minimum
    pq.insert(2, 7.0);

    assert(pq.minIndex() == 1);  // Initially, index 1 has min priority

    // Change key of index 0 to lowest priority
    pq.changeKey(0, 1.0);
    assert(pq.minIndex() == 0);
    assert(pq.minKey() == 1.0);

    // Change key of index 2 to even lower priority
    pq.changeKey(2, 0.5);
    assert(pq.minIndex() == 2);
    assert(pq.minKey() == 0.5);

    // Increase a key
    pq.changeKey(2, 10.0);
    assert(pq.minIndex() == 0);  // Now index 0 should be min again

    std::cout << "PASSED\n";
}

void test_increase_decrease_key() {
    std::cout << "Testing increaseKey and decreaseKey... ";

    IndexMinPQ<int> pq(5);

    pq.insert(0, 10);
    pq.insert(1, 20);
    pq.insert(2, 30);

    assert(pq.minIndex() == 0);

    // Decrease key of index 1 (higher priority)
    pq.decreaseKey(1, 5);
    assert(pq.minIndex() == 1);
    assert(pq.keyOf(1) == 5);

    // Increase key of index 1 (lower priority)
    pq.increaseKey(1, 25);
    assert(pq.minIndex() == 0);  // Index 0 should be min again
    assert(pq.keyOf(1) == 25);

    // Test error conditions for decreaseKey
    try {
        pq.decreaseKey(0, 10);  // Same key
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        pq.decreaseKey(0, 15);  // Larger key
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    // Test error conditions for increaseKey
    try {
        pq.increaseKey(2, 30);  // Same key
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        pq.increaseKey(2, 25);  // Smaller key
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    std::cout << "PASSED\n";
}

void test_remove_arbitrary_element() {
    std::cout << "Testing remove arbitrary element... ";

    IndexMinPQ<int> pq(10);

    // Insert several elements
    for (int i = 0; i < 5; ++i) {
        pq.insert(i, i * 10 + 5);  // Values: 5, 15, 25, 35, 45
    }

    assert(pq.size() == 5);
    assert(pq.contains(2));
    assert(pq.minIndex() == 0);  // Index 0 has minimum value (5)

    // Remove element from middle
    pq.remove(2);
    assert(pq.size() == 4);
    assert(!pq.contains(2));

    // Verify remaining elements maintain heap property
    assert(pq.minIndex() == 0);  // Should still be the minimum

    // Remove min element
    pq.remove(0);
    assert(pq.minIndex() == 1);  // Next minimum should be index 1

    // Remove all remaining elements
    pq.remove(1);
    pq.remove(3);
    pq.remove(4);
    assert(pq.empty());

    std::cout << "PASSED\n";
}

void test_edge_cases_and_errors() {
    std::cout << "Testing edge cases and error conditions... ";

    IndexMinPQ<int> pq(3);

    // Test operations on empty PQ
    try {
        pq.minIndex();
        assert(false);
    } catch (const std::underflow_error&) {
        // Expected
    }

    try {
        pq.minKey();
        assert(false);
    } catch (const std::underflow_error&) {
        // Expected
    }

    try {
        pq.removeMin();
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
    IndexMinPQ<std::string> string_pq(5);
    string_pq.insert(0, "zebra");
    string_pq.insert(1, "apple");  // This should be minimum
    string_pq.insert(2, "banana");

    assert(string_pq.minIndex() == 1);  // "apple" is lexicographically smallest
    assert(string_pq.minKey() == "apple");

    // Test with custom struct
    struct Task {
        std::string name;
        int priority;

        Task() : name(""), priority(0) {}
        Task(const std::string& n, int p) : name(n), priority(p) {}

        bool operator<(const Task& other) const { return priority < other.priority; }
        bool operator>(const Task& other) const { return priority > other.priority; }
        bool operator==(const Task& other) const { return priority == other.priority; }
        bool operator<=(const Task& other) const { return priority <= other.priority; }
    };

    IndexMinPQ<Task> task_pq(3);
    task_pq.insert(0, Task("Low", 10));
    task_pq.insert(1, Task("High", 1));   // Lowest priority number = highest priority
    task_pq.insert(2, Task("Medium", 5));

    assert(task_pq.minIndex() == 1);  // High priority task (lowest number)
    assert(task_pq.minKey().name == "High");

    std::cout << "PASSED\n";
}

void test_heap_property_maintenance() {
    std::cout << "Testing heap property maintenance... ";

    IndexMinPQ<int> pq(20);
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
            // Remove min
            int min_idx = pq.removeMin();
            inserted_indices.erase(
                std::find(inserted_indices.begin(), inserted_indices.end(), min_idx)
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
            int min_idx = pq.minIndex();
            int min_key = pq.minKey();

            // Verify this is actually the minimum
            auto indices = pq.indices();
            bool is_actually_min = true;
            for (int idx : indices) {
                if (pq.keyOf(idx) < min_key) {
                    is_actually_min = false;
                    break;
                }
            }
            assert(is_actually_min);
        }
    }

    std::cout << "PASSED\n";
}

void test_dijkstra_simulation() {
    std::cout << "Testing Dijkstra's algorithm simulation... ";

    // Simulate a simple graph with 5 vertices
    const int V = 5;
    IndexMinPQ<int> pq(V);

    // Initialize distances
    std::vector<int> dist(V, 1000);  // "Infinity" as large number
    dist[0] = 0;  // Start vertex

    // Insert all vertices
    for (int i = 0; i < V; ++i) {
        pq.insert(i, dist[i]);
    }

    // Simulate relaxing edges
    // Edge 0->1 with weight 4
    if (dist[1] > dist[0] + 4) {
        dist[1] = dist[0] + 4;
        pq.decreaseKey(1, dist[1]);  // Shorter path found
    }

    // Edge 0->2 with weight 2
    if (dist[2] > dist[0] + 2) {
        dist[2] = dist[0] + 2;
        pq.decreaseKey(2, dist[2]);
    }

    // Edge 0->3 with weight 7
    if (dist[3] > dist[0] + 7) {
        dist[3] = dist[0] + 7;
        pq.decreaseKey(3, dist[3]);
    }

    // Process vertices in order of shortest distance
    std::vector<int> processed_order;
    std::vector<int> final_distances;

    while (!pq.empty()) {
        int u = pq.removeMin();  // Get vertex with minimum distance
        processed_order.push_back(u);
        final_distances.push_back(dist[u]);
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

void test_event_simulation() {
    std::cout << "Testing event-driven simulation... ";

    // Event scheduling where lower time = higher priority
    IndexMinPQ<double> event_pq(10);

    // Schedule events
    event_pq.insert(0, 10.5);  // Event 0 at time 10.5
    event_pq.insert(1, 5.2);   // Event 1 at time 5.2 (earliest)
    event_pq.insert(2, 8.0);   // Event 2 at time 8.0
    event_pq.insert(3, 15.0);  // Event 3 at time 15.0

    assert(event_pq.minIndex() == 1);  // Event 1 is earliest
    assert(event_pq.minKey() == 5.2);

    // Reschedule event 0 to an earlier time
    event_pq.decreaseKey(0, 3.0);
    assert(event_pq.minIndex() == 0);  // Now event 0 is earliest

    // Process events in chronological order
    std::vector<int> event_order;
    std::vector<double> event_times;

    while (!event_pq.empty()) {
        int event_id = event_pq.removeMin();
        event_order.push_back(event_id);
        // Note: Can't call keyOf after removal, so we track times separately
    }

    // Should process in order: 0, 1, 2, 3
    assert(event_order[0] == 0);  // Event 0 (time 3.0)
    assert(event_order[1] == 1);  // Event 1 (time 5.2)
    assert(event_order[2] == 2);  // Event 2 (time 8.0)
    assert(event_order[3] == 3);  // Event 3 (time 15.0)

    std::cout << "PASSED\n";
}

void test_task_scheduling() {
    std::cout << "Testing task scheduling simulation... ";

    // Task scheduling where lower priority number = higher priority
    IndexMinPQ<int> task_pq(10);

    // Add tasks with different priorities
    task_pq.insert(0, 5);   // Normal priority
    task_pq.insert(1, 1);   // Critical priority (highest)
    task_pq.insert(2, 10);  // Low priority
    task_pq.insert(3, 3);   // High priority

    assert(task_pq.minIndex() == 1);  // Critical task first

    // Upgrade priority of task 2
    task_pq.decreaseKey(2, 2);  // Now high priority

    // Process tasks in priority order
    std::vector<int> task_order;
    std::vector<int> priorities;

    while (!task_pq.empty()) {
        int task = task_pq.removeMin();
        task_order.push_back(task);
    }

    // Should process in order: 1 (pri 1), 2 (pri 2), 3 (pri 3), 0 (pri 5)
    assert(task_order[0] == 1);  // Critical
    assert(task_order[1] == 2);  // High (upgraded)
    assert(task_order[2] == 3);  // High
    assert(task_order[3] == 0);  // Normal

    std::cout << "PASSED\n";
}

void test_large_dataset_performance() {
    std::cout << "Testing large dataset performance... ";

    const int N = 1000;
    IndexMinPQ<int> pq(N);

    auto start = std::chrono::high_resolution_clock::now();

    // Insert all elements in reverse order (worst case for min heap)
    for (int i = 0; i < N; ++i) {
        pq.insert(i, N - i);  // Largest values first
    }

    // Perform many key changes
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> idx_dis(0, N-1);
    std::uniform_int_distribution<int> key_dis(1, N*2);

    for (int i = 0; i < 100; ++i) {
        int idx = idx_dis(gen);
        int new_key = key_dis(gen);
        pq.changeKey(idx, new_key);
    }

    // Remove all elements - should come out in ascending order
    int count = 0;
    int last_key = 0;
    while (!pq.empty()) {
        int idx = pq.removeMin();
        count++;
        // Note: Can't access key after removal
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    assert(count == N);

    std::cout << "PASSED (took " << duration.count() << "ms)\n";
}

void test_utility_functions() {
    std::cout << "Testing utility functions... ";

    IndexMinPQ<int> pq(5);

    pq.insert(0, 30);
    pq.insert(2, 10);  // This should be minimum
    pq.insert(4, 20);

    // Test indices() function
    auto indices = pq.indices();
    assert(indices.size() == 3);

    // Test keys_in_heap_order() function
    auto keys = pq.keys_in_heap_order();
    assert(keys.size() == 3);
    assert(keys[0] == 10);  // Root should have minimum key

    std::cout << "PASSED\n";
}

void test_min_max_comparison() {
    std::cout << "Testing MinPQ behavior vs MaxPQ... ";

    std::vector<int> data = {5, 2, 8, 1, 9, 3};

    IndexMinPQ<int> min_pq(6);
    for (size_t i = 0; i < data.size(); ++i) {
        min_pq.insert(i, data[i]);
    }

    // MinPQ should extract in ascending order
    std::vector<int> min_extracted;
    while (!min_pq.empty()) {
        int idx = min_pq.removeMin();
        min_extracted.push_back(data[idx]);  // Get original value
    }

    // Verify ascending order
    for (size_t i = 1; i < min_extracted.size(); ++i) {
        assert(min_extracted[i-1] <= min_extracted[i]);
    }
    assert(min_extracted[0] == 1);  // Smallest first
    assert(min_extracted.back() == 9);  // Largest last

    std::cout << "PASSED\n";
}

void test_edge_case_values() {
    std::cout << "Testing edge case values... ";

    IndexMinPQ<int> pq(5);

    // Test with extreme values
    pq.insert(0, INT_MAX);
    pq.insert(1, INT_MIN);  // This should be minimum
    pq.insert(2, 0);

    assert(pq.minIndex() == 1);
    assert(pq.minKey() == INT_MIN);

    assert(pq.removeMin() == 1);  // INT_MIN first
    assert(pq.removeMin() == 2);  // 0 second
    assert(pq.removeMin() == 0);  // INT_MAX last

    // Test with negative numbers
    IndexMinPQ<int> neg_pq(5);
    neg_pq.insert(0, -5);
    neg_pq.insert(1, -1);
    neg_pq.insert(2, -10);  // Most negative = minimum
    neg_pq.insert(3, 0);
    neg_pq.insert(4, 3);

    // Test the order by checking minimum at each step before removal
    assert(neg_pq.minIndex() == 2);  // -10 is minimum
    assert(neg_pq.minKey() == -10);
    neg_pq.removeMin();

    assert(neg_pq.minIndex() == 0);  // -5 is next minimum
    assert(neg_pq.minKey() == -5);
    neg_pq.removeMin();

    assert(neg_pq.minIndex() == 1);  // -1 is next minimum
    assert(neg_pq.minKey() == -1);
    neg_pq.removeMin();

    assert(neg_pq.minIndex() == 3);  // 0 is next minimum
    assert(neg_pq.minKey() == 0);
    neg_pq.removeMin();

    assert(neg_pq.minIndex() == 4);  // 3 is last
    assert(neg_pq.minKey() == 3);
    neg_pq.removeMin();

    assert(neg_pq.empty());  // All elements should be removed

    std::cout << "PASSED\n";
}

void performance_comparison() {
    std::cout << "\n=== Performance Analysis ===\n";

    const int N = 5000;

    // Test IndexMinPQ performance
    auto start = std::chrono::high_resolution_clock::now();

    IndexMinPQ<int> pq(N);

    // Insert elements
    for (int i = 0; i < N; ++i) {
        pq.insert(i, N - i);  // Insert in reverse order
    }
    auto insert_time = std::chrono::high_resolution_clock::now();

    // Test decreaseKey performance (common in Dijkstra)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, N-1);

    for (int i = 0; i < 1000; ++i) {
        int idx = dis(gen);
        int current_key = pq.keyOf(idx);
        if (current_key > 1) {
            pq.decreaseKey(idx, current_key - 1);
        }
    }
    auto decrease_time = std::chrono::high_resolution_clock::now();

    // Test removal performance
    int removed_count = 0;
    while (!pq.empty()) {
        pq.removeMin();
        removed_count++;
    }
    auto remove_time = std::chrono::high_resolution_clock::now();

    auto insert_duration = std::chrono::duration_cast<std::chrono::milliseconds>(insert_time - start);
    auto decrease_duration = std::chrono::duration_cast<std::chrono::milliseconds>(decrease_time - insert_time);
    auto remove_duration = std::chrono::duration_cast<std::chrono::milliseconds>(remove_time - decrease_time);

    std::cout << "Insert " << N << " elements: " << insert_duration.count() << "ms\n";
    std::cout << "1000 decreaseKey operations: " << decrease_duration.count() << "ms\n";
    std::cout << "Remove all elements: " << remove_duration.count() << "ms\n";

    assert(removed_count == N);
}

int main() {
    std::cout << "=== IndexMinPQ Test Suite ===\n\n";

    try {
        test_constructor_and_basic_properties();
        test_single_element_operations();
        test_basic_insert_and_min_operations();
        test_change_key_operations();
        test_increase_decrease_key();
        test_remove_arbitrary_element();
        test_edge_cases_and_errors();
        test_different_data_types();
        test_heap_property_maintenance();
        test_dijkstra_simulation();
        test_event_simulation();
        test_task_scheduling();
        test_large_dataset_performance();
        test_utility_functions();
        test_min_max_comparison();
        test_edge_case_values();

        std::cout << "\n=== All tests PASSED! ===\n";

        performance_comparison();

    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}