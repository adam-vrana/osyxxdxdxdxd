#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *input = stdin;
    FILE *output = stdout;
    FILE *log = NULL;

    int flag_i = 0;
    int flag_n = 0;
    char *pattern = NULL;
    char *input_filename = NULL;
    char *output_filename = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            flag_i = 1;
        } else if (strcmp(argv[i], "-n") == 0) {
            flag_n = 1;
        } else if (pattern == NULL) {
            pattern = argv[i];
        } else if (input_filename == NULL) {
            input_filename = argv[i];
        } else if (output_filename == NULL) {
            output_filename = argv[i];
        }
    }

    if (pattern == NULL) {
        fprintf(stderr, "Chyba: Nebyl zadán hledaný řetězec.\n");
        return 1;
    }

    if (input_filename != NULL) {
        input = fopen(input_filename, "r");
        if (input == NULL) {
            fprintf(stderr, "Chyba: Nelze otevřít vstupní soubor '%s'\n", input_filename);
            return 2;
        }
    }

    if (output_filename != NULL) {
        output = fopen(output_filename, "w");
        if (output == NULL) {
            fprintf(stderr, "Chyba: Nelze otevřít výstupní soubor '%s'\n", output_filename);
            if (input != stdin) fclose(input);
            return 3;
        }
    }

    if (input_filename && output_filename) {
        log = fopen("filter.log", "a");
        if (log == NULL) {
            fprintf(stderr, "Chyba: Nelze vytvořit logovací soubor.\n");
            if (input != stdin) fclose(input);
            if (output != stdout) fclose(output);
            return 4;
        }
    }

    char buffer[1024];
    int line_number = 1;
    int matches = 0;
    int lines_processed = 0;

    while (fgets(buffer, sizeof(buffer), input)) {
        lines_processed++;
        char *found = strstr(buffer, pattern);

        if (found != NULL) {
            matches++;
            if (flag_n) {
                fprintf(output, "%d: %s", line_number, buffer);
            } else {
                fputs(buffer, output);
            }
        }
        line_number++;
    }

    if (log != NULL) {
        fprintf(log, "pocet zpracovanych radku: %d, pocet shod: %d\n", lines_processed, matches);
        fclose(log);
    }

    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return 0;
}
