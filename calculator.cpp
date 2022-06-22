#include <ctype.h>
#include <list>
#include <math.h>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calculate(std::list<char *> input);
void evaluate(int a, int b, char op);
char *create_string(int n);

static int total = 0;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Only 1 argument.\n");
        return 1;
    }

    char *line = argv[1];
    int length = strlen(line);
    char buffer[100];
    // char *input[100];
    char *curr = buffer;
    char c;
    int strnum = 0;
    std::stack<char> par_check;
    std::list<char *> input;
    std::list<char *>::iterator iter;

    printf("input: %s\n", line);
    printf("input length: %d\n", length);

    while ((c = *line++) != '\0') {
        input.push_back(curr);

        if (isdigit(c)) {
            *curr++ = c;
            // print all the digits of a number
            printf("Digit: %c\n", c);
            while (isdigit(*line)) {
                c = *line++;
                printf("Digit: %c\n", c);
                *curr++ = c;
            }
            *curr++ = '\0';
        } else {
            if (c != '*' && c != '/' && c != '+' && c != '-' && c != '(' && c != ')') {
                printf("Invalid input. Try again.\n");
                return 1;
            }
            // check parenthesis matching
            if (c == '(') {
                par_check.push(c);
                if (*line == ')') { // if like '()3+4'
                    printf("Invalid input. Try again.\n");
                    return 1;
                }
            }
            if (c == ')') {
                if (!par_check.empty()) {
                    if (!isdigit(*(line - 2))) { // if like '(3+)4'
                        printf("Invalid input. Try again.\n");
                        return 1;
                    }
                    par_check.pop();
                } else { // if like ')3+4'
                    printf("Invalid input. Try again.\n");
                    return 1;
                }
            }

            // print the characters
            printf("not digit: %c\n", c);
            *curr++ = c;
            *curr++ = '\0';
        }
    }

    printf("\nfound:\n");
    for (iter = input.begin(); iter != input.end(); iter++) {
        printf("%s\n", *iter);
    }

    // if parentheses are not matching, then invalid syntax
    if (!par_check.empty()) {
        printf("Invalid input. Try again.\n");
        return 1;
    }

    calculate(input);

    printf("\nafter:\n");
    for (iter = input.begin(); iter != input.end(); iter++) {
        printf("%s\n", *iter);
    }

    return 0;
}

void calculate(std::list<char *> input) {
    std::list<char *>::iterator iter, prev;
    int a, b;
    char c;

    printf("\n");
    iter = input.begin();

    while (iter != input.end()) {
        c = (*iter)[0];

        if (!isdigit(c)) {
            if (c == '*' || c == '/' || c == '+' || c == '-') {
                a = atoi(*std::prev(iter));
                b = atoi(*std::next(iter));
                printf("%d %c %d\n", a, c, b);

                // prev = std::prev(iter)--;

                // iter = prev;

                switch (c) {
                case '+':
                    input.insert(iter, create_string(a + b));
                    break;
                case '-':
                    input.insert(iter, create_string(a - b));
                    break;
                case '*':
                    input.insert(iter, create_string(a * b));
                    break;
                case '/':
                    input.insert(iter, create_string(a / b));
                    break;
                default:
                    break;
                }
                input.erase(std::prev(iter));
                input.erase(std::next(iter));
                input.erase(iter);
            }
        }

        iter++;
    }
}

void evaluate(int a, int b, char op) {
    switch (op) {
    case '+':
        total += a + b;
        break;
    case '-':
        total += a - b;
        break;
    case '*':
        total += a * b;
        break;
    case '/':
        total += a / b;
        break;
    default:
        break;
    }
}

char *create_string(int n) {
    int digits = floor(log10(abs(n))) + 1;
    int t = 10;
    char *ret, *c;

    ret = (char *)malloc(digits + 1);
    c = ret;
    t *= digits - 1;

    while (t > 1) {
        *c++ = (n % t) + '0';
        t /= 10;
    }

    *c = '\0';

    return ret;
}
