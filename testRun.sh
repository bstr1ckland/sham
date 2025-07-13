#!/bin/bash

# Add other files as needed
gcc main.c cpu/cpu_temp.c gpu/gpu_temp.c -o main 

# If GCC fails
if [ $? -ne 0 ]; then
    echo "Error: Could not compile program. Installing GCC now"
    sudo apt install gcc -y
fi

# Run the executable
./main