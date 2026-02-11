# PySUB Interpreter

PySUB is a custom interpreter written in C++ that executes a Python-subset scripting language. It features a complete pipeline including lexical analysis, expression evaluation (using infix-to-postfix conversion), and an interpreter engine that handles control flow and variable management.

## Features

* **Interactive REPL Interface**: Run commands line-by-line or load entire script files.
* **Mathematical Expression Evaluator**: Handles complex arithmetic (`+`, `-`, `*`, `/`, `%`) and logical operations (`and`, `or`, `not`) with proper order of operations.
* **Control Flow**: Full support for indentation-based blocks including:
    * `if`, `elif`, `else` conditionals.
    * `while` loops.
    * `for` loops (iterating via `range()`).
* **Variable Management**: A symbol table system to store and retrieve integer variables.
* **Built-in Functions**: Support for `print()`, `input()`, and `int()` casting.

## Technical Architecture

The project is split into several modular components:

* **`interface.cpp`**: Manages the Command Line Interface (CLI), help system, and file loading.
* **`lexAnalysis.cpp`**: The Lexer. It breaks raw source code into tokens (Keywords, Identifiers, Operators) and specifically handles whitespace/indentation to define scope.
* **`expevaluator.cpp`**: The Parser/Evaluator. It converts infix expressions to postfix notation (RPN) using a stack-based algorithm to ensure correct precedence before solving.
* **`interpreter.cpp`**: The Execution Engine. It walks through the tokenized code, manages the program state (loops, conditionals), and executes logic blocks.

## Getting Started

### Prerequisites
* A C++ Compiler (GCC, Clang, or MSVC).
* Standard C++ libraries (no external dependencies required).

### Compilation
You can compile the project using `g++` or your preferred IDE.

```bash
g++ main.cpp interface.cpp lexAnalysis.cpp expevaluator.cpp interpreter.cpp -o PySUB
