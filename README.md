# RVC Software Controller

## Intro
Building a Robot Vacuum Cleaner Software Controller with SASD

## Execution Environment

- **Operating System**: Linux 
- **Compiler**: GCC (GNU Compiler Collection) version 7.0 or later
- **C Standard**: C99
- **Required Libraries**:
  - **pthread** (required for multi-threading)
  - **criterion**: For unit testing the software logic
  - **stdio.h**, **stdlib.h**, **time.h**, **stdbool.h** (standard libraries)

The program creates two threads (`input_thread`, `controller_thread`) to handle sensor input and control motor/cleaner behavior based on given states.

### Developer Environment
- **Operating System**: Ubuntu 20.04 LTS 
- **Compiler**: GCC 10.5.0

## Makefile Specification

The project uses a `Makefile` to automate the build process. The `Makefile` includes targets to compile the program, clean the build files, and handle dependencies.

### Makefile Targets

#### Build Targets
- **`make`**: Compiles the source files (`main.c`, `rvc_sw.c`) into an executable (`main`)
- **`make test`**: Compiles the test files (`test.c`, `rvc_sw.c`) into a test executable (`test`) linked with the `criterion` library

#### Clean Targets
- **`make clean`**: Removes the compiled object files (`*.o`) and the main executable (`main`)
- **`make test_clean`**: Removes the test executable (`test`)
- **`make full_clean`**: Cleans up both build and test-related files

## Build and Run Instructions
### Build the Program
To compile the program, run the following command

    make

### Run the Program
After building the program, execute it with

    ./main

### Clean the Build
To clean the object files and executable, use

    make clean

## Testing Report
### Results Summary:
- All 26 tests passed successfully.
- No tests failed or crashed during the testing process.

### Screenshot
![test](https://github.com/user-attachments/assets/40a871cd-441b-4e15-a58e-bc0b0e179268)
