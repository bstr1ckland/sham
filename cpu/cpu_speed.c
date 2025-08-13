/*
 * File: cpu_speed.c
 * Author: Ben Strickland
 * Date: 2025-08-12
 * Notes:
 *   Speed is returned in MHz.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/errors.h"

double get_cpu_speed()
{
    const char *path = "/proc/cpuinfo";
    FILE *file = fopen(path, "r");
    validate_file(file);

    char line[256];
    double total_mhz = 0.0;
    int core_count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "cpu MHz", 7) == 0) {
            double mhz = 0.0;
            if (sscanf(line, "cpu MHz\t: %lf", &mhz) == 1) {
                total_mhz += mhz;
                core_count++;
            }
        }
    }

    if(core_count == 0) return -1.0;

    fclose(file);

    return total_mhz / core_count;
}