
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "checker.h"

int bin_to_int(const char *bin_str) {
    int result = 0;
    bin_str += 2;
    while (*bin_str) {
        result = result * 2 + (*bin_str - '0');
        bin_str++;
    }
    return result;
}

int check_numbers() {
    char line[10000];
    int line_number = 1;
    
    while (fgets(line, sizeof(line), stdin)) {
        char *token = strtok(line, " \n");
        int sum = 0;
        int count = 0;
        int expected_sum = -1;
        
        while (token != NULL) {
            if (strstr(token, ".") || strncmp(token, "0x", 2) == 0 || strncmp(token, "0b", 2) != 0) {
                fprintf(stderr, "Error: Wrong format detected on line %d\n", line_number);
                return 0;
            }
            
            int num = bin_to_int(token);
            char *next_token = strtok(NULL, " \n");
            
            if (next_token == NULL) {
                expected_sum = num;
                break;
            } else {
                sum += num;
                count++;
            }
            
            token = next_token;
        }
        
        printf("Line %d: Calculated sum = %d, Expected sum = %d\n", line_number, sum, expected_sum);
        
        if (sum != expected_sum) {
            fprintf(stderr, "Error on line %d: Sum mismatch. Expected: %d, Got: %d\n", line_number, expected_sum, sum);
            return 0;
        } 

        line_number++;
    }
    
    return 1;
}