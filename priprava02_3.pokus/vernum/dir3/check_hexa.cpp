
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "checker.h"

int hex_to_int(const char *hex_str) {
    int result = 0;
    sscanf(hex_str, "0x%x", &result);
    return result;
}

int check_numbers() {
    char line[10000];
    int line_number = 1;
    
    while (fgets(line, sizeof(line), stdin)) { 
        char *token = strtok(line, " ");
        int sum = 0;
        int count = 0;
        
        while (token != NULL) {
            char *next_token = strtok(NULL, " ");
            
            if (strstr(token, ".") || strncmp(token, "0b", 2) == 0 || strncmp(token, "0x", 2) != 0) {
                fprintf(stderr, "Error: Wrong format detected on line %d\n", line_number);
                return 0;
            }
            
            int num = hex_to_int(token);
            
            if (next_token == NULL) {
                printf("Line %d: Calculated sum = 0x%x, Expected sum = 0x%x\n", line_number, sum, num);
                if (sum != num) {
                    fprintf(stderr, "Error on line %d: Sum mismatch. Expected: 0x%x, Got: 0x%x\n", line_number, sum, num);
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