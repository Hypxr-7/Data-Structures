/* 
 *  Reads in a fully parenthesized infix expression from standard input
 *  and prints an equivalent postfix expression to standard output.
 */

#include <cassert>
#include <iostream>
#include <string>
#include "04-LLStack.hpp"

std::string infixToPostfix(const std::string& expression);
int precedence(char op);
bool isOperator(char op);

void testInfixToPostfix();

int main() {
    testInfixToPostfix();
    std::cout << "All tests passed!" << std::endl;
    
    return 0;
}

std::string infixToPostfix(const std::string& expression) {
    std::string postfix;
    Stack<char> stack;

    for (char ch : expression) {
        if (std::isdigit(ch) || std::isalpha(ch)) {
            postfix += ch;
        } else if (ch == '(') {
            stack.push(ch);
        } else if (ch == ')') {
            while (!stack.isEmpty() && stack.peek() != '(') {
                postfix += stack.pop();
            }
            stack.pop(); // Remove '(' from stack
        } else if (isOperator(ch)) {
            while (!stack.isEmpty() && precedence(stack.peek()) >= precedence(ch)) {
                postfix += stack.pop();
            }
            stack.push(ch);
        }
    }

    while (!stack.isEmpty()) {
        postfix += stack.pop();
    }

    return postfix;
}

int precedence(char op) {
    if (op == '/' || op == '*')
        return 2;
    else if (op == '+' || op == '-')
        return 1;
    else
        return -1;
}

bool isOperator(char op) {
    return (op == '+' || op == '-' || op == '*' || op == '/');
}

void testInfixToPostfix() {
    assert(infixToPostfix("(a+b)*(c+d)") == "ab+cd+*");
    assert(infixToPostfix("a+b*c+d") == "abc*+d+");
    assert(infixToPostfix("a*(b+c)") == "abc+*");
    assert(infixToPostfix("a+b") == "ab+");
    assert(infixToPostfix("a*(b+c*(d+e))") == "abcde+*+*");
    assert(infixToPostfix("a+b*(c+d*e)") == "abcde*+*+");
    assert(infixToPostfix("a+b*c+d/e-f") == "abc*+de/+f-");
}
