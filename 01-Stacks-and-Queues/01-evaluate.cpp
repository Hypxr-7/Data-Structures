// Evaluates (fully parenthesized) arithmetic expressions using
// Dijkstra's two-stack algorithm.

// Note: the operators, operands, and parentheses must be
// separated by whitespace. Also, each operation must
// be enclosed in parentheses. For example, you must write
// ( 1 + ( 2 + 3 ) ) instead of ( 1 + 2 + 3 ).

#include <iostream>
#include <string>
#include <stack>

int evaluate(const std::string& expression);
bool isOperator(char c);
int calculate(int n1, int n2, char op);

int main() {
    std::string expression = "( 1 + ( ( 2 + 3 ) * ( 4 * 5 ) ) )";
    std::cout << evaluate(expression) << '\n';

    return 0;
}

int evaluate(const std::string& expression) {
    std::stack<int> operands;
    std::stack<char> operators;

    for (auto c : expression) {
        if (c == '(' || c == ' '); // do nothing
        else if (isdigit(c)) operands.push(static_cast<int>(c - '0'));
        else if (isOperator(c)) operators.push(c);
        else if (c == ')') {
            int num1 = operands.top(); operands.pop();
            int num2 = operands.top(); operands.pop();
            char op = operators.top(); operators.pop();
            operands.push(calculate(num1, num2, op));
        }
    }

    return operands.top();
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int calculate(int n1, int n2, char op) {
    if (op == '+') return n2 + n1;
    if (op == '-') return n2 - n1;
    if (op == '*') return n2 * n1;
    if (op == '/') return n2 / n1;
    return -1;
}