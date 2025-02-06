# Assembly Language Processor in C

This project is a **macro assembler** written in C that processes assembly source files, expands macros, and generates machine code. It was developed as part of a **C programming course project** by **Almog Mordechai** and **Raz Hershko**.

## 🚀 Features
- ✅ **Macro expansion** - Handles macro definitions and replaces them in code.
- ✅ **Two-pass assembler** - Parses and processes the assembly instructions in two stages.
- ✅ **Symbol table management** - Stores and tracks labels and variables.
- ✅ **Error detection** - Identifies syntax errors and reserved word misuse.
- ✅ **Output file generation** - Creates `.ob`, `.ent`, and `.ext` files.

## 📁 Project Structure
| File | Description |
|------|------------|
| **MACRO.c** | Processes macros and manages macro storage. |
| **MAIN.c** | Main entry point of the program. |
| **STAGE_ONE.c** | First pass: Parses assembly instructions, labels, and directives. |
| **STAGE_TWO.c** | Second pass: Generates machine code and symbol tables. |
| **TO_FILE.c** | Manages file output for assembled code. |
| **HELP.c** | Utility functions for parsing and processing. |
| **LISTS.c** | Handles linked lists for managing instructions. |
| **main.h** | Header file with function prototypes and definitions. |
| **makefile** | Automates the compilation process. |

## ⚙️ Installation and Compilation
### Prerequisites
Ensure you have:
- A **C compiler** (e.g., GCC or Clang)
- **make utility** (for automated compilation)

### Compilation
Use the included `makefile` to compile:
```sh
make
```
This will generate an executable named **main**.

### Cleaning Up
To remove compiled files:
```sh
make clean
```

## 🖥️ Usage
Run the assembler with an assembly source file:
```sh
./main file.as
```
This will process `file.as` and output:
- **file.ob** - Machine code
- **file.ent** - Entry symbols
- **file.ext** - External symbols

### Example
To process `example.as`, run:
```sh
./main example.as
```
Expected output files:
- `example.ob`
- `example.ent`
- `example.ext`

## 📝 Code Example
### Input (`example.as`):
```assembly
mcr HELLO
    mov r1, r2
endmcr

HELLO
```

### Expanded Output (`after_macro.am`):
```assembly
mov r1, r2
```

### Machine Code Output (`example.ob`):
```
0001 0010 1100
0002 1011 0000
```

## ❌ Error Handling
- If an **invalid macro name** is used:
  ```
  Error: You are trying to deploy a macro name that is not allowed.
  ```
- If a **file does not exist**:
  ```
  Error Macro: cannot open the source file
  ```
- If **memory allocation fails**:
  ```
  Memory allocation failed in macro.
  ```

## 🎯 Project Goal
This assembler was created to **convert a high-level assembly language into machine code** while handling **macros, labels, and instructions efficiently**. It is part of a **C programming course project** to provide hands-on experience with low-level programming concepts.

## 🤝 Contributing
Want to contribute? Follow these steps:
1. **Fork the repository** on GitHub.
2. **Clone your forked repo**:
   ```sh
   git clone https://github.com/your-username/your-repo.git
   ```
3. **Create a new branch**:
   ```sh
   git checkout -b feature-branch
   ```
4. **Commit changes** and push:
   ```sh
   git add .
   git commit -m "Added new feature"
   git push origin feature-branch
   ```
5. **Open a Pull Request** and wait for review!
