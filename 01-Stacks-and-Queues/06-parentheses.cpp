/*
 *
 *  Reads in a text file and checks to see if the parentheses are balanced.
 *
 */
 
 #include <cassert>
 #include <iostream>
 #include <string>
 #include "04-LLStack.hpp"

 bool isBalanced(const std::string& parentheses);

 void testIsBalanced();

 int main() {
   testIsBalanced();

   return 0;
 }

 bool isBalanced(const std::string& parentheses) {
   Stack<char> stack;

   for (auto c : parentheses) {
      if (c == '(' || c == '[' || c == '{') {
         stack.push(c);
      }

      else if (c == ')') {
         if (stack.isEmpty()) return false;
         if (stack.pop() != '(') return false;
      }

      else if (c == '}') {
         if (stack.isEmpty()) return false;
         if (stack.pop() != '{') return false;
      }

      else if (c == ']') {
         if (stack.isEmpty()) return false;
         if (stack.pop() != '[') return false;
      }
   }

   return stack.isEmpty();
 }

 void testIsBalanced() {
   // Test case 1: Balanced parentheses
    assert(isBalanced("()") == true);
    assert(isBalanced("[]") == true);
    assert(isBalanced("{}") == true);
    assert(isBalanced("()[]{}") == true);
    assert(isBalanced("[()]{}{[()()]()}") == true);

    // Test case 2: Unbalanced parentheses
    assert(isBalanced("(") == false);
    assert(isBalanced(")") == false);
    assert(isBalanced("[(])") == false);
    assert(isBalanced("([)]") == false);
    assert(isBalanced("{[}") == false);

    // Test case 3: Empty string
    assert(isBalanced("") == true);

    // Test case 4: Complex balanced and unbalanced cases
    assert(isBalanced("{[()()]}") == true);
    assert(isBalanced("{[(])}") == false);
    assert(isBalanced("{[()]}[{}]") == true);
    assert(isBalanced("{[()]}[{}](") == false);

    std::cout << "All test cases passed!" << std::endl;
 }