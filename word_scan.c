#include "header.h"

// Function to identify keywords and identifiers from the input file
Status word_scan(char ch, Token *token)
{
    int i = 0;
    token->tok[i++] = ch;     // Store the first character of the word

    // Read the remaining characters of the word
    while (1)
    {
        int c = fgetc(token->inputfp);

        if (c == EOF)
            break;

            // Check whether the character is not a letter, digit, or underscore
        if (!isalnum((unsigned char)c) && c != '_')
        {
            ungetc(c, token->inputfp);
            break;
        }

        if (i < 49)
            token->tok[i++] = c;
    }

    token->tok[i] = '\0';

    // Check whether the word matches a data-type keyword
    for (int j = 0; res_kwords_data[j][0] != '\0'; j++)
    {
        // Compare the scanned word with each data-type keyword
        if (strcmp(token->tok, res_kwords_data[j]) == 0)
        {
            printf("%-10d %-15s : KEYWORD_DATA\n",
                   token->line, token->tok);
            fprintf(token->output_fp, "%-10d %-15s : KEYWORD_DATA\n",
                    token->line, token->tok);
            return Success;
        }
    }

     // Check whether the word matches a non-data-type keyword
    for (int j = 0; res_kwords_non_data[j][0] != '\0'; j++)
    {
        // Compare the scanned word with each non-data-type keyword
        if (strcmp(token->tok, res_kwords_non_data[j]) == 0)
        {
            printf("%-10d %-15s : KEYWORD_NON_DATA\n",
                   token->line, token->tok);
            fprintf(token->output_fp, "%-10d %-15s : KEYWORD_NON_DATA\n",
                    token->line, token->tok);
            return Success;
        }
    }

    // If the word is not a keyword, classify it as an identifier
    printf("%-10d %-15s : IDENTIFIER\n", token->line, token->tok);

    // Write the identifier to the output file
    fprintf(token->output_fp, "%-10d %-15s : IDENTIFIER\n",
            token->line, token->tok);

    return Success;
}