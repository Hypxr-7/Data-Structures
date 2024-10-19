// g++ -std=c++17 -o bin/random_grid_animation RandomGridAnimation.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <raylib.h>

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

void drawGrid(int N, int cellSize) {
    for (int i = 0; i <= N; ++i) {
        DrawLine(i * cellSize, 0, i * cellSize, N * cellSize, LIGHTGRAY);
        DrawLine(0, i * cellSize, N * cellSize, i * cellSize, LIGHTGRAY);
    }
}

void drawConnection(const Connection& conn, int N, int cellSize) {
    int x1 = (conn.p % N) * cellSize + cellSize / 2;
    int y1 = (conn.p / N) * cellSize + cellSize / 2;
    int x2 = (conn.q % N) * cellSize + cellSize / 2;
    int y2 = (conn.q / N) * cellSize + cellSize / 2;
    DrawLine(x1, y1, x2, y2, RED);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <N>" << std::endl;
        return 1;
    }

    int N = std::atoi(argv[1]);
    std::vector<Connection> connections = generate(N);

    const int cellSize = 40;
    const int screenWidth = N * cellSize;
    const int screenHeight = N * cellSize;

    InitWindow(screenWidth, screenHeight, "Random Grid Animation");

    SetTargetFPS(10); // Set the frame rate for the animation

    int currentConnection = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        drawGrid(N, cellSize);

        for (int i = 0; i <= currentConnection && i < connections.size(); ++i) {
            drawConnection(connections[i], N, cellSize);
        }

        if (currentConnection < connections.size()) {
            ++currentConnection;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}