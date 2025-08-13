#!/bin/bash

# Check if nvml.h exists (NVIDIA NVML)
NVML_HEADER="/usr/include/nvidia/nvml.h"
USE_NVML=0
if [ -f "$NVML_HEADER" ]; then
    USE_NVML=1
    echo "NVIDIA NVML detected, compiling NVIDIA support."
else
    echo "No NVIDIA NVML detected, skipping NVIDIA support."
fi

# Compile flags
CFLAGS=""
LDFLAGS=""
SRC="main.c utils/errors.c utils/find_hwmon.c cpu/cpu_temp.c cpu/cpu_usage.c cpu/cpu_speed.c gpu/gpu_temp.c gpu/gpu_usage.c"

if [ $USE_NVML -eq 1 ]; then
    SRC="$SRC gpu/nvidia_usage.c"
    CFLAGS="-DUSE_NVML"
    LDFLAGS="-lnvidia-ml"
fi

gcc $SRC $CFLAGS $LDFLAGS -o main
if [ $? -ne 0 ]; then
    echo "Error: Could not compile program."
    exit 1
fi

# Run program
./main

# Clean up
rm main