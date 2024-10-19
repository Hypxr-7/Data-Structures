#include <cassert>
#include <cmath>
#include <iostream>
#include <tuple>
#include <map>
#include <vector>
#include "MinPQ.hpp"

/******************************************************************************
 * Constants
*******************************************************************************/

constexpr int N = 100;

/******************************************************************************
 * Helper Functions
*******************************************************************************/

long long cubeSum(int n1, int n2);

/******************************************************************************
 * Operator Overloads
*******************************************************************************/

bool operator<(const std::tuple<long long, int, int>& t1, const std::tuple<long long, int, int>& t2);
bool operator>(const std::tuple<long long, int, int>& t1, const std::tuple<long long, int, int>& t2);
std::ostream& operator<<(std::ostream& os, const std::tuple<long long, int, int>);

/******************************************************************************
 * Tests
*******************************************************************************/

void test_operator_less_than();
void test_operator_greater_than();

/******************************************************************************
 * Main
*******************************************************************************/

int main() {
    MinPQ<std::tuple<long long, int, int>> pq;

    for (size_t i = 0; i < N; ++i) 
        pq.insert(std::make_tuple(cubeSum(i, i), i, i));
    
    std::map<long long, std::vector<std::pair<int, int>>> sumMap;
    
    while (!pq.isEmpty()) {
        auto s = pq.delMin();
        std::cout << s << '\n';
        sumMap[std::get<0>(s)].emplace_back(std::get<1>(s), std::get<2>(s));
        if (std::get<2>(s) < N)
            pq.insert(std::make_tuple(cubeSum(std::get<1>(s), std::get<2>(s) + 1), 
                                    std::get<1>(s), 
                                    std::get<2>(s) + 1));
    }

    for (const auto& [sum, pairs] : sumMap) {
        if (pairs.size() > 1) {
            for (size_t i = 0; i < pairs.size(); ++i) {
                for (size_t j = i + 1; j < pairs.size(); ++j) {
                    std::cout << sum << " = " 
                            << pairs[i].first << "^3 + " 
                            << pairs[i].second << "^3 = " 
                            << pairs[j].first << "^3 + " 
                            << pairs[j].second << "^3\n";
                }
            }
        }
    }

    return 0;
}

/******************************************************************************
 * Function Definitions
*******************************************************************************/

long long cubeSum(int n1, int n2) {
    return std::pow(n1, 3) + std::pow(n2, 3);
}

bool operator<(const std::tuple<long long, int, int> &t1, const std::tuple<long long, int, int> &t2)
{
    return std::get<0>(t1) < std::get<0>(t2);
}

bool operator>(const std::tuple<long long, int, int> &t1, const std::tuple<long long, int, int> &t2) {
    return std::get<0>(t1) > std::get<0>(t2);
}

std::ostream &operator<<(std::ostream &os, const std::tuple<long long, int, int> t) {
    os << std::get<0>(t) << " = " << std::get<1>(t) << "^3 + " << std::get<2>(t) << "^3";
    return os;
}

void test_operator_less_than() {
    std::tuple<long long, int, int> t1 = std::make_tuple(1LL, 2, 3);
    std::tuple<long long, int, int> t2 = std::make_tuple(2LL, 3, 4);
    std::tuple<long long, int, int> t3 = std::make_tuple(1LL, 3, 4);

    assert(t1 < t2); // t1's first element is less than t2's first element
    assert(!(t2 < t1)); // t2's first element is greater than t1's first element
    assert(!(t1 < t3)); // t1's first element is equal to t3's first element
    assert(!(t3 < t1)); // t3's first element is equal to t1's first element
}

void test_operator_greater_than() {
    std::tuple<long long, int, int> t1 = std::make_tuple(1LL, 2, 3);
    std::tuple<long long, int, int> t2 = std::make_tuple(2LL, 3, 4);
    std::tuple<long long, int, int> t3 = std::make_tuple(1LL, 3, 4);

    assert(!(t1 > t2));
    assert(t2 > t1); 
    assert(!(t1 > t3)); 
    assert(!(t3 > t1)); 
}
