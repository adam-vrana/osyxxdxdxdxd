#include <stdio.h>
#include "checker.h"

int main()
{
    if (check_numbers()) {
        printf("All lines verified successfully.\n");
        return 0;
    } else {
        printf("Verification failed!\n");
        return 1;
    }
}