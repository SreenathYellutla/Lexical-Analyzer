#include "header.h"
int main(int argc, char *argv[]) {
    Token token;

    // Validate the command-line arguments
    if (read_validate_arguments(argc, argv) == Failure) {
        return 1;
    }

    // Open the input and output files
    if (open_file(argv, &token) == Failure) {
        return 1;
    }

    printf("--- Lexical Analysis Started ---\n");
    if (scan_character(&token) == Success) {
        printf("--- Lexical Analysis Completed Successfully ---\n");
    } else {
        printf("--- Lexical Analysis Failed ---\n");
    }

    // Close files safely
    if (token.inputfp) fclose(token.inputfp);
    if (token.output_fp) fclose(token.output_fp);

    return 0;
}