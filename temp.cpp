#include <ctype.h>
#include <deque>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

bool do_operation(std::string prev_op, std::string next_op);
double get_val(double a, double b, std::string op);
void collapse_back(std::deque<std::string> &tokens, std::deque<std::string> &operators, int num);
void collapse_front(std::deque<std::string> &tokens, std::deque<std::string> &operators, int num);
void collapse_back(std::deque<std::string> &tokens, std::deque<std::string> &operators,
                   std::string op);

int main(int argc, char *argv[]) {
    /* if (argc != 2) {
        printf("Only 1 argument.\n");
        return 1;
    } */

    std::string line("(((3)+(4 ))-(5 / 6)   *(6 /5)-5*(5*3-1  0))");
    std::deque<std::string> tokens;
    std::deque<std::string> operators;
    int i = 0;

    while (line[i] != '\0') {
        if (line[i] != ' ') {
            std::string token(1, line[i]);

            if (isdigit(line[i]) || line[i] == '.') {
                i++;
                while (isdigit(line[i]) || line[i] == '.' || line[i] == ' ') {
                    if (line[i] != ' ')
                        token += line[i];
                    i++;
                }

                tokens.push_back(token);
            } else {
                if (!operators.empty() && do_operation(operators.back(), token)) {
                    if (token.compare("(") != 0) {
                        if (token.compare(")") != 0)
                            collapse_back(tokens, operators, 1);
                        else
                            collapse_back(tokens, operators, "(");
                    }
                }

                operators.push_back(token);
                i++;

                if (operators.size() >= 2 && operators.back().compare(")") == 0) {
                    operators.pop_back();
                    operators.pop_back();
                    if (!operators.empty() &&
                        (operators.back().compare("*") == 0 || operators.back().compare("/") == 0))
                        collapse_back(tokens, operators, 1);
                }
            }
        } else {
            i++;
        }
    }

    if (tokens.size() > 2 || operators.size() > 0) {
        collapse_front(tokens, operators, operators.size());
    }

    std::cout << "Result: " << tokens.back() << "\n";

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

void collapse_back(std::deque<std::string> &tokens, std::deque<std::string> &operators, int num) {
    while (num--) {
        double b = atof(tokens.back().c_str());
        tokens.pop_back();
        double a = atof(tokens.back().c_str());
        tokens.pop_back();
        tokens.push_back(std::to_string(get_val(a, b, operators.back())));
        operators.pop_back();
    }
}

void collapse_back(std::deque<std::string> &tokens, std::deque<std::string> &operators,
                   std::string op) {
    while (operators.back().compare(op) != 0) {
        double b = atof(tokens.back().c_str());
        tokens.pop_back();
        double a = atof(tokens.back().c_str());
        tokens.pop_back();
        tokens.push_back(std::to_string(get_val(a, b, operators.back())));
        operators.pop_back();
    }
}

void collapse_front(std::deque<std::string> &tokens, std::deque<std::string> &operators, int num) {
    while (num--) {
        double a = atof(tokens.front().c_str());
        tokens.pop_front();
        double b = atof(tokens.front().c_str());
        tokens.pop_front();
        tokens.push_front(std::to_string(get_val(a, b, operators.front())));
        operators.pop_front();
    }
}