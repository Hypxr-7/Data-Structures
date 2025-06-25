#pragma once

#include <stdexcept>
#include <string>
#include <vector>

/**
 * A data type for alphabets, for use with string-processing code
 * that must convert between an alphabet of size R and the integers
 * 0 through R-1.
 *
 * Provides bidirectional mapping between characters and their indices
 * within a defined alphabet. Useful for:
 * - Radix sorts and string processing algorithms
 * - Encoding/decoding with custom character sets
 * - Trie and suffix array implementations
 * - Compression algorithms that work with specific alphabets
 *
 * Key features:
 * - Fast O(1) character-to-index and index-to-character conversion
 * - Validation that all characters in alphabet are unique
 * - Support for custom alphabets and predefined common alphabets
 * - Efficient storage using arrays for fast lookup
 *
 * Warning: supports only ASCII characters (0-255) for efficiency.
 * For Unicode support, modifications would be needed.
 */
class Alphabet {
    std::vector<char> alphabet_;           // Characters in the alphabet
    std::vector<int> inverse_;             // Character to index mapping (inverse[c] = index of char c)
    int R_;                               // Radix (size of alphabet)

    static constexpr int MAX_CHAR = 256;  // Support ASCII characters 0-255

public:
    // Predefined common alphabets
    static const Alphabet BINARY;         // { 0, 1 }
    static const Alphabet OCTAL;          // { 0, 1, 2, 3, 4, 5, 6, 7 }
    static const Alphabet DECIMAL;        // { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
    static const Alphabet HEXADECIMAL;    // { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F }
    static const Alphabet DNA;            // { A, C, G, T }
    static const Alphabet LOWERCASE;      // { a, b, c, ..., z }
    static const Alphabet UPPERCASE;      // { A, B, C, ..., Z }
    static const Alphabet PROTEIN;        // { A, C, D, E, F, G, H, I, K, L, M, N, P, Q, R, S, T, V, W, Y }
    static const Alphabet BASE64;         // Base-64 alphabet (64 characters)
    static const Alphabet ASCII;          // ASCII alphabet (0-127)
    static const Alphabet EXTENDED_ASCII; // Extended ASCII alphabet (0-255)

    /**
     * Initializes a new alphabet from the given set of characters.
     * Characters must be unique within the alphabet.
     *
     * @param alpha string containing all characters in the alphabet
     * @throws std::invalid_argument if alphabet contains duplicate characters
     * @throws std::invalid_argument if any character is outside ASCII range
     */
    explicit Alphabet(const std::string& alpha) : R_(static_cast<int>(alpha.length())) {
        // Check for duplicate characters using boolean array
        std::vector<bool> seen(MAX_CHAR, false);

        for (char c : alpha) {
            // Check if character is within supported range
            const auto uc = static_cast<unsigned char>(c);
            if (uc >= MAX_CHAR) {
                throw std::invalid_argument("Character outside supported range: " + std::to_string(static_cast<int>(c)));
            }

            // Check for duplicates
            if (seen[uc]) {
                throw std::invalid_argument("Illegal alphabet: repeated character = '" + std::string(1, c) + "'");
            }
            seen[uc] = true;
        }

        // Initialize alphabet array
        alphabet_.reserve(R_);
        for (char c : alpha) {
            alphabet_.push_back(c);
        }

        // Initialize inverse mapping (all positions start as invalid)
        inverse_.assign(MAX_CHAR, -1);

        // Build character-to-index mapping
        for (int i = 0; i < R_; ++i) {
            const auto uc = static_cast<unsigned char>(alphabet_[i]);
            inverse_[uc] = i;
        }
    }

    /**
     * Initializes a new alphabet using characters 0 through radix-1.
     * Creates an alphabet where character i maps to index i.
     *
     * @param radix the number of characters in the alphabet
     * @throws std::invalid_argument if radix is negative or too large
     */
    explicit Alphabet(int radix) : R_(radix) {
        if (radix < 0) {
            throw std::invalid_argument("Radix must be non-negative");
        }
        if (radix > MAX_CHAR) {
            throw std::invalid_argument("Radix too large for supported character range");
        }

        alphabet_.reserve(R_);
        inverse_.assign(R_, 0);  // For consecutive alphabet, inverse size = radix

        // Character i maps to index i
        for (int i = 0; i < R_; ++i) {
            alphabet_.push_back(static_cast<char>(i));
            inverse_[i] = i;
        }
    }

    /**
     * Default constructor: initializes alphabet using characters 0 through 255.
     * Creates extended ASCII alphabet.
     */
    Alphabet() : Alphabet(256) {}

    /**
     * Returns true if the character is in this alphabet.
     *
     * @param c the character to check
     * @return true if c is a character in this alphabet, false otherwise
     */
    bool contains(char c) const {
        const auto uc = static_cast<unsigned char>(c);
        return uc < inverse_.size() && inverse_[uc] != -1;
    }

    /**
     * Returns the number of characters in this alphabet (the radix).
     *
     * @return the radix of this alphabet
     */
    int radix() const {
        return R_;
    }

    /**
     * Returns the binary logarithm of the number of characters in this alphabet.
     * Useful for determining how many bits are needed to represent each character.
     *
     * @return the binary logarithm (rounded up) of the radix
     */
    int lgR() const {
        int lgR = 0;
        for (int t = R_ - 1; t >= 1; t /= 2) {
            lgR++;
        }
        return lgR;
    }

    /**
     * Returns the index corresponding to the given character.
     * Performs fast O(1) lookup using inverse array.
     *
     * @param c the character
     * @return the index corresponding to character c (0 to R-1)
     * @throws std::invalid_argument if c is not in this alphabet
     */
    int toIndex(char c) const {
        const auto uc = static_cast<unsigned char>(c);
        if (uc >= inverse_.size() || inverse_[uc] == -1) {
            throw std::invalid_argument("Character '" + std::string(1, c) + "' not in alphabet");
        }
        return inverse_[uc];
    }

    /**
     * Returns the indices corresponding to the characters in the string.
     * Converts each character to its alphabet index.
     *
     * @param s the string to convert
     * @return vector of indices corresponding to characters in s
     * @throws std::invalid_argument if any character in s is not in this alphabet
     */
    std::vector<int> toIndices(const std::string& s) const {
        std::vector<int> indices;
        indices.reserve(s.length());

        for (const char c : s) {
            indices.push_back(toIndex(c));
        }
        return indices;
    }

    /**
     * Returns the character corresponding to the given index.
     * Performs fast O(1) lookup using alphabet array.
     *
     * @param index the index (must be between 0 and R-1)
     * @return the character corresponding to index
     * @throws std::invalid_argument if index is out of range
     */
    char toChar(const int index) const {
        if (index < 0 || index >= R_) {
            throw std::invalid_argument("Index must be between 0 and " + std::to_string(R_ - 1) + ": " + std::to_string(index));
        }
        return alphabet_[index];
    }

    /**
     * Returns the string corresponding to the given indices.
     * Converts each index to its corresponding character.
     *
     * @param indices vector of indices to convert
     * @return string of characters corresponding to the indices
     * @throws std::invalid_argument if any index is out of range
     */
    std::string toChars(const std::vector<int>& indices) const {
        std::string result;
        result.reserve(indices.size());

        for (const int index : indices) {
            result.push_back(toChar(index));
        }
        return result;
    }

    /**
     * Returns string representation of this alphabet.
     * Shows all characters in the alphabet in order.
     *
     * @return string containing all alphabet characters
     */
    std::string toString() const {
        std::string result;
        result.reserve(R_);
        for (int i = 0; i < R_; ++i) {
            result.push_back(alphabet_[i]);
        }
        return result;
    }

    /**
     * Returns the character at the specified position in the alphabet.
     * Equivalent to toChar() but with array-style access.
     *
     * @param index position in alphabet
     * @return character at that position
     * @throws std::out_of_range if index is invalid
     */
    char operator[](int index) const {
        if (index < 0 || index >= R_) {
            throw std::out_of_range("Index out of range: " + std::to_string(index));
        }
        return alphabet_[index];
    }

    /**
     * Returns iterator to beginning of alphabet.
     * Allows range-based for loops over alphabet characters.
     */
    std::vector<char>::const_iterator begin() const {
        return alphabet_.begin();
    }

    /**
     * Returns iterator to end of alphabet.
     * Allows range-based for loops over alphabet characters.
     */
    std::vector<char>::const_iterator end() const {
        return alphabet_.end();
    }
};

// Static member definitions
const Alphabet Alphabet::BINARY("01");
const Alphabet Alphabet::OCTAL("01234567");
const Alphabet Alphabet::DECIMAL("0123456789");
const Alphabet Alphabet::HEXADECIMAL("0123456789ABCDEF");
const Alphabet Alphabet::DNA("ACGT");
const Alphabet Alphabet::LOWERCASE("abcdefghijklmnopqrstuvwxyz");
const Alphabet Alphabet::UPPERCASE("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
const Alphabet Alphabet::PROTEIN("ACDEFGHIKLMNPQRSTVWY");
const Alphabet Alphabet::BASE64("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
const Alphabet Alphabet::ASCII(128);
const Alphabet Alphabet::EXTENDED_ASCII(256);
