# Variable definitions
CC = gcc               # Compiler
CFLAGS = -pthread -std=c99  # Compilation options
SRC = main.c            # Source file
OBJ = main.o            # Object file
EXEC = main             # Final executable file name

# Default target
all: $(EXEC)

# Rule to create the executable file
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(CFLAGS)

# Rule to create the object file
$(OBJ): $(SRC) rvc_sw.h
	$(CC) $(CFLAGS) -c $(SRC)

# Rule to clean up built files
clean:
	rm -f $(OBJ) $(EXEC)
