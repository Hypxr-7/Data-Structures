#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include "MedianPQ.hpp"


int main() {
    // Generate vector with random values
    std::vector<int> v(11);
    std::mt19937 mt(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 50);
    auto rn = [&](){ return dist(mt); };
    std::generate(v.begin(), v.end(), rn);

    // Pass all values in MedianPQ
    MedianPQ<int> mpq;
    for (const auto i : v) 
        mpq.push(i);

    // Display Vector
    std::sort(v.begin(), v.end());
    std::cout << "Array contents:\n";
    for (const auto i : v) std::cout << i << ' ';
    std::cout << '\n';

    // Median
    std::cout << "Median: " << mpq.getMedian() << '\n';
    
    // Remove median and display the new one
    mpq.removeMedian();
    std::cout << "New Median after Removal: " << mpq.getMedian() << '\n';

    return 0;
}


int randNum() {
    static std::mt19937 mt(std::random_device{}());
    static std::uniform_int_distribution<int> dist(0, 50);
    return dist(mt);
}