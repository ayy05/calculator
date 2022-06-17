#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


int main(int argc, char *argv[]) {
    if (argc == 2) {
        char *line = argv[1];
        char *c = *d = argv[1];
        int length = strlen(line);
        char *input[100];
        int i = 0;
        printf("input: %s\n", line);
        printf("input length: %d\n", (int) strlen(line));

        while(*c != '\0') {
            input[i++] = c;

            if(isdigit(*c)) {
                printf("Digit: %c\n", *c++);
                while(isdigit(*c)) {
                    printf("Digit: %c\n", *c++);
                }
                
            }

            if(!isdigit(*c)) {
                printf("not digit: %c\n", *c++);
            }
        }





    }
    return 0;
}
