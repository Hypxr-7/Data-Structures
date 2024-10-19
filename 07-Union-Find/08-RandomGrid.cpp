/*
Random grid generator. Write a program RandomGrid that takes an int value
N from the command line, generates all the connections in an N-by-N grid, puts them
in random order, randomly orients them (so that p q and q p are equally likely to oc-
cur), and prints the result to standard output. To randomly order the connections, use
a RandomBag (see Exercise 1.3.34 on page 167). To encapsulate p and q in a single object,
use the Connection nested class shown below. Package your program as two static
methods: generate(), which takes N as argument and returns an array of connec-
tions, and main(), which takes N from the command line, calls generate(), and iterates
through the returned array to print the connections.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

class Connection {
public:
    int p, q;

    Connection(int p, int q) : p(p), q(q) {}
};

template <typename T>
class RandomBag {
private:
    std::vector<T> items;

public:
    void add(const T& item) {
        items.push_back(item);
    }

    std::vector<T> getItems() {
        std::shuffle(items.begin(), items.end(), std::default_random_engine(std::time(0)));
        return items;
    }
};

std::vector<Connection> generate(int N) {
    RandomBag<Connection> bag;

    // Generate all connections in an N-by-N grid
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i < N - 1) bag.add(Connection(i * N + j, (i + 1) * N + j)); // Vertical connection
            if (j < N - 1) bag.add(Connection(i * N + j, i * N + j + 1));   // Horizontal connection
        }
    }

    // Get all connections in random order
    std::vector<Connection> connections = bag.getItems();

    // Randomly orient the connections
    std::default_random_engine generator(std::random_device{}());
    std::bernoulli_distribution flip(0.5);
    for (auto& conn : connections) {
        if (flip(generator) == 1) {
            std::swap(conn.p, conn.q);
        }
    }

    return connections;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <N>" << std::endl;
        return 1;
    }

    int N = std::atoi(argv[1]);
    std::vector<Connection> connections = generate(N);

    for (const auto& conn : connections) {
        std::cout << conn.p << " " << conn.q << std::endl;
    }
}
