#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stack>
#include <string>

enum err_code { DECIMAL, OPERATOR, PARENTHESES, INVALID_ARG, INVALID_NUM, OVERFLOW, UNDERFLOW };
const std::string std_operators = "+-*/";
const std::string valid_operators = "+-*/()";

bool is_valid(char l, char r);
bool do_operation(std::string prev_op, std::string next_op);
double evaluate(double a, double b, std::string op);
void collapse(std::stack<std::string> &tokens, std::stack<std::string> &operators);
void handle_error(err_code n, int pos);
void check_valid_num(std::string num, int pos);

// Sample input: ((3+4-(5+ 5*3/2*(4-(5 / 6.01   ))))*(6 /5+2)-5*(5*3-10))
// A simple calculator program that allows addition, subtraction, multiplication, division, and
// parentheses.
int main(int argc, char *argv[]) {
    // Checks that only one argument was given (the expression to be evaluated).
    if (argc != 2) {
        std::cerr << "Needs 1 argument.\n";
        exit(1);
    }

    std::cout << std::numeric_limits<double>::max() << "\n";

    // Declarations
    std::string line(argv[1]);
    std::stack<std::string> tokens;
    std::stack<std::string> operators;
    int i = 0;
    int size = line.size();

    // Iterates through each character in the input string to divide the string into
    // tokens.
    while (i < size) {
        // Skips whitespace between numbers and operators. Commas are also treated as whitespace.
        if (line[i] != ' ' && line[i] != ',') {
            // Sets the first character as the start of the new token to be added to the tokens or
            // operators stack.
            std::string token(1, line[i]);

            // If the character is a digit, appends the rest of the digits of the number to the
            // token string and adds the number string to the tokens stack.
            if (isdigit(line[i]) || line[i] == '.' || line[i] == ',') {
                i++; // Move to the next character in the input.

                // Continues through the string, checking if the next character is a digit or
                // decimal '.'.
                while (isdigit(line[i]) || line[i] == '.' || line[i] == ' ' || line[i] == ',') {
                    // Only adds valid digits, not whitespace to the number token.
                    if (line[i] != ' ' && line[i] != ',') {
                        token += line[i]; // Appends the new digit to the number string.

                        // If the digits are separated by whitespace, there is a syntax error.
                        if (i > 0 && line[i - 1] == ' ')
                            handle_error(DECIMAL, i);
                    }

                    i++; // Move to the next character in the input.
                }

                // Check that this numeric token is within bounds of a double.
                check_valid_num(token, i);

                // Checks that decimal syntax is correct and there is only one or zero decimals in a
                // number (i.e. no '3.4.5' or '3.', only '3.45'). If not, it's an error.
                if (token.find_first_of(".") != token.find_last_of("."))
                    handle_error(DECIMAL, i);

                // Push the newly-made number token to the tokens stack.
                tokens.push(token);

            } else {
                // Otherwise, the character should represent an operator.
                // Current operator = operator being looked at at the time.

                // Checks that the operator is a valid operator: +-*/(). If not, it's an error.
                if (valid_operators.find_first_of(line[i]) == std::string::npos)
                    handle_error(INVALID_ARG, i);

                // Checks that the current operator's syntax is valid.
                if (std_operators.find_first_of(line[i]) != std::string::npos) {
                    int l_t = i - 1; // Left token position (to the current operator)
                    int r_t = i + 1; // Right token position (to the current operator)

                    // Move the left and right token positions to the first non-whitespace
                    // character in the input string.
                    while (l_t > 0 && line[l_t] == ' ')
                        l_t--;
                    while (r_t < line.size() && line[r_t] == ' ')
                        r_t++;

                    // Check that the syntax of the current operator relative to what is on the left
                    // and right is correct. If it's not, it's an error.
                    if (!is_valid(line[l_t], line[r_t]))
                        handle_error(OPERATOR, i);
                }

                // Looks at precedence of the current operator, and performs an operation in the
                // stack if precedence allows, reducing the size of both the tokens and
                // operator stack.
                if (!operators.empty() && do_operation(operators.top(), token)) {
                    // If an open parentheses, we have to wait for more input, so skip.
                    if (token.compare("(") != 0) {
                        // Otherwise, if it is a close parentheses, collapse/perform operations in
                        // the stack until the '(' operator is found. This fully calculates the
                        // inside of the parentheses.
                        if (token.compare(")") == 0)
                            while (!operators.empty() && operators.top().compare("(") != 0)
                                collapse(tokens, operators);
                        else // Otherwise, only collapse/perform one operation.
                            collapse(tokens, operators);
                    }
                }

                operators.push(token); // Push the current operator onto the operators stack.
                i++;                   // Move to the next character in the input.

                // If a close parentheses is at the top of the stack, the value of expression
                // in the parentheses has been calculated so we remove them.
                if (operators.size() >= 2 && operators.top().compare(")") == 0) {
                    operators.pop(); // Removes ")" from the stack.
                    operators.pop(); // Removes "(" from the stack.

                    // Checks if there is a multiply or divide operation to perform after/outside
                    // the parentheses. If so, collapses the stack and perform the operation.
                    if (!operators.empty() &&
                        (operators.top().compare("*") == 0 || operators.top().compare("/") == 0))
                        collapse(tokens, operators);
                }
            }
        } else {
            i++; // Moves to the next character in the input if the character was whitespace.
        }
    }

    collapse(tokens, operators);

    // If the tokens and operators stack are not at the expected size, there were probably
    // mismatched parentheses.
    if (tokens.size() > 1 || operators.size() > 0) {
        handle_error(PARENTHESES, 0);
    }

    // Prints out the result of the calculation.
    std::cout << "Result: " << tokens.top() << "\n";

    return 0;
}

// Returns true if an std_operator has valid syntax given the tokens to the left and right of it.
bool is_valid(char l, char r) {
    return ((isdigit(l) && isdigit(r)) || (l == ')' && r == '(') || (l == ')' && isdigit(r)) ||
            (isdigit(l) && r == '('));
}

// Returns true if an operation can be performed given precedence based on the previous and
// current/next operators.
bool do_operation(std::string prev_op, std::string next_op) {
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

// Evaluates a given expression given two double numbers a and b, and an operator (+-*/).
// Checks that the expression is within bounds of a double's size.
// Returns the value of the expression.
double evaluate(double a, double b, std::string op) {
    // Addition
    if (op.compare("+") == 0) {
        if (b > 0 && a >= std::numeric_limits<double>::max() - b)
            handle_error(OVERFLOW, 0);
        else if (b < 0 && a <= std::numeric_limits<double>::min() - b)
            handle_error(UNDERFLOW, 0);

        return a + b;
    }

    // Subtraction
    if (op.compare("-") == 0) {
        if (b < 0 && a >= std::numeric_limits<double>::max() + b)
            handle_error(OVERFLOW, 0);
        else if (b > 0 && a <= std::numeric_limits<double>::min() + b)
            handle_error(UNDERFLOW, 0);

        return a - b;
    }

    // Multiplication
    if (op.compare("*") == 0) {
        if (a == -1 && b == std::numeric_limits<double>::max())
            handle_error(OVERFLOW, 0);
        else if (b == -1 && a == std::numeric_limits<double>::min())
            handle_error(UNDERFLOW, 0);

        if (b != 0 && a > std::numeric_limits<double>::max() / b)
            handle_error(OVERFLOW, 0);
        else if (b != 0 && a < std::numeric_limits<double>::min() / b)
            handle_error(UNDERFLOW, 0);

        return a * b;
    }

    // Division
    return a / b;
}

// Collapses the given tokens and operators stack by performing a single operation. Removes the used
// tokens from the the stack and stores the value of the operation done back into the tokens stack.
void collapse(std::stack<std::string> &tokens, std::stack<std::string> &operators) {
    if (tokens.size() >= 2 && operators.size() >= 1) {
        double b = strtod(tokens.top().c_str(), NULL);
        tokens.pop();
        double a = strtod(tokens.top().c_str(), NULL);
        tokens.pop();

        tokens.push(std::to_string(evaluate(a, b, operators.top())));
        operators.pop();
    }
}

// Prints different error messages based on the given error code and exits the program.
// Some error messages print out the position of the invalid character/syntax from the input string.
void handle_error(err_code n, int pos) {
    switch (n) {
    case DECIMAL:
        std::cerr << "Error, invalid number at character " << pos << ".\n";
        break;
    case OPERATOR:
        std::cerr << "Error, invalid operator syntax at character " << pos << ".\n";
        break;
    case PARENTHESES:
        std::cerr << "Error, mismatched parentheses.\n";
        break;
    case INVALID_ARG:
        std::cerr << "Error, invalid argument at character " << pos << ".\n";
        break;
    case INVALID_NUM:
        std::cerr << "Error, invalid number input at character " << pos << ".\n";
        break;
    case OVERFLOW:
        std::cerr << "Error, double overflow.\n";
        break;
    case UNDERFLOW:
        std::cerr << "Error, double underflow.\n";
        break;
    }

    exit(1);
}

// Checks if a converted string->double is out of range. Causes an error and exits the program if
// so.
void check_valid_num(std::string num, int pos) {
    strtod(num.c_str(), NULL);

    if (errno == ERANGE)
        handle_error(INVALID_NUM, pos);
}
