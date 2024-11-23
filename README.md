# RVC Software Controller

## Intro
Building a Robot Vacuum Cleaner Software Controller with SASD

## Execution Environment

- **Operating System**: Linux 
- **Compiler**: GCC (GNU Compiler Collection) version 7.0 or later
- **Required Libraries**:
  - **pthread** (required for multi-threading)
  - **stdio.h**, **stdlib.h**, **time.h**, **stdbool.h** (standard libraries)

The program creates two threads (`input_thread`, `controller_thread`) to handle sensor input and control motor/cleaner behavior based on given states.

### Developer Environment
- **Operating System**: Ubuntu 20.04 LTS 
- **Compiler**: GCC 10.5.0

## Makefile Specification

The project uses a `Makefile` to automate the build process. The `Makefile` includes targets to compile the program, clean the build files, and handle dependencies.

### Makefile Targets

- **`make`**: Compiles the `main.c` file and creates the `main` executable.
- **`make clean`**: Removes the compiled object files (`main.o`) and the executable (`main`).
