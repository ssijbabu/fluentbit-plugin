#!/bin/bash

# Build script for Fluent Bit C plugins

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Building Fluent Bit C Plugins${NC}"

# Create build directory
if [ ! -d "build" ]; then
    mkdir -p build
    echo -e "${GREEN}✓ Created build directory${NC}"
fi

# Run CMake
cd build
echo -e "${YELLOW}Configuring CMake...${NC}"
cmake ..

# Build
echo -e "${YELLOW}Building plugins...${NC}"
make -j$(nproc)

echo -e "${GREEN}✓ Build completed successfully!${NC}"
echo -e "${GREEN}Plugins location: build/*/lib*.so${NC}"
