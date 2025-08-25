#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpu.h"
#include "nvidia.h"
#include "../utils/errors.h"
#include "../utils/find_hwmon.h"


/**
 * Returns the temperature of the GPU in celsius.
 */
double gpu_temp() 
{
    // Find correct hwmon directory for the CPU
    char *hwmon_dir = find_hwmon("gpu");
    if (!hwmon_dir) {
        fprintf(stderr, "Could not find matching hwmon directory for GPU.\n");
        return -1;
    }

    // Construct path to temperature input file
    char path[512];
    snprintf(path, sizeof(path), "/sys/class/hwmon/%s/temp1_input", hwmon_dir);
    free(hwmon_dir); // cleanup

    FILE *file = fopen(path, "r");
    validate_file(file);

    int temp_millideg;
    if(fscanf(file, "%d", &temp_millideg) != 1) 
    {
        perror("Failed to read CPU temperature");
        fclose(file);
        return -1;
    }

    fclose(file);

    // Convert millidegrees to degrees Celsius with decimal
    return temp_millideg / 1000.0;
}


/**
 * Returns GPU usage in percentage. NVIDIA systems have it's own
 * functionality, explaining the exclusivity
 */
int gpu_usage()
{
    // Determines what kind of GPU the user has
    // So this is kinda hard coded but can change with later versions
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

            if(fscanf(amd_intel_file, "%d", &gpu_usage) != 1)
            {
                perror("Failed to read GPU usage");
                fclose(file);
                return -1;
            }
            break;

        case NVIDIA_GPU:
            gpu_usage = get_nvidia_gpu_usage();
            break;

        default:
            printf("Error: Your device is not supported, or GPU not found.\n");
            break;
    }

    fclose(file);
    return gpu_usage;
}


/**
 * Returns GPU VRAM installed in the GPU in GBs.
 */
double vram_total()
{
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
    
    unsigned long long bytes;
    switch(vendor_id)
    {
        case AMD_GPU1:
        case AMD_GPU2:
            const char *amd_path = "/sys/class/drm/card1/device/mem_info_vram_total";
            FILE *amd_file = fopen(amd_path, "r");
            validate_file(file);

            if(fscanf(amd_file, "%llu", &bytes) != 1) {
                perror("Failed to read total VRAM");
                fclose(amd_file);
                return -1;
            }
            fclose(amd_file);
            break;

        case INTEL_GPU:
            perror("Error: Intel GPU does not have VRAM support");
            return -1;
            break;

        case NVIDIA_GPU:
            bytes = nvidia_vram_total();
            break;
    }

    fclose(file);

    // Convert bytes -> GB
    return bytes / pow(1024.0, 3);
}


/**
 * Returns GPU VRAM actively used in the GPU in GBs.
 */
double vram_used() 
{
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
    
    unsigned long long bytes;
    switch(vendor_id)
    {
        case AMD_GPU1:
        case AMD_GPU2:
            const char *amd_path = "/sys/class/drm/card1/device/mem_info_vram_used";
            FILE *amd_file = fopen(amd_path, "r");
            validate_file(amd_file);

            if(fscanf(amd_file, "%llu", &bytes) != 1)
            {
                perror("Failed to read used VRAM");
                fclose(amd_file);
                return -1;
            }
            break;

        case INTEL_GPU:
            perror("Error: Intel GPU does not have VRAM support");
            return -1;
            break;

        case NVIDIA_GPU:
            bytes = nvidia_vram_total();
            break;
    }

    fclose(file);

    // Convert bytes -> GB
    return bytes / pow(1024.0, 3);
}