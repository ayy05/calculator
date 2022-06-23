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

const char *OPERATORS = "*/-+()";

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

    while ((c = *line++) != '\0') {
        if (isdigit(c)) {
            f_c = curr;
            *curr++ = c;

            while (isdigit(*line)) {
                c = *line++;
                *curr++ = c;
            }

            *curr++ = '\0';

            tokens.push(atoi(f_c));
        } else {
            if (c == '*' || c == '/') {
                if (!operators.empty() && (operators.top() == '*' || operators.top() == '/')) {
                    tokens.push(operators.top());
                    operators.pop();
                }
                operators.push(c);
            }

            else if (c == '+' || c == '-') {
                if (!operators.empty() && (operators.top() == '+' || operators.top() == '-' ||
                                           operators.top() == '*' || operators.top() == '/')) {
                    tokens.push(operators.top());
                    operators.pop();
                }
                operators.push(c);
            }

            else if (c == '(') {
                operators.push(c);
            }

            else if (c == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    tokens.push(operators.top());
                    operators.pop();
                }

                if (!operators.empty() && operators.top() != '(') {
                    fprintf(stderr, "Mismatched parenthes\n");
                    return 1;
                }

                operators.pop();
            }

            else {
                fprintf(stderr, "Invalid character.\n");
                return 1;
            }
        }
    }

    // push the rest of the operators onto the output queue
    while (!operators.empty()) {
        tokens.push(operators.top());
        operators.pop();
    }

    // process the tokens
    while (!tokens.empty()) {
        output.push(tokens.front());
        tokens.pop();

        op = output.top();
        if (op == ADD || op == SUBTRACT || op == MULTIPLY || op == DIVIDE) {
            output.pop();     // pop the operator
            b = output.top(); // get the value and pop it from the stack
            output.pop();
            a = output.top(); // get the value and pop it from the stack
            output.pop();

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

    printf("Result: %.2f\n", output.top());

    return 0;
}
