# Makefile for the ProcX project.
# This Makefile automates the compilation, linking, and testing processes.

# Compiler to use
CC = gcc
# CFLAGS are compiler flags:
#   -Wall: Enable all standard warnings
#   -Wextra: Enable extra warnings
#   -O2: Optimize for speed
#   -Iinclude: Add the 'include' directory to the search path for header files
CFLAGS = -Wall -Wextra -O2 -Iinclude
# LDFLAGS are linker flags:
#   -lncursesw: Link with the ncursesw library for wide-character UI functionalities
LDFLAGS = -lncursesw

# Directories for source and object files
SRC_DIR = src
OBJ_DIR = build

# Source files (all .c files that need to be compiled)
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/system/sys_info.c \
       $(SRC_DIR)/system/process_list.c \
       $(SRC_DIR)/ui/display.c

# Object files (automatically generated from source files, placed in OBJ_DIR)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# The final executable target name
TARGET = procx

# Default target: builds the main executable
all: $(TARGET)

# Rule to link object files into the final executable
$(TARGET): $(OBJS)
	@mkdir -p $(@D) # Create the build directory if it doesn't exist
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to compile each source file into an object file
# This is a pattern rule: for any .c file in SRC_DIR, compile it into a .o file in OBJ_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D) # Create the subdirectory in build if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@ # $< is the prerequisite (source file), $@ is the target (object file)

# Target for running unit tests
test:
	# Compile test_sys_info.c and sys_info.c into a test_runner executable
	$(CC) tests/test_sys_info.c src/system/sys_info.c -o test_runner -Iinclude
	./test_runner # Execute the test runner

# Target to clean up generated files
clean:
	rm -rf $(OBJ_DIR) $(TARGET) test_runner # Remove all object files, the executable, and the test runner

# Phony targets are targets that do not correspond to actual files.
# This prevents Make from getting confused if a file with the same name exists.
.PHONY: all clean test
