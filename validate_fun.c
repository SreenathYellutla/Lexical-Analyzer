#include "header.h"

Status read_validate_arguments(int argc, char *argv[]) {
    // Check whether exactly one command-line argument is provided
    if (argc != 2) {
        printf("Usage: ./lexer <input_file.c>\n");
        return Failure;
    }

    // Check whether the filename is NULL or empty
    if (argv[1] == NULL || argv[1][0] == '\0') 
        return Failure;

    // Reject filenames that start with a dot
    if (argv[1][0] == '.') 
        return Failure;

    // Check if the filename ends with ".c"
    char *ch = strstr(argv[1], ".c");
    if (ch == NULL || strcmp(ch, ".c") != 0) {
        printf("ERROR: File must be a valid .c file\n");
        return Failure;
    }

   // Count the number of dots in the filename
    int i = 0, c = 0;
    while (argv[1][i] != '\0') {
        if (argv[1][i] == '.') {
            c++;
        }
        i++;
    }
    // If you want to allow only one dot (e.g. main.c), keep this check:
    if (c > 1) {
        printf("ERROR: Invalid filename format\n");
        return Failure;
    }

    return Success;
}

Status open_file(char *argv[], Token *token) {
    token->input_file_name = argv[1];
    token->inputfp = fopen(token->input_file_name, "r");
    if (token->inputfp == NULL) {
        printf("ERROR: Opening input file failed\n");
        return Failure;
    }
    printf("Entered file : %s\n", token->input_file_name);

    // Assign output filename and open file pointer correctly
    token->output_file_name = "Analysis_file.txt";
    token->output_fp = fopen(token->output_file_name, "w");
    if (token->output_fp == NULL) {
        printf("ERROR: Opening output file failed\n");
        fclose(token->inputfp); // Clean up input file if output fails
        return Failure;
    }
    printf("Output File  : %s\n\n", token->output_file_name);
    
    return Success;
}

