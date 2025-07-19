/*
 * File: gpu_temp.c
 * Author: Ben Strickland
 * Date: 2025-07-13
 * Notes:
 *   Temperature is returned in Celsius.
 */

#include <stdio.h>
#include <stdlib.h>

double get_gpu_temp() 
{
    // This file is the reported GPU temperature
    const char *path = "/sys/class/hwmon/hwmon1/temp1_input";
    FILE *file = fopen(path, "r");

    if (!file) 
    {
        perror("Failed to open temperature file");
        return -1;
    }

    int temp_millideg;
    if (fscanf(file, "%d", &temp_millideg) != 1) 
    {
        perror("Failed to read temperature");
        fclose(file);
        return -1;
    }

    fclose(file);

    // Convert millidegrees to degrees Celsius with decimal
    return temp_millideg / 1000.0;
}