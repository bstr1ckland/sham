/*
 * File: cpu_temp.c
 * Author: Ben Strickland
 * Date: 2025-07-03
 * Notes:
 *   Temperature is returned in Celsius.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/errors.h"
#include "../utils/find_hwmon.h"

double get_cpu_temp() 
{
    // Find correct hwmon directory for the CPU
    char *hwmon_dir = find_hwmon("cpu");
    if (!hwmon_dir) {
        fprintf(stderr, "Could not find matching hwmon directory for CPU.\n");
        return -1;
    }

    // Construct path to temperature input file
    char path[512];
    snprintf(path, sizeof(path), "/sys/class/hwmon/%s/temp1_input", hwmon_dir);
    free(hwmon_dir); // cleanup

    FILE *file = fopen(path, "r");
    validate_file(file);

    int temp_millideg;
    if (fscanf(file, "%d", &temp_millideg) != 1) 
    {
        perror("Failed to read CPU temperature");
        fclose(file);
        return -1;
    }

    fclose(file);

    // Convert millidegrees to degrees Celsius with decimal
    return temp_millideg / 1000.0;
}