#!/bin/bash
# Script to run clang-tidy on the C source code.

# Ensure the build directory exists for clang-tidy to find compile_commands.json
# For this to work best, a compilation database (compile_commands.json)
# should be generated, e.g., using 'bear make' or CMake.
# As this project uses a simple Makefile, we will just run clang-tidy directly
# on source files for a basic check, assuming the default clang-tidy config (.clang-tidy).

echo "Running clang-tidy for static analysis..."

# Find all C source files and run clang-tidy.
# Excludes files specified in .clang-format-ignore (or similar build outputs).
find src -type f -name "*.c" -exec clang-tidy {} -- -Iinclude \;

echo "Clang-tidy analysis complete."
