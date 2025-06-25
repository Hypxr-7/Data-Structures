#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../data_structures/Alphabet.hpp"

void testBasicConstruction() {
    // Test construction with simple alphabet
    Alphabet simple("abc");

    assert(simple.radix() == 3);
    assert(simple.contains('a'));
    assert(simple.contains('b'));
    assert(simple.contains('c'));
    assert(!simple.contains('d'));
    assert(!simple.contains('A'));

    std::cout << "testBasicConstruction passed\n";
}

void testDuplicateCharacters() {
    // Test that duplicate characters throw exception
    try {
        Alphabet invalid("abca");  // 'a' appears twice
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("repeated character") != std::string::npos);
    }

    try {
        Alphabet invalid("1122");  // '1' and '2' each appear twice
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("repeated character") != std::string::npos);
    }

    std::cout << "testDuplicateCharacters passed\n";
}

void testRadixConstructor() {
    // Test construction with radix
    Alphabet ascii128(128);

    assert(ascii128.radix() == 128);

    // Characters 0-127 should be present
    for (int i = 0; i < 128; ++i) {
        char c = static_cast<char>(i);
        assert(ascii128.contains(c));
        assert(ascii128.toIndex(c) == i);
        assert(ascii128.toChar(i) == c);
    }

    // Character 128 should not be present
    assert(!ascii128.contains(static_cast<char>(128)));

    std::cout << "testRadixConstructor passed\n";
}

void testInvalidRadixConstructor() {
    // Test invalid radix values
    try {
        Alphabet invalid(-1);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("non-negative") != std::string::npos);
    }

    try {
        Alphabet tooLarge(300);  // Exceeds MAX_CHAR
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("too large") != std::string::npos);
    }

    std::cout << "testInvalidRadixConstructor passed\n";
}

void testDefaultConstructor() {
    // Test default constructor (extended ASCII)
    Alphabet defaultAlpha;

    assert(defaultAlpha.radix() == 256);

    // All 8-bit characters should be present
    for (int i = 0; i < 256; ++i) {
        char c = static_cast<char>(i);
        assert(defaultAlpha.contains(c));
        assert(defaultAlpha.toIndex(c) == i);
        assert(defaultAlpha.toChar(i) == c);
    }

    std::cout << "testDefaultConstructor passed\n";
}

void testPredefinedAlphabets() {
    // Test BINARY alphabet
    assert(Alphabet::BINARY.radix() == 2);
    assert(Alphabet::BINARY.contains('0'));
    assert(Alphabet::BINARY.contains('1'));
    assert(!Alphabet::BINARY.contains('2'));
    assert(Alphabet::BINARY.toIndex('0') == 0);
    assert(Alphabet::BINARY.toIndex('1') == 1);

    // Test DNA alphabet
    assert(Alphabet::DNA.radix() == 4);
    assert(Alphabet::DNA.contains('A'));
    assert(Alphabet::DNA.contains('C'));
    assert(Alphabet::DNA.contains('G'));
    assert(Alphabet::DNA.contains('T'));
    assert(!Alphabet::DNA.contains('N'));

    // Test DECIMAL alphabet
    assert(Alphabet::DECIMAL.radix() == 10);
    for (char c = '0'; c <= '9'; ++c) {
        assert(Alphabet::DECIMAL.contains(c));
    }
    assert(!Alphabet::DECIMAL.contains('A'));

    // Test HEXADECIMAL alphabet
    assert(Alphabet::HEXADECIMAL.radix() == 16);
    for (char c = '0'; c <= '9'; ++c) {
        assert(Alphabet::HEXADECIMAL.contains(c));
    }
    for (char c = 'A'; c <= 'F'; ++c) {
        assert(Alphabet::HEXADECIMAL.contains(c));
    }
    assert(!Alphabet::HEXADECIMAL.contains('G'));

    std::cout << "testPredefinedAlphabets passed\n";
}

void testToIndexValidCharacters() {
    Alphabet alpha("xyz123");

    assert(alpha.toIndex('x') == 0);
    assert(alpha.toIndex('y') == 1);
    assert(alpha.toIndex('z') == 2);
    assert(alpha.toIndex('1') == 3);
    assert(alpha.toIndex('2') == 4);
    assert(alpha.toIndex('3') == 5);

    std::cout << "testToIndexValidCharacters passed\n";
}

void testToIndexInvalidCharacters() {
    Alphabet alpha("abc");

    try {
        alpha.toIndex('d');
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("not in alphabet") != std::string::npos);
    }

    try {
        alpha.toIndex('A');  // Wrong case
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("not in alphabet") != std::string::npos);
    }

    std::cout << "testToIndexInvalidCharacters passed\n";
}

void testToCharValidIndices() {
    Alphabet alpha("xyz123");

    assert(alpha.toChar(0) == 'x');
    assert(alpha.toChar(1) == 'y');
    assert(alpha.toChar(2) == 'z');
    assert(alpha.toChar(3) == '1');
    assert(alpha.toChar(4) == '2');
    assert(alpha.toChar(5) == '3');

    std::cout << "testToCharValidIndices passed\n";
}

void testToCharInvalidIndices() {
    Alphabet alpha("abc");

    try {
        alpha.toChar(-1);
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("must be between 0 and") != std::string::npos);
    }

    try {
        alpha.toChar(3);  // Out of range
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("must be between 0 and") != std::string::npos);
    }

    std::cout << "testToCharInvalidIndices passed\n";
}

void testToIndices() {
    Alphabet alpha("abc123");

    std::string input = "a1b2c3";
    std::vector<int> expected = {0, 3, 1, 4, 2, 5};
    std::vector<int> actual = alpha.toIndices(input);

    assert(actual == expected);

    // Test empty string
    std::vector<int> emptyResult = alpha.toIndices("");
    assert(emptyResult.empty());

    std::cout << "testToIndices passed\n";
}

void testToIndicesInvalidCharacter() {
    Alphabet alpha("abc");

    try {
        alpha.toIndices("abcd");  // 'd' not in alphabet
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("not in alphabet") != std::string::npos);
    }

    std::cout << "testToIndicesInvalidCharacter passed\n";
}

void testToChars() {
    Alphabet alpha("abc123");

    std::vector<int> indices = {0, 3, 1, 4, 2, 5};
    std::string expected = "a1b2c3";
    std::string actual = alpha.toChars(indices);

    assert(actual == expected);

    // Test empty vector
    std::string emptyResult = alpha.toChars({});
    assert(emptyResult.empty());

    std::cout << "testToChars passed\n";
}

void testToCharsInvalidIndex() {
    Alphabet alpha("abc");

    try {
        alpha.toChars({0, 1, 3});  // Index 3 out of range
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("must be between 0 and") != std::string::npos);
    }

    std::cout << "testToCharsInvalidIndex passed\n";
}

void testRoundTripConversion() {
    // Test that encoding then decoding gives original string
    Alphabet alpha("abcdefghijklmnopqrstuvwxyz0123456789");

    std::string original = "hello123world";
    std::vector<int> indices = alpha.toIndices(original);
    std::string decoded = alpha.toChars(indices);

    assert(original == decoded);

    // Test with predefined alphabets
    std::string dna = "ACGTACGT";
    std::vector<int> dnaIndices = Alphabet::DNA.toIndices(dna);
    std::string decodedDNA = Alphabet::DNA.toChars(dnaIndices);
    assert(dna == decodedDNA);

    std::cout << "testRoundTripConversion passed\n";
}

void testLgR() {
    // Test binary logarithm calculation
    assert(Alphabet::BINARY.lgR() == 1);      // 2^1 = 2
    assert(Alphabet::DNA.lgR() == 2);         // 2^2 = 4
    assert(Alphabet::DECIMAL.lgR() == 4);     // 2^4 = 16 > 10 >= 2^3 = 8
    assert(Alphabet::HEXADECIMAL.lgR() == 4); // 2^4 = 16

    // Test with power of 2
    Alphabet power2(8);
    assert(power2.lgR() == 3);  // 2^3 = 8

    // Test with non-power of 2
    Alphabet nonPower2(7);
    assert(nonPower2.lgR() == 3);  // 2^3 = 8 > 7 >= 2^2 = 4

    // Edge case: single character
    Alphabet single("a");
    assert(single.lgR() == 0);  // 2^0 = 1

    std::cout << "testLgR passed\n";
}


void testToString() {
    Alphabet alpha("xyz123");
    std::string str = alpha.toString();
    assert(str == "xyz123");

    // Test with predefined alphabet
    assert(Alphabet::BINARY.toString() == "01");
    assert(Alphabet::DNA.toString() == "ACGT");

    std::cout << "testToString passed\n";
}

void testOperatorSubscript() {
    Alphabet alpha("xyz123");

    assert(alpha[0] == 'x');
    assert(alpha[1] == 'y');
    assert(alpha[2] == 'z');
    assert(alpha[3] == '1');
    assert(alpha[4] == '2');
    assert(alpha[5] == '3');

    // Test out of range
    try {
        char c = alpha[-1];
        (void)c;  // Suppress unused variable warning
        assert(false);
    } catch (const std::out_of_range& e) {
        std::string msg = e.what();
        assert(msg.find("out of range") != std::string::npos);
    }

    try {
        char c = alpha[6];
        (void)c;
        assert(false);
    } catch (const std::out_of_range& e) {
        std::string msg = e.what();
        assert(msg.find("out of range") != std::string::npos);
    }

    std::cout << "testOperatorSubscript passed\n";
}

void testIterators() {
    Alphabet alpha("abc123");

    // Test range-based for loop
    std::string collected;
    for (char c : alpha) {
        collected += c;
    }
    assert(collected == "abc123");

    // Test explicit iterators
    std::string collected2;
    for (auto it = alpha.begin(); it != alpha.end(); ++it) {
        collected2 += *it;
    }
    assert(collected2 == "abc123");

    // Test iterator distance
    auto distance = std::distance(alpha.begin(), alpha.end());
    assert(distance == alpha.radix());

    std::cout << "testIterators passed\n";
}

void testEmptyAlphabet() {
    // Test alphabet with no characters (edge case)
    Alphabet empty("");

    assert(empty.radix() == 0);
    assert(empty.lgR() == 0);
    assert(!empty.contains('a'));
    assert(empty.toString().empty());

    // toChar and toIndex should throw for any input
    try {
        empty.toChar(0);
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    try {
        empty.toIndex('a');
        assert(false);
    } catch (const std::invalid_argument&) {
        // Expected
    }

    // Empty conversions should work
    assert(empty.toIndices("").empty());
    assert(empty.toChars({}).empty());

    std::cout << "testEmptyAlphabet passed\n";
}

void testSingleCharacterAlphabet() {
    Alphabet single("x");

    assert(single.radix() == 1);
    assert(single.lgR() == 0);
    assert(single.contains('x'));
    assert(!single.contains('y'));
    assert(single.toIndex('x') == 0);
    assert(single.toChar(0) == 'x');
    assert(single.toString() == "x");

    std::cout << "testSingleCharacterAlphabet passed\n";
}

void testCaseSensitivity() {
    Alphabet alpha("aAbBcC");

    assert(alpha.radix() == 6);
    assert(alpha.contains('a'));
    assert(alpha.contains('A'));
    assert(alpha.contains('b'));
    assert(alpha.contains('B'));
    assert(alpha.contains('c'));
    assert(alpha.contains('C'));

    // Different cases should have different indices
    assert(alpha.toIndex('a') != alpha.toIndex('A'));
    assert(alpha.toIndex('b') != alpha.toIndex('B'));
    assert(alpha.toIndex('c') != alpha.toIndex('C'));

    std::cout << "testCaseSensitivity passed\n";
}

void testSpecialCharacters() {
    // Test alphabet with special characters
    Alphabet special("!@#$%^&*()");

    assert(special.radix() == 10);
    assert(special.contains('!'));
    assert(special.contains('@'));
    assert(special.contains('*'));
    assert(!special.contains('a'));

    // Test round-trip with special characters
    std::string input = "!@#*";
    std::vector<int> indices = special.toIndices(input);
    std::string output = special.toChars(indices);
    assert(input == output);

    std::cout << "testSpecialCharacters passed\n";
}

void testLargeAlphabet() {
    // Test with larger alphabet
    std::string largeAlphabetStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    Alphabet large(largeAlphabetStr);

    assert(large.radix() == 62);

    // Test all characters are present and have correct indices
    for (size_t i = 0; i < largeAlphabetStr.length(); ++i) {
        char c = largeAlphabetStr[i];
        assert(large.contains(c));
        assert(large.toIndex(c) == static_cast<int>(i));
        assert(large.toChar(static_cast<int>(i)) == c);
    }

    std::cout << "testLargeAlphabet passed\n";
}

void testBoundaryValues() {
    // Test with boundary ASCII values
    std::string boundaryChars;
    boundaryChars.push_back('\0');  // Null character
    boundaryChars.push_back('\x01'); // SOH
    boundaryChars.push_back('\x7F'); // DEL
    boundaryChars.push_back('\xFF'); // 255

    Alphabet boundary(boundaryChars);

    assert(boundary.radix() == 4);
    assert(boundary.contains('\0'));
    assert(boundary.contains('\x01'));
    assert(boundary.contains('\x7F'));
    assert(boundary.contains('\xFF'));

    std::cout << "testBoundaryValues passed\n";
}

void testBASE64Alphabet() {
    // Test BASE64 alphabet specifically
    const Alphabet& base64 = Alphabet::BASE64;

    assert(base64.radix() == 64);

    // Test that all expected characters are present
    std::string expected = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    assert(base64.toString() == expected);

    // Test round-trip conversion with valid BASE64 characters only
    std::string input = "ABCDEFGHIJabcdefghij0123456789";  // Only valid BASE64 chars
    std::vector<int> indices = base64.toIndices(input);
    std::string output = base64.toChars(indices);
    assert(input == output);

    // Test that invalid characters are properly rejected
    try {
        base64.toIndex(' ');  // Space not in BASE64 alphabet
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("not in alphabet") != std::string::npos);
    }

    try {
        base64.toIndices("Hello World!");  // Contains invalid characters
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        assert(msg.find("not in alphabet") != std::string::npos);
    }

    std::cout << "testBASE64Alphabet passed\n";
}

void testPROTEINAlphabet() {
    // Test PROTEIN alphabet specifically
    const Alphabet& protein = Alphabet::PROTEIN;

    assert(protein.radix() == 20);

    // Test that all amino acid characters are present
    std::string aminoAcids = "ACDEFGHIKLMNPQRSTVWY";
    assert(protein.toString() == aminoAcids);

    // Test that excluded amino acids are not present
    assert(!protein.contains('B'));  // Aspartic acid or Asparagine
    assert(!protein.contains('J'));  // Leucine or Isoleucine
    assert(!protein.contains('O'));  // Pyrrolysine
    assert(!protein.contains('U'));  // Selenocysteine
    assert(!protein.contains('X'));  // Unknown
    assert(!protein.contains('Z'));  // Glutamic acid or Glutamine

    std::cout << "testPROTEINAlphabet passed\n";
}

void testPerformance() {
    // Test performance with repeated operations
    const Alphabet& alpha = Alphabet::EXTENDED_ASCII;

    // Test many character-to-index conversions
    for (int i = 0; i < 1000; ++i) {
        for (int c = 0; c < 256; ++c) {
            char ch = static_cast<char>(c);
            int index = alpha.toIndex(ch);
            assert(index == c);
        }
    }

    // Test many index-to-character conversions
    for (int i = 0; i < 1000; ++i) {
        for (int index = 0; index < 256; ++index) {
            char ch = alpha.toChar(index);
            assert(static_cast<unsigned char>(ch) == index);
        }
    }

    std::cout << "testPerformance passed\n";
}

int main() {
    std::cout << "Running Alphabet tests...\n";

    testBasicConstruction();
    testDuplicateCharacters();
    testRadixConstructor();
    testInvalidRadixConstructor();
    testDefaultConstructor();
    testPredefinedAlphabets();
    testToIndexValidCharacters();
    testToIndexInvalidCharacters();
    testToCharValidIndices();
    testToCharInvalidIndices();
    testToIndices();
    testToIndicesInvalidCharacter();
    testToChars();
    testToCharsInvalidIndex();
    testRoundTripConversion();
    testLgR();
    testToString();
    testOperatorSubscript();
    testIterators();
    testEmptyAlphabet();
    testSingleCharacterAlphabet();
    testCaseSensitivity();
    testSpecialCharacters();
    testLargeAlphabet();
    testBoundaryValues();
    testBASE64Alphabet();
    testPROTEINAlphabet();
    testPerformance();

    std::cout << "All tests passed!\n";
    return 0;
}