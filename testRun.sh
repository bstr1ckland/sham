#!/bin/bash

# Check if nvml.h exists (NVIDIA NVML)
NVML_HEADER="/usr/include/nvidia/nvml.h"
USE_NVML=0
if [ -f "$NVML_HEADER" ]; then
    USE_NVML=1
fi

# Source files
SRC="main.c utils/errors.c utils/find_hwmon.c cpu/cpu.c gpu/gpu.c ram/ram.c"

# Compiler flags
CFLAGS=""
LDFLAGS="-lncurses"

if [ $USE_NVML -eq 1 ]; then
    SRC="$SRC gpu/nvidia_usage.c"
    CFLAGS="-DUSE_NVML"
    LDFLAGS="$LDFLAGS -lnvidia-ml"
fi

# Compile
gcc $SRC $CFLAGS -o main $LDFLAGS
if [ $? -ne 0 ]; then
    echo "Error: Could not compile program."
    exit 1
fi

# Run program
./main

# Clean up
rm main
