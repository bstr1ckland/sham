/*
 * File: gpu_temp.c
 * Author: Ben Strickland
 * Date: 2025-07-13
 * Notes:
 *   For AMD and Intel GPUs, the following file path reads
 *   GPU usage in percent: /sys/class/drm/card1/device/gpu_busy_percent
 * 
 *   Reading /sys/class/drm/card1/device/vendor will tell us what our card is.
 *    0x8086 -> Intel
 *    0x1002 / 0x1022 -> AMD
 *    0x10de -> NVIDIA
 */

#include <stdio.h>
#include <stdlib.h>
#include "../utils/errors.h"

int get_gpu_usage()
{
    // Determines what kind of GPU the user has
    const char *path = "/sys/class/drm/card1/device/vendor";
    FILE *file = fopen(path, "r");
    validate_file(file);

    // Convert the hex string to an int
    unsigned int vendor_id;
    if(fscanf(file, "%x", &vendor_id) != 1)
    {
        perror("Failed to read GPU vendor ID");
        fclose(file);
        return -1;
    }
    /*
    int gpu_usage;
    switch(vendor_id)
    {
        // USE MACROS (#DEFINE) FOR CASES!!!
        case constant expression:
            break;
        default:
            break;
    }
    */

    return 69;
}