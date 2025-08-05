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
#include "gpu_usage.h"
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
    
    int gpu_usage;
    switch(vendor_id)
    {
        // AMD and Intel GPUs have the same filepath usage
        case INTEL_GPU:
        case AMD_GPU1:
        case AMD_GPU2:
            const char *amd_intel_path = "/sys/class/drm/card1/device/gpu_busy_percent";
            FILE *amd_intel_file = fopen(amd_intel_path, "r");

            if (fscanf(amd_intel_file, "%d", &gpu_usage) != 1)
            {
                perror("Failed to read GPU usage");
                fclose(file);
                return -1;
            }
            break;

        case NVIDIA_GPU:
            // Will have to make seperate file for this, as it requires its own header
            printf("nvidia\n");
            break;

        default:
            printf("Error: Your device is not supported, or GPU not found.\n");
            break;
    }

    fclose(file);
    return gpu_usage;
}