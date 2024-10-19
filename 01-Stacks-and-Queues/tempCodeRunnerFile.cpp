// Test case 3: n = 6, m = 4
    result = captureJosephusOutput(6, 4);
    expected = "3 0 4 2 1 5 \n";
    if (result == expected) {
        std::cout << "Test case 3 passed.\n";
    } else {
        std::cout << "Test case 3 failed. Expected: " << expected << " but got: " << result << "\n";
    }