#!/bin/bash

# Add other files as needed
gcc main.c utils/errors.c cpu/cpu_temp.c cpu/cpu_usage.c gpu/gpu_temp.c gpu/gpu_usage.c -o main 

if [ $? -ne 0 ]; then
    echo "Error: Could not compile program."
    rm a.out
    break
fi

# Run the executable
./main

# Remove main executable from directory after ran
rm main