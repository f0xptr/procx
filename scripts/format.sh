#!/bin/bash
# Script to format C source code using clang-format.

# Find all C source and header files and format them in-place.
# Excludes files in the 'build' directory.
find . -type f \( -name "*.c" -o -name "*.h" \) \
  -not -path "./build/*" \
  -exec clang-format -style=file -i {} +

echo "Code formatting complete."
