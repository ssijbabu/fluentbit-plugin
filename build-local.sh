#!/bin/bash
# Quick build and test script

set -e

echo "Building Fluent Bit plugins..."

# Clean previous builds
./clean.sh

# Create build directory
mkdir -p build
cd build

# Configure with cmake
cmake ..

# Build 
make -j$(nproc)

# Show built plugins
echo "✓ Build completed!"
echo ""
echo "Built plugins available at:"
find . -name "*.so" -type f

cd ..

echo ""
echo "To test with Docker:"
echo "  docker build -t fluentbit-plugins ."
echo "  docker run -it fluentbit-plugins"
