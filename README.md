# QkScript - Command Interpreter Program

This C++ program implements a command interpreter that allows users to execute various commands interactively. It supports commands for printing, repeating actions, managing variables, file operations (open, write, close), and more.

## Features

- **Command Handling**: Processes user input to execute commands.
- **Variable Management**: Stores and replaces variables in command outputs.
- **File Operations**: Supports opening, writing to, and closing files.
- **Error Handling**: Provides error messages for invalid commands or file operations.

## Commands

The program recognizes the following commands:

- `print <message>`: Prints a formatted message with variable substitutions.
- `repeat <n> <command>`: Repeats a command `n` times.
- `var <name> = <value>`: Defines or updates a variable.
- `open <filename>`: Opens a file for writing.
- `write <content>`: Writes content to the currently open file.
- `close`: Closes the currently open file.

## Usage

1. **Compile**: Compile the program using a C++ compiler.
   
   ```bash
   g++ main.cpp -o command_interpreter
   ```

2. **Run**: Execute the compiled program.

   ```bash
   ./command_interpreter
   ```

3. **Commands**: Enter commands at the prompt. Example usage:
   
   ```plaintext
   command: var name = John
   command: print Hello, name
   Hello, John
   command: open output.txt
   command: write This is a test.
   command: close
   ```

## Notes

- **Exit**: To exit the program, enter `return`. Ensure no files are open before exiting.

