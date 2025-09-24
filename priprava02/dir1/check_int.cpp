#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "checker.h"

int check_numbers()
{
    char line[1024];
    int line_num = 1;
    
    while (fgets(line, sizeof(line), stdin)) {
        char *token;
        int numbers[100];
        int count = 0;
        int sum = 0;
        int expected_sum;
        
        // Parse line - tokenize by spaces
        token = strtok(line, " \n");
        while (token != NULL) {
            numbers[count] = atoi(token);  // atoi - only integers!
            count++;
            token = strtok(NULL, " \n");
        }
        
        if (count < 2) {
            fprintf(stderr, "Error: Invalid format on line %d\n", line_num);
            return 0;
        }
        
        // Last number is expected sum
        expected_sum = numbers[count-1];
        
        // Calculate sum of all numbers except the last one
        for (int i = 0; i < count-1; i++) {
            sum += numbers[i];
        }
        
        if (sum != expected_sum) {
            fprintf(stderr, "Error: Line %d - calculated sum %d != expected sum %d\n", 
                    line_num, sum, expected_sum);
            return 0;
        }
        
        printf("Line %d: %d numbers, sum %d - ano\n", line_num, count-1, sum);
        line_num++;
    }
    
    return 1;
}