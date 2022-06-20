#include <ctype.h>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calculate(char **input);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Only 1 argument.\n");
        return 1;
    }

    char *line = argv[1];
    int length = strlen(line);
    char buffer[100];
    char *input[100];
    char *curr = buffer;
    char c;
    int strnum = 0;
    int i;
    std::stack<char> par_check;

    printf("input: %s\n", line);
    printf("input length: %d\n", length);

    while ((c = *line++) != '\0') {
        input[strnum++] = curr;

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
    for (i = 0; i < strnum; i++) {
        printf("%d: %s\n", i, input[i]);
    }

    // if parentheses are not matching, then invalid syntax
    if (!par_check.empty()) {
        printf("Invalid input. Try again.\n");
        return 1;
    }

    calculate(input);

    return 0;
}

void calculate(char **input) {}