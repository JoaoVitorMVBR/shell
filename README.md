# Simple shell in C

This project implements a simple shell written in C, capable of interpreting and executing both internal (like 'cd', 'exit') and external (like  'ls', 'pwd') commands.

## Index

- [Overview](#overview)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [How to use](#how-to-use)
- [Internal Commands](#internal-commands)
- [Code Structure](#code-structure)
- [Contribution](#contribution)
- [License](#license)

## Overview

This simple shell is a command line program that allows the execution of operating system commands. It was developed as part of a project to reinforce knowledge of process manipulation, file systems and program execution in the Unix-like environment.

## Features

- **Reading commands**: Reads and interprets the command line entered by the user.
- **External command execution**: Allows the execution of commands available on the system, such as `ls', 'pwd', etc.
- **Internal commands**: Implements the internal commands `cd', 'help' and ’exit'.
- **Directory manipulation**: The command ’cd` allows you to navigate between directories, including options such as ’cd ~‘(home directory) and ’cd -‘(previous directory).

## Requirements

- Unix-like system (Linux or macOS).
- GCC compiler (or compatible).

## Installation

1. Clone this repository:
   `bash
   git clone https://github.com/JoaoVitorMVBR/shell

2. Enter the folder

3. Compile the file
    `bash
    gcc main.c -o sh.exe

## Internal Commands

- cd [directory]: Changes the working directory.
-help: Displays the list of built-in commands.
-exit: Closes the shell.

## Code Structure

- sh_loop: Main function that controls the shell execution cycle.
- sh_read_line: Reads the command line typed by the user.
- sh_split_line: Divides the command line into tokens for interpretation.
- sh_execute: Identifies and executes internal or external commands.
-sh_cd: Implementation of the internal cd command for directory browsing.
- sh_launch: Creates a child process to execute external commands using fork and execvp.

## Contribution

- Contributions are welcome! If you find a bug or have suggestions for improvements, please open an issue or send a pull request.
