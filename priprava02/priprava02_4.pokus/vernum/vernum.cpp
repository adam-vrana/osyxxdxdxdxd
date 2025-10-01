#include <stdio.h>
#include <string.h>
#include "checker.h"

int main()
{
    char line[10000];
    int line_number = 1;
    int all_success = 1;
    
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = 0;
        
        if (!check_numbers(line, line_number)) {
            all_success = 0;
            break;
        }
        line_number++;
    }
    
    if (all_success) {
        printf("All lines verified successfully.\n");
        return 0;
    } else {
        printf("Verification failed!\n");
        return 1;
    }
}