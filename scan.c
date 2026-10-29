#include "header.h"
#include <ctype.h>

Status scan_character(Token *token)
{
    int ch;
    token->line = 1;
    printf("\n");
    print_box_header(token);     // Display the header of the output box

    // Read characters from the input file until the end of the file
    while ((ch = fgetc(token->inputfp)) != EOF)
    {
        /* Handle Windows line endings (\r\n) */
        if (ch == '\r')
        {
            int next = fgetc(token->inputfp);

            if (next == '\n')
            {
                token->line++;
                continue;
            }

            if (next == EOF)
                break;

            ch = next;
        }

        /* Count new lines */
        if (ch == '\n')
        {
            token->line++;
            continue;
        }

        /* Skip spaces and tabs 
        if (ch == ' ' || ch == '\t')
            continue;
        */

        //Preprocessor directive 
        if (ch == '#')
        {
            if (preprocessor_scan(ch, token) != Success)
                return Failure;
        }
        // Comments 
        else if (ch == '/')
        {
            if (comment_scan(ch, token) != Success)
                return Failure;
        }
        // Identifiers and keywords 
        else if (isalpha((unsigned char)ch) || ch == '_')
        {
            if (word_scan(ch, token) != Success)
                return Failure;
        }

        //Integer constants 
        else if (isdigit((unsigned char)ch))
        {
            if (is_Integer(ch, token) != Success)
                return Failure;
        }
        // Character literals 
        else if (ch == '\'')
        {
            if (is_character(ch, token) != Success)
                return Failure;
        }
        // String literals 
        else if (ch == '"')
        {
            if (is_string_literal(ch, token) != Success)
                return Failure;
        }
        // Semicolon 
        else if (ch == ';')
        {
            if (is_terminating_operator(ch, token) != Success)
                return Failure;
        }
        // Symbols 
        else if (is_symbol(ch, token) == Success)
        {
            if (ch == '(' || ch == '{' || ch == '[')
            {
                printf("%-10d %-15c : OPEN_BRACE\n", token->line, ch);
                fprintf(token->output_fp,
                        "%-10d %-15c : OPEN_BRACE\n", token->line, ch);
            }
            else if (ch == ')' || ch == '}' || ch == ']')
            {
                printf("%-10d %-15c : CLOSE_BRACE\n", token->line, ch);
                fprintf(token->output_fp,
                        "%-10d %-15c : CLOSE_BRACE\n", token->line, ch);
            }
            else
            {
                printf("%-10d %-15c : SYMBOL\n", token->line, ch);
                fprintf(token->output_fp,
                        "%-10d %-15c : SYMBOL\n", token->line, ch);
            }
        }
        // Operators
        else if (is_operator(ch, token) == Success)
        {
            continue;
        }
    }
    print_box_footer(token);
    return Success;
}