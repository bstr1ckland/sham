#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ram.h"
#include "../utils/errors.h"


/**
 * Helper function to target info in /proc/meminfo
 */
static unsigned long read_meminfo_value(const char *key) {
    const char *path = "/proc/meminfo";
    FILE *file = fopen(path, "r");
    validate_file(file);

    char line[256];
    char label[64];
    unsigned long value = 0;

    while (fgets(line, sizeof(line), file)) {
        // Read label and value, ignore the unit
        if (sscanf(line, "%63[^:]: %lu %*s", label, &value) == 2) {
            if (strcmp(label, key) == 0) {
                fclose(file);
                return value; // in kB
            }
        }
    }

    fclose(file);
    return 0;
}


/**
 * Returns metric specified by @info
 */
double ram_info(const char *info) {
    unsigned long wanted_info = read_meminfo_value(info);
    if(!wanted_info) return -1;
    return (double)wanted_info / pow(1024, 2); // Convert to GB
}