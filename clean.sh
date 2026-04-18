#!/bin/bash

# Clean build artifacts

echo "Cleaning build artifacts..."

if [ -d "build" ]; then
    rm -rf build
    echo "✓ Removed build directory"
fi

# Clean CMake cache files
find . -name "CMakeFiles" -type d -exec rm -rf {} + 2>/dev/null || true
find . -name "CMakeCache.txt" -delete 2>/dev/null || true
find . -name "cmake_install.cmake" -delete 2>/dev/null || true

echo "✓ Cleaned successfully!"
