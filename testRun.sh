#!/bin/bash

# Add other files as needed
gcc main.c cpu/cpu_temp.c cpu/cpu_usage.c gpu/gpu_temp.c -o main 

# If GCC fails
if [ $? -ne 0 ]; then
    echo "Error: Could not compile program."
    break
fi

# Run the executable
./main