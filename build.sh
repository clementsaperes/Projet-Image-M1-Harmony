#!/bin/bash
set -e

if [ "$1" == "--clean" ]; then
    echo "Cleaning build directory..."
    rm -rf build
fi

mkdir -p build
cd build
cmake ..
cmake --build . -j$(nproc)