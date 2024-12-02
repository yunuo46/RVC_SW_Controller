# Variable definitions
CC = gcc					# Compiler
CFLAGS = -pthread -std=c99  # Compilation options
SRC = main.c rvc_sw.c		# Source files
OBJ = main.o rvc_sw.o		# Object files
EXEC = main					# Final executable file name
TSSRC = test.c rvc_sw.c     # Test source file
TSBIN = test				# Test executable file name
TSAGS = -lcriterion			# Test options

# Default target
all: $(EXEC)

# Rule to create the executable file
$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(CFLAGS)

# Rule to create the object files
main.o: main.c rvc_sw.h
	$(CC) -c main.c

rvc_sw.o: rvc_sw.c rvc_sw.h
	$(CC) -c rvc_sw.c

# Rule to clean up built files
clean:
	rm -f $(OBJ) $(EXEC)

# Rule to compile and link the test executable
test: $(TSSRC) rvc_sw.h
	$(CC) -o $(TSBIN) $(TSSRC) $(TSAGS)

# Rule to clean up test files
test_clean:
	rm -f $(TSBIN)

# Clean everything
full_clean: clean test_clean
