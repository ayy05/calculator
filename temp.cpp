#include <ctype.h>
#include <iostream>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string>

bool do_operation(std::string prev_op, std::string next_op);
double get_val(double a, double b, std::string op);
void collapse(std::stack<std::string> &tokens, std::stack<std::string> &operators, int num);

int main(int argc, char *argv[]) {
    /* if (argc != 2) {
        printf("Only 1 argument.\n");
        return 1;
    } */

    std::string line("3+4-(5/6)*(6/5)-5");
    std::stack<std::string> tokens;
    std::stack<std::string> operators;
    int i = 0;

    while (line[i] != '\0') {
        if (line[i] != ' ') {
            std::string token(1, line[i]);

            if (isdigit(line[i]) || line[i] == '.') {
                i++;
                while (isdigit(line[i]) || line[i] == '.') {
                    if (line[i] != ' ')
                        token += line[i++];
                }

                tokens.push(token);
            } else {
                if (!operators.empty() && do_operation(operators.top(), token)) {
                    collapse(tokens, operators, 1);
                }

                operators.push(token);
                if (operators.size() >= 2 && operators.top().compare(")") == 0) {
                    operators.pop();
                    operators.pop();
                }
                i++;
            }
        }
    }

    if (tokens.size() > 2 || operators.size() > 0) {
        collapse(tokens, operators, operators.size());
    }

    std::cout << "Result: " << tokens.top() << "\n";

    return 0;
}

bool do_operation(std::string prev_op, std::string next_op) {
    if (next_op.compare(prev_op) == 0) {
        return true;
    } else {
        if (next_op.compare("*") == 0 || next_op.compare("/") == 0) {
            return (prev_op.compare("*") == 0 || prev_op.compare("/") == 0);
        } else if (next_op.compare("(") == 0) {
            return false;
        } else if (next_op.compare("+") == 0 || next_op.compare("-") == 0) {
            return prev_op.compare("(") != 0 && prev_op.compare(")") != 0;
        } else {
            return next_op.compare(")") == 0;
        }
    }
}

double get_val(double a, double b, std::string op) {
    if (op.compare("+") == 0) {
        return a + b;
    }
    if (op.compare("-") == 0) {
        return a - b;
    }
    if (op.compare("*") == 0) {
        return a * b;
    }
    if (op.compare("/") == 0) {
        return a / b;
    }

    fprintf(stderr, "Invalid operator.\n");
    return 1;
}

void collapse(std::stack<std::string> &tokens, std::stack<std::string> &operators, int num) {
    while (num--) {
        double b = atof(tokens.top().c_str());
        tokens.pop();
        double a = atof(tokens.top().c_str());
        tokens.pop();
        tokens.push(std::to_string(get_val(a, b, operators.top())));
        operators.pop();
    }
}