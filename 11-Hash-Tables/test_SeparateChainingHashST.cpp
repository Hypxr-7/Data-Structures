#include <cassert>
#include <iostream>

#include "SeparateChainingHashST.hpp"

// UDT
// struct S {
//     std::string first_name;
//     std::string last_name;

//     // Required for UDT hashing
//     bool operator==(const S& other) const {
//         if (this->first_name != other.first_name) return false;
//         if (this->last_name != other.last_name) return false;
//         return true;
//     }
// };

// std::hash specialization
// template <>
// struct std::hash<S> {
//     std::size_t operator()(const S& s) const noexcept {
//         std::size_t h1 = std::hash<std::string>{}(s.first_name);
//         std::size_t h2 = std::hash<std::string>{}(s.last_name);
//         return h1 ^ (h2 << 1);  // or use boost::hash_combine
//     }
// };

void test();

int main() {
    test();
    std::cout << "All tests passed\n";

    return 0;
}

void test() {
    SeparateChainingHashST<std::string, int> ht;

    assert(ht.isEmpty());
    assert(ht.size() == 0);
    assert(!ht.contains("foo"));

    try {
        ht.get("foo");
        assert(false);
    } catch (const std::out_of_range& e) {
        // blank
    } catch (...) {
        assert(false);
    }

    ht.put("foo", 1);
    assert(ht.size() == 1);
    assert(ht.contains("foo"));
    assert(ht.get("foo") == 1);

    ht.put("bar", 2);
    assert(ht.size() == 2);
    assert(ht.contains("bar"));
    assert(ht.get("bar") == 2);

    ht.put("foo", 3);
    assert(ht.size() == 2);
    assert(ht.get("foo") == 3);

    ht.deleteKey("foo");
    assert(ht.size() == 1);
    assert(!ht.contains("foo"));

    ht.deleteKey("bar");
    assert(ht.isEmpty());
    assert(ht.size() == 0);
    assert(!ht.contains("bar"));

    try {
        ht.get("bar");
        assert(false);
    } catch (const std::out_of_range& e) {
        // blank
    } catch (...) {
        assert(false);
    }

    // Additional tests
    // SeparateChainingHashST<std::string, int> ht2;

    // // Test with multiple keys and values
    // ht2.put("apple", 10);
    // ht2.put("banana", 20);
    // ht2.put("cherry", 30);
    // ht2.put("date", 40);
    // ht2.put("elderberry", 50);

    // assert(ht2.size() == 5);
    // assert(ht2.contains("apple"));
    // assert(ht2.contains("banana"));
    // assert(ht2.contains("cherry"));
    // assert(ht2.contains("date"));
    // assert(ht2.contains("elderberry"));

    // assert(ht2.get("apple") == 10);
    // assert(ht2.get("banana") == 20);
    // assert(ht2.get("cherry") == 30);
    // assert(ht2.get("date") == 40);
    // assert(ht2.get("elderberry") == 50);

    // // Test updating values
    // ht2.put("apple", 15);
    // assert(ht2.get("apple") == 15);

    // // Test deleting keys
    // ht2.deleteKey("banana");
    // assert(!ht2.contains("banana"));
    // assert(ht2.size() == 4);

    // ht2.deleteKey("cherry");
    // assert(!ht2.contains("cherry"));
    // assert(ht2.size() == 3);

    // // Test key() method
    // std::vector<std::string> keys = ht2.keys();
    // std::vector<std::string> expectedKeys = {"apple", "date", "elderberry"};
    // assert(keys.size() == expectedKeys.size());
    // for (const auto& key : expectedKeys) {
    //     assert(std::find(keys.begin(), keys.end(), key) != keys.end());
    // }

    // // Test clearing the hash table
    // ht2.deleteKey("apple");
    // ht2.deleteKey("date");
    // ht2.deleteKey("elderberry");
    // assert(ht2.isEmpty());
    // assert(ht2.size() == 0);

    // // Test with user-defined type S
    // SeparateChainingHashST<S, int> ht3;

    // S person1 = {"John", "Doe"};
    // S person2 = {"Jane", "Doe"};
    // S person3 = {"Alice", "Smith"};
    // S person4 = {"Bob", "Johnson"};

    // ht3.put(person1, 100);
    // ht3.put(person2, 200);
    // ht3.put(person3, 300);
    // ht3.put(person4, 400);

    // assert(ht3.size() == 4);
    // assert(ht3.contains(person1));
    // assert(ht3.contains(person2));
    // assert(ht3.contains(person3));
    // assert(ht3.contains(person4));

    // assert(ht3.get(person1) == 100);
    // assert(ht3.get(person2) == 200);
    // assert(ht3.get(person3) == 300);
    // assert(ht3.get(person4) == 400);

    // // Test updating values
    // ht3.put(person1, 150);
    // assert(ht3.get(person1) == 150);

    // // Test deleting keys
    // ht3.deleteKey(person2);
    // assert(!ht3.contains(person2));
    // assert(ht3.size() == 3);

    // ht3.deleteKey(person3);
    // assert(!ht3.contains(person3));
    // assert(ht3.size() == 2);

    // // Test key() method
    // std::vector<S> keysS = ht3.keys();
    // std::vector<S> expectedKeysS = {person1, person4};
    // assert(keysS.size() == expectedKeysS.size());
    // for (const auto& key : expectedKeysS) {
    //     assert(std::find(keysS.begin(), keysS.end(), key) != keysS.end());
    // }

    // // Test clearing the hash table
    // ht3.deleteKey(person1);
    // ht3.deleteKey(person4);
    // assert(ht3.isEmpty());
    // assert(ht3.size() == 0);
}