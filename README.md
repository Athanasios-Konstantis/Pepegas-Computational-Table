# Pepegas Computational Table

*A simple "Excel-like" table application for the Windows command-line*

## Overview

Pepegas Computational Table is a lightweight spreadsheet-style application that runs in the Windows CMD environment. Written in C as a first-year engineering group project, it aims to give a quick, simple way to manipulate tabular data without the overhead of a full GUI spreadsheet tool.

## Features

- A console-based grid interface where you can enter data into cells.

- Basic functionality to view, edit and save cell contents (text and numbers).

- Keyboard navigation (arrow keys) between cells.

- Built to demonstrate foundational programming and data-structure skills in C.

- Stand-alone executable (for easy testing) plus full source code included.


## Getting Started

### Prerequisites

- Windows OS (tested on Windows 10)
- Command Prompt (CMD) access
- (Optional) A C compiler if you want to rebuild from source

### Running the Executable

1. Download or clone this repository

2. In the repository folder, locate the `Executable.exe` file

3. In CMD, navigate to that folder and run `Executable.exe`

4. The program should launch; follow the on-screen instructions to interact with the table

### Building from Source

If you prefer to compile the program yourself:

1. Ensure you have a C compiler installed (e.g., MinGW, Visual Studio C, or another compatible).

2. Open a command prompt in the repository folder.

3. Run: `gcc Main_Code.c -o PepegasTable.exe` (Or use your compiler’s standard build command.)

4. Execute the resulting PepegasTable.exe.

## Usage

- Use your mouse or the custom command line arguments to move between cells.

- Enter numeric values into a selected cell.

- Use the appropriate key (e.g., Enter) to commit the cell’s content.

- Use a custom command line argument to execute a mathematical operation

- The application has a menu on key instructions in the interface — please refer to those while running by typing `help`

## Project Context

This project was developed during our first year in the engineering programme, as a group of two students. Its main goal was educational: to practice designing a data table structure, handling input/output in the console, and implementing basic user-interaction logic. While it does not aim to match full spreadsheet software feature-sets, it serves as a fun demonstration of how table-like tools can be implemented at a low-level.

## Acknowledgments

Thanks to my classmate (co-developer) Thanasis Chouklis for collaborating on this project, and to the instructors in the first-year engineering course for providing the inspiration and environment to build this tool.