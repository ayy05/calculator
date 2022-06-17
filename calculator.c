#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc == 2) {
        char *line = argv[1];
        int length = strlen(line);
        char buffer[100];
        char *input[100];
        char *curr = buffer;
        char c;
        int strnum = 0;
        int i;

        printf("input: %s\n", line);
        printf("input length: %d\n", length);

        while ((c = *line++) != '\0') {
            input[strnum++] = curr;

            if (isdigit(c)) {
                *curr++ = c;
                printf("Digit: %c\n", c);
                while (isdigit(*line)) {
                    c = *line++;
                    printf("Digit: %c\n", c);
                    *curr++ = c;
                }
                *curr++ = '\0';
            } else {
                printf("not digit: %c\n", c);
                *curr++ = c;
                *curr++ = '\0';
            }
        }

        printf("\nfound:\n");
        for (i = 0; i < strnum; i++) {
            printf("%d: %s\n", i, input[i]);
        }
    }
    return 0;
}
