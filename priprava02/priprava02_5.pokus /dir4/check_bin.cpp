
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/checker.h"

int bin_to_int(const char *bin_str) {
    int result = 0;
    // bin_str += 2;
    // while (*bin_str) {
    //     result = result * 2 + (*bin_str - '0');
    //     bin_str++;
    // }
    sscanf(bin_str, "0b%b", &result);
    return result;
}

int check_numbers(const char* line, int line_number, int flag_debug) {
    char line_copy[10000];
    strcpy(line_copy, line);
    
    char *token = strtok(line_copy, " \n");
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
    
    if (flag_debug) {
        printf("Line %d: Calculated sum = %d, Expected sum = %d\n", line_number, sum, expected_sum);
    }
    
    if (sum != expected_sum) {
        fprintf(stderr, "Error on line %d: Sum mismatch. Expected: %d, Got: %d\n", line_number, expected_sum, sum);
        return 0;
    } 
    
    return 1;
}