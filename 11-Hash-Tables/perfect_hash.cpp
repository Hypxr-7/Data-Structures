/*
 * Write a program to find values of a and M, with M as small as possible, such that
 * the hash function (a * k) % M for transforming the kth letter of the alphabet into a
 * table index produces distinct values (no collisions) for the keys S E A R C H X M P L.
 * The result is known as a perfect hash function.
*/

#include <cassert>
#include <format>
#include <iostream>
#include <set>
#include <string_view>

#include "SeparateChainingHashST.hpp"

bool isPerfectHash(const int a, const int M, const std::set<char>& keys);
std::pair<int, int> findPerfectHash(const std::set<char>& keys);

int main() {
    std::set<char> keys;
    for (const char c : std::string_view{"SEARCHXMPL"})
        keys.insert(c);

    auto [a, M] = findPerfectHash(keys);
    assert(isPerfectHash(a, M, keys));

    std::cout << std::format("Perfect hash function found: a = {}, M = {}\n", a, M);

    return 0;
}

bool isPerfectHash(const int a, const int M, const std::set<char>& keys) {
    std::set<int> seen;
    for (const char c : keys) {
        int hashValue = (a * c) % M;
        if (seen.contains(hashValue))
            return false;
        seen.insert(hashValue);
    }
    return true;
}

std::pair<int, int> findPerfectHash(const std::set<char>& keys) {
    int M = keys.size();
    while (true) {
        for (int a = 1; a < M; ++a)
            if (isPerfectHash(a, M, keys))
                return std::pair<int, int>(a, M);
        ++M;
    }
}
