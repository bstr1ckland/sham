/*
 * File: cpu_speed.c
 * Author: Ben Strickland
 * Date: 2025-08-12
 * Notes:
 *   Speed is returned in MHz. or something
 */

#include <stdio.h>
#include <stdlib.h>
#include "../utils/errors.h"

double get_cpu_speed()
{
    // /proc/cpuinfo is CPU speed
    const char *path = "/proc/cpuinfo";
    FILE *file = fopen(path, "r");
    validate_file(file);

    // going to have to take all the core readings and then divide by count

    return 1.1;
}