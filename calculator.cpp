#include <assert.h>
#include <ctype.h>
#include <queue>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 100
#define ADD 43
#define SUBTRACT 45
#define MULTIPLY 42
#define DIVIDE 47

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Only 1 argument.\n");
        return 1;
    }

    char *line = argv[1];
    char buffer[MAX_INPUT];
    char *curr = buffer;
    char c, *f_c;
    std::queue<double> tokens;
    std::stack<char> operators;
    std::stack<double> output;
    double a, b;
    int op;

    printf("Input: %s\n", line);

    // Get the tokens from the input line.
    while ((c = *line++) != '\0') {
        if (c != ' ') {
            // If the character is a digit...
            if (isdigit(c) || c == '.') {
                f_c = curr;
                *curr++ = c;

                // Read consecutive characters that until a non-digit character is reached.
                // Reads the characters into the buffer string.
                while (isdigit(*line) || *line == '.') {
                    c = *line++;
                    *curr++ = c;
                }

                // End the string once all the digits are found.
                *curr++ = '\0';

                // Use atoi() to conver the string to a number and push it onto the tokens queue.
                tokens.push(atof(f_c));

                // Else the character is a non-digit...
            } else {
                // If multiply or divide...
                if (c == '*' || c == '/') {
                    // Check that the operator stack is not empty, then check the top of the stack.
                    // If the top of the stack is an operator with the same precendence (multiply or
                    // divide), push the top of the operator stack onto the tokens queue and remove
                    // it from the stack.
                    if (!operators.empty() && (operators.top() == '*' || operators.top() == '/')) {
                        tokens.push(operators.top());
                        operators.pop();
                    }
                    // Push the character onto the operators stack.
                    operators.push(c);
                }

                // If add or subtract...
                else if (c == '+' || c == '-') {
                    // Check that the operator stack is not empty, then check the top of the stack.
                    // If the top of the stack is an operator with the same precendence (multiply,
                    // divide, add, subtract), push the top of the operator stack onto the tokens
                    // queue and remove it from the stack.
                    if (!operators.empty() && (operators.top() == '+' || operators.top() == '-' ||
                                               operators.top() == '*' || operators.top() == '/')) {
                        tokens.push(operators.top());
                        operators.pop();
                    }
                    // Push the character onto the operators stack.
                    operators.push(c);
                }

                // If a left parenthesis...
                else if (c == '(') {
                    // Push the character onto the operators stack.
                    operators.push(c);
                }

                // If a right parenthesis...
                else if (c == ')') {
                    // Push all operators on the operators stack onto the tokens queue until the
                    // left parenthesis is found.
                    while (!operators.empty() && operators.top() != '(') {
                        tokens.push(operators.top());
                        operators.pop();
                    }

                    // If the left parenthesis was not found, then there are mismatched parentheses.
                    if (!operators.empty() && operators.top() != '(') {
                        fprintf(stderr, "Mismatched parentheses\n");
                        return 1;
                    }

                    // Push the character onto the operators stack.
                    operators.pop();
                }

                // Otherwise, was not a valid operator.
                else {
                    fprintf(stderr, "Invalid character.\n");
                    return 1;
                }
            }
        }
    }

    // Push the remaining operators on the stack onto the token queue.
    while (!operators.empty()) {
        tokens.push(operators.top());
        operators.pop();
    }

    // Process the tokens.
    while (!tokens.empty()) {
        // Push the front of the tokens queue onto the output stack and remove it from the queue.
        output.push(tokens.front());
        tokens.pop();

        op = output.top();
        // If the token is an operator...
        if (op == ADD || op == SUBTRACT || op == MULTIPLY || op == DIVIDE) {
            output.pop();     // Pop the operator.
            b = output.top(); // Get the first value and pop it from the stack.
            output.pop();
            a = output.top(); // Get the second value and pop it from the stack.
            output.pop();

            // Push the output of the evaluated expression back onto the output stack.
            switch (op) {
            case ADD:
                output.push(a + b);
                break;
            case SUBTRACT:
                output.push(a - b);
                break;
            case MULTIPLY:
                output.push(a * b);
                break;
            case DIVIDE:
                output.push(a / b);
                break;
            }
        }
    }

    // Once all the tokens are processed, the top of the output stack will be the final result.
    printf("Result: %.2f\n", output.top());

    return 0;
}
