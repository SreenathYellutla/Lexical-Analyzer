# C Lexical Analyzer

## 📌 Overview

The **C Lexical Analyzer** is a project developed using the C programming language to analyze source code and identify different types of tokens. It reads a C source file character by character, classifies tokens, and displays the analysis results in the terminal and an output file.

This project helps understand how a compiler's lexical analysis phase works.

## 🎯 Features

- **Command-Line Argument Validation** – Validates the input filename and `.c` extension.
- **File Handling** – Opens the input C file and creates an output file for analysis results.
- **Preprocessor Directives** – Identifies directives such as `#include` and `#define`.
- **Keyword Detection** – Identifies data-type and non-data-type keywords.
- **Identifier Detection** – Recognizes variable names, function names, and other identifiers.
- **Constant Detection** – Identifies integer constants.
- **Literal Detection** – Recognizes character and string literals.
- **Comment Detection** – Identifies single-line and multi-line comments.
- **Operator Detection** – Recognizes operators and compound operators.
- **Symbol Detection** – Identifies brackets, parentheses, and other symbols.
- **Line Number Tracking** – Tracks the line number of each token.
- **Colored Terminal Output** – Uses ANSI escape codes to highlight the table header.
- **Output File Generation** – Saves token analysis results in `Analysis_file.txt`.

## 🛠️ Technologies Used

- C Programming Language
- Standard C Libraries
- File Handling (`fopen`, `fgetc`, `fclose`)
- String Handling
- Command-Line Arguments
- ANSI Escape Codes

## 📂 Project Structure

```text
C-Lexical-Analyzer/
│
├── main.c
├── header.h
├── read_validate_arguments.c
├── open_file.c
├── scan_character.c
├── word_scan.c
├── preprocessor_scan.c
├── comment_scan.c
├── operator_scan.c
├── literal_scan.c
├── test.c
└── README.md
```

*Note: Adjust the filenames above to match the actual files in your repository.*

## ⚙️ How to Compile

Make sure GCC is installed on your system.

Compile all your project source files together:

```bash
gcc *.c -o lexer
```

If your project contains multiple test files with their own `main()` functions, compile only the source files required for the lexical analyzer instead of using `gcc *.c`.

## ▶️ How to Run

Run the program by passing a C source file as a command-line argument:

```bash
./lexer test.c
```

On successful execution, the program scans the input file and displays the identified tokens.

## 📄 Output

The output is displayed in a table containing:

| Column | Description |
|---|---|
| Line No | Line number where the token occurs |
| Token | Identified token text |
| Token Type | Classification of the token |

Example:

```text
Line No    Token           Token Type
---------------------------------------------
1          #include        PREPROCESSOR
2          int             KEYWORD_DATA
2          main            IDENTIFIER
2          (               OPEN_BRACE
2          10              INTEGER
2          return          KEYWORD_NON_DATA
```

The analysis results are also saved in:

```text
Analysis_file.txt
```

## 🧪 Test Cases

The project can be tested using a C file containing:

- Preprocessor directives
- Data types and keywords
- Identifiers
- Integer constants
- Character and string literals
- Single-line and multi-line comments
- Arithmetic, relational, logical, and compound operators
- Braces, parentheses, and semicolons
- Loops and conditional statements

## 📚 Concepts Learned

- Lexical analysis and token classification
- Character-by-character file scanning
- C strings and character handling
- Pointers and structures
- Command-line argument validation
- File operations and error handling
- Functions and modular programming
- Input validation and boundary conditions

## 🚀 Future Enhancements

- Improve detection of invalid tokens and malformed literals.
- Add support for floating-point and other numeric constants.
- Improve handling of escape sequences and compound operators.
- Add more comprehensive error reporting.
- Expand support for C preprocessor directives.
- Improve token output formatting and color presentation.

## 👨‍💻 Author

**Sreenath Yellutla**

Electronics and Communication Engineering | Embedded C | C Programming

## ⭐ Acknowledgment

This project was developed as a learning exercise to strengthen my understanding of C programming, file handling, and compiler fundamentals.

If you find this project useful, consider giving the repository a ⭐.
