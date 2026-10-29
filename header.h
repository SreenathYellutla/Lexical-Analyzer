#ifndef HEADER_H
#define HEADER_H
#define YELLOW  "\033[1;33m"
#define CYAN  "\033[1;36m"
#define RESET "\033[0m"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct 
{
    char *input_file_name;      // Stores the name or path of the input file
    FILE *inputfp;              // File pointer used to read the input file

    char *output_file_name;     // Stores the name or path of the output file
    FILE *output_fp;            // File pointer used to read the output file

    int line;                   // Tracks the current line number in the input file
    char tok[50];               // Buffer to store the current token (maximum 49 characters + '\0')
} Token;

typedef enum {
    Failure,
    Success
} Status;

static char* res_kwords_data[] = {"const", "volatile", "extern", "auto", "register", "static",
                    "signed", "unsigned", "int", "double", "char", "short", "long",
                    "float", "struct", "union", "void", "enum", "typedef", ""};

static char *res_kwords_non_data[] = { "goto", "continue", "return", "break",
                    "if", "else", "for", "while", "do", "switch",
                    "case", "sizeof", "default", ""
                    }; 

static char *directives[] = {"#include", "#define","#if","#ifdef", "#ifndef", "#endif",
    "#else","#elif", "#undef", "#error", "#pragma",NULL};

static char *compound_operators[] = {"==", "!=", "|=", "&=", "^=", "<=", ">=", "+=", "-=", "*=", "/=", "%=", 
    "<<", ">>", "++", "--", "->"}; 

static char operators[] = {'+', '-', '*', '/', '%', '=', '>', '<', '!', '~', '&', '|', ',', '^'};

static char symbols[] = {'(', ')', '{', '}', '[', ']', ':', ';'}; 

Status read_validate_arguments(int argc, char *argv[]);
Status open_file(char *argv[], Token *token);
Status scan_character(Token *token);
Status header_scan(char ch, Token *token);
Status macro_scan(char ch, Token *token);
Status preprocessor_scan(int ch, Token *token);
Status comment_scan(char ch, Token *token);
Status word_scan(char ch, Token *token);
Status is_symbol(char ch, Token *token);
Status is_operator(char ch, Token *token);
Status is_compound_operator(char ch, Token *token);
Status is_Integer(char ch, Token *token);
Status is_character(char ch, Token *token);
Status is_string_literal(char ch, Token *token);
Status is_terminating_operator(char ch, Token *token);
void print_box_header(Token *token);
void print_box_footer(Token *token);

#endif