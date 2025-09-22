#!/bin/bash

# Create build directory
mkdir -p build
cd build

# Run cmake and make
cmake ..
make -j$(nproc)

echo "Build complete! Run with: ./focusNexus [file.fn] or ./focusNexus for REPL"