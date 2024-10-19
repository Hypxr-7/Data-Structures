/*
Random connections. Develop a UF client ErdosRenyi that takes an integer
value N from the command line, generates random pairs of integers between 0 and N-1,
calling connected() to determine if they are connected and then merge() if not, 
looping until all sites are connected, and printing the number
of connections generated. Package your program with a function  count() that takes
N as argument and returns the number of connections and a main() that takes N from
the command line, calls count(), and prints the returned value.
*/

#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include "WeightedQuickUnionPathCompressionUF.hpp"

int count(int n);

int main(int argc, char** argv) {
    if (argc < 2 || argc > 2) {
        std::cerr << "Usage: " << argv[0] << " <N>\n";
        return 1;
    }

    int n;
    try {
        n = std::stoi(argv[1]);
    } catch(const std::invalid_argument& e) {
        std::cerr << "Argument is not a number\n";
        return 1;
    }

    if (n < 0) {
        std::cerr << "Number must be positive\n";
        return 1;
    }

    std::cout << "Connections: " << count(n) << std::endl;

    return 0;
}

int count(int n) {
    WeightedQuickUnionPathCompressionUF uf(n);
    std::mt19937 mt(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, n-1);
    int connections = 0;
    
    while (true) {
        int p = dist(mt);
        int q = dist(mt);
        if (!uf.connected(p, q)) {
            uf.merge(p, q);
            ++connections;
        }

        bool allConnected = true;
        int root = uf.find(0);
        for (int i = 0; i < n; ++i) {
            if (uf.find(i) != root) {
                allConnected = false;
                break;
            }
        }
        if (allConnected) break;
    }

    return connections;
}
