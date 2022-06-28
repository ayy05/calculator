#include <cctype>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <string>

using namespace std;

enum err_code { DECIMAL, OPERATOR, PARENTHESES, INVALID_ARG };
const string std_operators = "+-*/";
const string valid_operators = "+-*/()";

bool is_valid(char l, char r);
bool do_operation(string prev_op, string next_op);
double evaluate(double a, double b, string op);
void collapse(stack<string> &tokens, stack<string> &operators);
void handle_error(err_code n, int pos);

// sample input: ((3+4-(5+ 5*3/2*(4-(5 / 6.01  - ))))*(6 /5+2)-5*(5*3-10))
int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Needs 1 argument.\n";
        exit(1);
    }

    string line(argv[1]);
    stack<string> tokens;
    stack<string> operators;
    int i = 0;
    int size = line.size();

    while (i < size) {
        if (line[i] != ' ') {
            string token(1, line[i]);

            if (isdigit(line[i]) || line[i] == '.') {
                i++;
                while (isdigit(line[i]) || line[i] == '.' || line[i] == ' ') {
                    if (line[i] != ' ') {
                        token += line[i];
                        if (i > 0 && line[i - 1] == ' ')
                            handle_error(DECIMAL, i);
                    }
                    i++;
                }

                if (token.find_first_of(".") != token.find_last_of("."))
                    handle_error(DECIMAL, i);

                tokens.push(token);
            } else {
                if (valid_operators.find_first_of(line[i]) == string::npos)
                    handle_error(INVALID_ARG, i);

                if (std_operators.find_first_of(line[i]) != string::npos) {
                    int l_n = i - 1;
                    int r_n = i + 1;

                    while (l_n > 0 && line[l_n] == ' ')
                        l_n--;

                    while (r_n < line.size() && line[r_n] == ' ')
                        r_n++;

                    if (!is_valid(line[l_n], line[r_n]))
                        handle_error(OPERATOR, i);
                }

                if (!operators.empty() && do_operation(operators.top(), token)) {
                    if (token.compare("(") != 0) {
                        if (token.compare(")") != 0)
                            collapse(tokens, operators);
                        else
                            while (!operators.empty() && operators.top().compare("(") != 0)
                                collapse(tokens, operators);
                    }
                }

                operators.push(token);
                i++;

                if (operators.size() >= 2 && operators.top().compare(")") == 0) {
                    operators.pop();
                    operators.pop();
                    if (!operators.empty() &&
                        (operators.top().compare("*") == 0 || operators.top().compare("/") == 0))
                        collapse(tokens, operators);
                }
            }
        } else {
            i++;
        }
    }

    if (tokens.size() > 1 || operators.size() > 0)
        handle_error(PARENTHESES, 0);

    cout << "Result: " << tokens.top() << "\n";

    return 0;
}

bool is_valid(char l, char r) {
    return ((isdigit(l) && isdigit(r)) || (l == ')' && r == '(') || (l == ')' && isdigit(r)) ||
            (isdigit(l) && r == '('));
}

bool do_operation(string prev_op, string next_op) {
    if (next_op.compare(prev_op) == 0)
        return true;

    if (next_op.compare("*") == 0 || next_op.compare("/") == 0)
        return (prev_op.compare("*") == 0 || prev_op.compare("/") == 0);

    if (next_op.compare("+") == 0 || next_op.compare("-") == 0)
        return prev_op.compare("(") != 0 && prev_op.compare(")") != 0;

    if (next_op.compare("(") == 0)
        return false;

    return next_op.compare(")") == 0;
}

double evaluate(double a, double b, string op) {
    if (op.compare("+") == 0)
        return a + b;

    if (op.compare("-") == 0)
        return a - b;

    if (op.compare("*") == 0)
        return a * b;

    return a / b;
}

void collapse(stack<string> &tokens, stack<string> &operators) {
    double b = atof(tokens.top().c_str());
    tokens.pop();
    double a = atof(tokens.top().c_str());
    tokens.pop();

    tokens.push(to_string(evaluate(a, b, operators.top())));
    operators.pop();
}

void handle_error(err_code n, int pos) {
    switch (n) {
    case DECIMAL:
        cerr << "Error, invalid number at character " << pos << ".\n";
        break;
    case OPERATOR:
        cerr << "Error, invalid operator syntax at character " << pos << ".\n";
        break;
    case PARENTHESES:
        cerr << "Error, mismatched parentheses.\n";
        break;
    case INVALID_ARG:
        cerr << "Error, invalid argument at character " << pos << ".\n";
        break;
    }

    exit(1);
}