
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/checker.h"

int check_numbers(const char* line, int line_number, int flag_debug) {
    char line_copy[10000];
    strcpy(line_copy, line);
    
    char *token = strtok(line_copy, " ");
    float sum = 0.0;
    int count = 0;
    
    while (token != NULL) {
        char *next_token = strtok(NULL, " ");

        if (strncmp(token, "0x", 2) == 0 || strncmp(token, "0b", 2) == 0) {
            fprintf(stderr, "Error: Wrong format detected on line %d\n", line_number);
            return 0;
        }
        
        float num = atof(token);
        
        if (next_token == NULL) {
            if (flag_debug) {
                printf("Line %d: Calculated sum = %.2f, Expected sum = %.2f\n", line_number, sum, num);
            }
            if (fabs(sum - num) > 0.01) {
                fprintf(stderr, "Error on line %d: Sum mismatch. Expected: %.2f, Got: %.2f\n", line_number, sum, num);
                return 0;
            } 
            break;
        } else {
            sum += num;
            count++;
        }
        
        token = next_token;
    }
    
    return 1;
}