#include <iostream>
#include <stack>
#include <string>
#include <cassert>
#include <cctype>
#include "04-LLStack.hpp"

int evaluatePostfix(const std::string& expression);

void testEvaluatePostfix();

int main() {
    testEvaluatePostfix();
    
    return 0;
}

int evaluatePostfix(const std::string& expression) {
    Stack<int> stack;

    for (char ch : expression) {
        if (std::isdigit(ch)) {
            stack.push(ch - '0');
        } else {
            int val1 = stack.pop();
            int val2 = stack.pop();

            switch (ch) {
                case '+':
                    stack.push(val2 + val1);
                    break;
                case '-':
                    stack.push(val2 - val1);
                    break;
                case '*':
                    stack.push(val2 * val1);
                    break;
                case '/':
                    stack.push(val2 / val1);
                    break;
            }
        }
    }
    return stack.pop();
}

void testEvaluatePostfix() {
    assert(evaluatePostfix("231*+9-") == -4);
    assert(evaluatePostfix("123+*8-") == -3);
    assert(evaluatePostfix("56+7*") == 77);
    assert(evaluatePostfix("34*52-/") == 4);
    assert(evaluatePostfix("82/3-") == 1);
    assert(evaluatePostfix("92-3*") == 21);
    assert(evaluatePostfix("12+34+*") == 21);
    assert(evaluatePostfix("56+78+*") == 165);
    assert(evaluatePostfix("12+34*+") == 15);
    assert(evaluatePostfix("123*+45*+") == 27);

    std::cout << "All test cases passed!" << std::endl;
}
