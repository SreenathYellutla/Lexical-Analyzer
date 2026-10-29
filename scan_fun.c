#include "header.h"
#include <ctype.h>
#include <string.h>
/*
 * Scans and validates preprocessor directives such as #include and #define.
 * Identifies the directive and routes header files or macro names to their scanners.
 * Reports invalid directives or missing header delimiters.
 */
Status preprocessor_scan(int ch, Token *token)
{
    int i = 0;
    int c;
    int j;

    token->tok[i++] = ch;

    if (ch != '#')
        return Failure;

    /* Read the directive */
    while (i < sizeof(token->tok) - 1){
        c = fgetc(token->inputfp);

        if (c == EOF || c == ' ' || c == '\t' ||
            c == '\n' || c == '\r' ||
            c == '<' || c == '"')
            break;

        token->tok[i++] = c;
    }

    token->tok[i] = '\0';

    /* Validate directive */
    for (j = 0; directives[j] != NULL; j++)
    {
        if (strcmp(token->tok, directives[j]) == 0)
            break;
    }
    //if directives not found it prints error message and returns failure
    if (directives[j] == NULL){
        printf("%-10d %-15s : PREPROCESSOR ERROR\n",token->line, token->tok);

        fprintf(token->output_fp,"%-10d %-15s : PREPROCESSOR ERROR\n",token->line, token->tok);

        return Failure;
    }
    printf("%-10d %-15s : PREPROCESSOR\n",token->line, token->tok);

    fprintf(token->output_fp,"%-10d %-15s : PREPROCESSOR\n",token->line, token->tok);

    /* Handle #include separately */
    if (strcmp(token->tok, "#include") == 0)
    {
        while (c == ' ' || c == '\t')
            c = fgetc(token->inputfp);
        /*after reading the directive, 
        * it checks for the next character to be either '<' or '"',
        * which are valid delimiters for header files. 
        * If neither is found, it reports a preprocessor error 
        * indicating missing header delimiters and returns failure.
        */
        if (c != '<' && c != '"')
        {
            printf("%-10d %-15s : PREPROCESSOR ERROR\n",token->line, "Missing header delimiters");

            fprintf(token->output_fp,"%-10d %-15s : PREPROCESSOR ERROR\n",
                    token->line, "Missing header delimiters");
            //ungetc is used to put back the character
            if (c != EOF && c != '\n')
                ungetc(c, token->inputfp);

            return Failure;
        }
        //Checking after header valiation
        if (header_scan(c, token) == Failure)
            return Failure;
    }
    /* Handle #define */
    else if (strcmp(token->tok, "#define") == 0)
    {
        while (c == ' ' || c == '\t')
            c = fgetc(token->inputfp);
        // it checks if the next character is a valid starting character for a macro name 
        if (macro_scan(c, token) == Failure)
            return Failure;
    }
    else
    {
        /* Leave other directives to the normal scanner */
        if (c != EOF)
            ungetc(c, token->inputfp);
    }

    return Success;
}



Status comment_scan(char ch, Token *token)
{
    int c;
    int prev = 0;
    int i = 0;
    int closed = 0;
    // Store the first '/' character
    token->tok[i++] = ch;
     // Read the next character to identify the comment type
    c = fgetc(token->inputfp);
    /* Single-line comment: // */
    if (c == '/')
    {
        token->tok[i++] = c;
        // Read characters until newline or end of file
        while ((c = fgetc(token->inputfp)) != EOF && c != '\n')
        {
            if (i < 48)
                token->tok[i++] = c;
        }
        // Terminate the comment string
        token->tok[i] = '\0';
         // Display the single-line comment
        printf("%-10d %-25s : SINGLE_LINE_COMMENT\n",token->line,token->tok);
        fprintf(token->output_fp,"%-10d %-25s : SINGLE_LINE_COMMENT\n",token->line,token->tok);
         // Update the line number if a newline was consumed
        if (c == '\n')
            token->line++;
        return Success;
    }
    // Multi-line comment: /* ... */
    if (c == '*')
    {
         // Store the opening comment delimiter
        token->tok[i++] = c;
        while ((c = fgetc(token->inputfp)) != EOF)
        {
            if (c == '\n')
                token->line++;

            if (i < 48)
                token->tok[i++] = c;
              // Detect the closing delimiter: */
            if (prev == '*' && c == '/')
            {
                closed = 1;
                break;
            }
            // Remember the previous character for delimiter detection
            prev = c;
        }

        token->tok[i] = '\0';
         // Report an error if EOF occurs before the closing delimiter
        if (!closed)
        {
            printf("%-10d %-25s : ERROR - Unterminated comment\n",
                   token->line, token->tok);

            fprintf(token->output_fp,
                    "%-10d %-25s : ERROR - Unterminated comment\n",
                    token->line, token->tok);

            return Failure;
        }

        printf("%-10d %-25s : MULTI_LINE_COMMENT\n",
               token->line, token->tok);

        fprintf(token->output_fp,
                "%-10d %-25s : MULTI_LINE_COMMENT\n",
                token->line, token->tok);

        return Success;
    }

    /* Not a comment: restore the lookahead character */
    if (c != EOF)
        ungetc(c, token->inputfp);

    token->tok[0] = '\0';

    return Failure;
}

Status header_scan(char ch, Token *token)
{
    int i = 0;
    int c;
    char close;
    
    if (ch == '<')
        close = '>';    // Header format: <stdio.h>
    else
        close = '"';    // Header format: "stdio.h"
    
    token->tok[i++] = ch;

    // Read header characters until the closing delimiter or an error
    while (i < 49)
    {
        c = fgetc(token->inputfp);

        if (c == EOF || c == '\n')
            break;

        // Store the current character in the token buffer
        token->tok[i++] = c;

        // Stop when the expected closing delimiter is found
        if (c == close)
            break;
    }
    // Add the null terminator to make token a valid C string
    token->tok[i] = '\0';

    // Report an error if the closing delimiter is missing
    if(c!=close){
        printf("%-10d %-15s : HEADER ERROR\n",
               token->line, token->tok);

        fprintf(token->output_fp,
                "%-10d %-15s : HEADER ERROR\n",
                token->line, token->tok);

        return Failure;
    }
    // Return Failure if the last character is not the expected closing delimiter
    if (token->tok[i - 1] != close)
        return Failure;

    printf("%-10d %-15s : HEADER_FILE\n",
           token->line, token->tok);

    fprintf(token->output_fp,
            "%-10d %-15s : HEADER_FILE\n",
            token->line, token->tok);

    return Success;
}


Status macro_scan(char ch, Token *token)
{
    int i = 0;
    int c = ch;

    // Skip whitespace before the macro name
    while (c == ' ' || c == '\t')
        c = fgetc(token->inputfp);

    // Check whether the macro name is missing
    if (c == '\n' || c == EOF)
    {
        printf("%-10d %-15s : MACRO ERROR\n",
               token->line, "Missing macro name");

        fprintf(token->output_fp,
                "%-10d %-15s : MACRO ERROR\n",
                token->line, "Missing macro name");
         // Update the line number if a newline was consumed
        if (c == '\n')
            token->line++;

        return Failure;
    }

    // A macro name must start with a letter or underscore
    if (!isalpha((unsigned char)c) && c != '_')
    {
        printf("%-10d %-15s : MACRO ERROR\n",
               token->line, "Invalid macro name");

        fprintf(token->output_fp,
                "%-10d %-15s : MACRO ERROR\n",
                token->line, "Invalid macro name");

        return Failure;
    }

    token->tok[i++] = c;

     // Read the remaining characters of the macro name
    while (i < 49)
    {
        c = fgetc(token->inputfp);

        if (c == EOF)
            break;

        // Macro names can contain only letters, digits, and underscores
        if (!isalnum((unsigned char)c) && c != '_')
            break;

        // Store the valid character in the token buffer
        token->tok[i++] = c;
    }
    // Add the null terminator to complete the macro-name string for printing macro
    token->tok[i] = '\0';

    printf("%-10d %-15s : MACRO_NAME\n",
           token->line, token->tok);

    fprintf(token->output_fp,
            "%-10d %-15s : MACRO_NAME\n",
            token->line, token->tok);
    // Return the delimiter to the input stream for the next scanner
    if (c != EOF)
        ungetc(c, token->inputfp);

    return Success;
}

// Check whether the given string matches any compound operator
int is_compound_operator_match(char *op_str) {
    // Iterate through all compound operators
    for(int i = 0; i < sizeof(compound_operators) / sizeof(compound_operators[0]); i++) {
         // Compare the input operator with the current compound operator
        if(strcmp(op_str, compound_operators[i]) == 0)
            return 1;
    }
    return 0;
}
// Check whether the given character is a valid symbol
Status is_symbol(char ch, Token *token) {
    for(int i = 0; i < sizeof(symbols); i++) {
        if(ch == symbols[i])
            return Success;
    }
    return Failure;
}

/* Check whether a character is part of an operator */
Status is_operator(char ch, Token *token) {
    // First check compound operators using lookahead
    int next_ch = fgetc(token->inputfp);
    char op_str[3] = {ch, next_ch, '\0'};

    // Check whether the two characters form a compound operator
    if(is_compound_operator_match(op_str)) {
        printf("%-10d %-15s : COMPOUND_OPERATOR\n", token->line, op_str);
        fprintf(token->output_fp, "%-10d %-15s : COMPOUND_OPERATOR\n", token->line, op_str);
        return Success;
    } else {
        ungetc(next_ch, token->inputfp);

         // Check whether the current character is a single-character operator
        for(int i = 0; i < sizeof(operators); i++) {
            if(ch == operators[i]) {
                printf("%-10d %-15c : OPERATOR\n", token->line, ch);
                fprintf(token->output_fp, "%-10d %-15c : OPERATOR\n", token->line, ch);
                return Success;
            }
        }
    }
    return Failure;
}

/* Check whether the current character starts a compound operator */
Status is_compound_operator(char ch, Token *token) {
    // Wrapper helper function matching header declaration
    int next_ch = fgetc(token->inputfp);

    // Combine both characters to form a possible compound operator
    char op_str[3] = {ch, next_ch, '\0'};
    ungetc(next_ch, token->inputfp);

    // Return Success if the combination is a valid compound operator
    return is_compound_operator_match(op_str) ? Success : Failure;
}

/* Scan and identify an integer literal */
Status is_Integer(char ch, Token *token) {
    int i = 0;
    token->tok[i++] = ch;

    // Read and store subsequent digits
    while((ch = fgetc(token->inputfp)) != EOF && isdigit(ch)) {
        if(i < 49)
            token->tok[i++] = ch;
    }
    token->tok[i] = '\0';
    ungetc(ch, token->inputfp);

    printf("%-10d %-15s : INTEGER\n", token->line, token->tok);
    fprintf(token->output_fp, "%-10d %-15s : INTEGER\n",token->line, token->tok);
    return Success;
}

/* Scan and identify a character literal */
Status is_character(char ch, Token *token) {
    int i = 0;
    token->tok[i++] = ch; // opening quote

    while((ch = fgetc(token->inputfp)) != EOF) {
        if(i < 49)
            token->tok[i++] = ch;
        if(ch == '\'')
            break;
    }
    token->tok[i] = '\0';

    printf("%-10d %-15s : CHARACTER_LITERAL\n", token->line, token->tok);
    fprintf(token->output_fp, "%-10d %-15s : CHARACTER_LITERAL\n", token->line, token->tok);
    return Success;
}

/* Scan and identify a string literal */
Status is_string_literal(char ch, Token *token) {
    int i = 0;
    token->tok[i++] = ch; // opening quote

    while((ch = fgetc(token->inputfp)) != EOF) {
        if(ch == '\n')
            (token->line)++;
        if(i < 49)
            token->tok[i++] = ch;

        // Stop when the closing double quote is found
        if(ch == '"')
            break;
    }
    token->tok[i] = '\0';

    printf("%-10d %-15s : STRING_LITERAL\n", token->line, token->tok);
    fprintf(token->output_fp, "%-10d %-15s : STRING_LITERAL\n", token->line, token->tok);
    return Success;
}

/* Check whether the character is a statement terminator */
Status is_terminating_operator(char ch, Token *token) {
    if(ch == ';') {
        printf("%-10d %-15c : TERMINATOR\n", token->line, ch);
        fprintf(token->output_fp, "%-10d %-15c : TERMINATOR\n", token->line, ch);
        return Success;
    }
    return Failure;
}
void print_box_header(Token *token)
{
    fprintf(token->output_fp,"+---------+----------------+-----------------------------+\n");
    fprintf(token->output_fp,"| LINE NO | TOKEN          | TOKEN TYPE                  |\n");
    fprintf(token->output_fp,"+---------+----------------+-----------------------------+\n");
    printf(CYAN);
    printf("+---------+----------------+-----------------------------+\n");
    printf("| LINE NO | TOKEN          | TOKEN TYPE                  |\n");
    printf("+---------+----------------+-----------------------------+\n");
    printf(RESET);
}
void print_box_row(int line, char *token, char *type)
{
    printf("| %-8d | %-15s | %-29s |\n",line, token, type);
}
void print_box_footer(Token *token)
{
    fprintf(token->output_fp,"+----------+---------------+--------------------------+\n");
    printf(CYAN);
    printf("+----------+---------------+--------------------------+\n");
    printf(RESET);
}