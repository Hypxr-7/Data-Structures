/*
Write a client that creates a symbol table mapping letter grades to numerical
scores, as in the table below, then reads from standard input a list of letter grades and
computes and prints the GPA (the average of the numbers corresponding to the grades).
A+   A    A-   B+   B    B-   C+   C    C-    D   F
4.33 4.00 3.67 3.33 3.00 2.67 2.33 2.00 1.67 1.00 0.00
*/

#include <iostream>
#include <iomanip>
#include <string_view>
#include "BinarySearchST.hpp"

double getGPA(std::string_view grade);

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "Usage: " << argv[0] << " <Grade_List>" << std::endl;
        return 1;
    }

    double sum = 0;
    for (int i = 1; i < argc; ++i) {
        sum += getGPA(argv[i]);
    }

    std::cout << "GPA: " << std::setprecision(3) << sum / static_cast<double>(argc - 1) << std::endl;  

    return 0;
}

double getGPA(std::string_view grade) {
    if (grade == "A+") return 4.33;
    if (grade == "A" ) return 4.00;
    if (grade == "A-") return 3.67;
    if (grade == "B+") return 3.33;
    if (grade == "B" ) return 3.00;
    if (grade == "B-") return 2.67;
    if (grade == "C+") return 2.33;
    if (grade == "C" ) return 2.00;
    if (grade == "C-") return 1.67;
    if (grade == "D" ) return 1.00;
    if (grade == "F" ) return 0.00;
    throw std::invalid_argument("argument to getGPA() is not a grade");
}
