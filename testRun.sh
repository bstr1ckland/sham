#!/bin/bash

# Check if nvml.h exists (NVIDIA NVML)
NVML_HEADER="/usr/include/nvidia/nvml.h"
USE_NVML=0
if [ -f "$NVML_HEADER" ]; then
    USE_NVML=1
fi

SRC="main.c utils/errors.c utils/find_hwmon.c cpu/cpu.c gpu/gpu.c"

if [ $USE_NVML -eq 1 ]; then
    SRC="$SRC gpu/nvidia_usage.c"
    CFLAGS="-DUSE_NVML"
    LDFLAGS="-lnvidia-ml"
fi

gcc $SRC -o main
if [ $? -ne 0 ]; then
    echo "Error: Could not compile program."
    exit 1
fi

# Run program
./main

# Clean up
rm main