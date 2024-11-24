#pragma once

#include <ctime>

class Stopwatch {
private:
    clock_t start;

public:
    Stopwatch() : start(clock()) {}

    void reset() {
        start = clock();
    }

    double elapsedTime() {
        clock_t end = clock();
        return static_cast<double>(end - start) / static_cast<double>(CLOCKS_PER_SEC);
    }
};