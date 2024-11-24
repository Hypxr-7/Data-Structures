#include <iostream>
#include <random>
#include <thread>
#include <fstream>

#include "Percolation.hpp"
#include "raylib.h"

void DrawGrid(Percolation& perc, int n, int cellSize) {
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            int x = col * cellSize;
            int y = row * cellSize;
            if (perc.isOpen(row, col))
                if (perc.isFull(row, col))
                    DrawRectangle(x, y, cellSize, cellSize, BLUE);
                else
                    DrawRectangle(x, y, cellSize, cellSize, WHITE);
            else
                DrawRectangle(x, y, cellSize, cellSize, BLACK);
            
            // DrawRectangleLines(x, y, cellSize, cellSize, GRAY);
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << ' ' << "<FileName>" << std::endl;
        return 1;
    }

    std::ifstream fileHandle(argv[1]);
    int n;
    fileHandle >> n;

    int screenSize = 900;
    const int cellSize = screenSize / n;
    Percolation perc(n);
    screenSize = cellSize * n;

    InitWindow(screenSize, screenSize, "Percolation Animation");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (!perc.percolates()) {
            int row;
            int col;
            fileHandle >> row >> col;
            if (!perc.isOpen(row, col)) {
                perc.open(row, col);
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGrid(perc, n, cellSize);
        EndDrawing();

        // std::this_thread::sleep_for(std::chrono::milliseconds(50));  // Slow down the animation
    }

    CloseWindow();
    fileHandle.close();

    return 0;
}

// g++ -std=c++17 -o bin/Animation Animation.cpp -lraylib -lm -lpthread -ldl -lrt -lX11