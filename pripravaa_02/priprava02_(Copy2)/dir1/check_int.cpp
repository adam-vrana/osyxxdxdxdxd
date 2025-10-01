#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "checker.h"

int check_numbers() {
    char line[10000];
    int line_number = 1;
    
    while (fgets(line, sizeof(line), stdin)) {
        char *token = strtok(line, " ");
        int sum = 0;
        int count = 0;
        
        while (token != NULL) {
            char *next_token = strtok(NULL, " ");
            
            if (strstr(token, ".") || strncmp(token, "0x", 2) == 0 || strncmp(token, "0b", 2) == 0) {
                fprintf(stderr, "Error: Wrong number detected on line %d\n", line_number);
                return 0;
            }
            
            int num = atoi(token);
            
            if (next_token == NULL) {
                printf("Line %d: Calculated sum = %d, Expected sum = %d\n", line_number, sum, num);
                if (sum != num) {
                    fprintf(stderr, "Error on line %d: Sum mismatch. Expected: %d, Got: %d\n", line_number, sum, num);
                    return 0;
                } 
                break;
            } else {
                sum += num;
                count++;
            }

            token = next_token;
        }

        line_number++;
    }
    
    return 1;
}