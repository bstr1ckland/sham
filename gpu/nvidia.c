#include <stdlib.h>
#include <stdio.h>
#include <nvml.h>
#include <math.h>
#include "nvidia.h"
#include "../utils/errors.h"

/**
 * Returns GPU Usage in percent. 
 */
double get_nvidia_gpu_usage()
{
    nvmlReturn_t result;
    nvmlDevice_t device;
    nvmlUtilization_t utilization;

    result = nvmlInit();
    if (result != NVML_SUCCESS) {
        printf("Failed to initialize NVML: %s\n", nvmlErrorString(result));
        return 1;
    }

    result = nvmlDeviceGetHandleByIndex(0, &device);
    if (result != NVML_SUCCESS) {
        printf("Failed to get handle for device 0: %s\n", nvmlErrorString(result));
        nvmlShutdown();
        return 1;
    }

    result = nvmlDeviceGetUtilizationRates(device, &utilization);
    if (result != NVML_SUCCESS) {
        printf("Failed to get utilization rates: %s\n", nvmlErrorString(result));
    } else {
        printf("GPU Utilization: %u%%\n", utilization.gpu);
        printf("Memory Utilization: %u%%\n", utilization.memory);
    }

    nvmlShutdown();
    return 0;
}


/**
 * Returns NVIDIA GPU VRAM installed in GBs.
 * Requires libnvidia-ml (NVML) and linking with -lnvidia-ml
 */
double nvidia_vram_total()
{
    nvmlReturn_t result;
    nvmlDevice_t device;
    unsigned int device_count;
    nvmlMemory_t memory;

    // Initialize NVML
    result = nvmlInit();
    if (result != NVML_SUCCESS) {
        fprintf(stderr, "Failed to initialize NVML: %s\n", nvmlErrorString(result));
        return -1;
    }

    result = nvmlDeviceGetCount(&device_count);
    if (result != NVML_SUCCESS || device_count == 0) {
        fprintf(stderr, "No NVIDIA GPU found: %s\n", nvmlErrorString(result));
        nvmlShutdown();
        return -1;
    }

    result = nvmlDeviceGetHandleByIndex(0, &device);
    if (result != NVML_SUCCESS) {
        fprintf(stderr, "Failed to get device handle: %s\n", nvmlErrorString(result));
        nvmlShutdown();
        return -1;
    }

    // Query memory info
    result = nvmlDeviceGetMemoryInfo(device, &memory);
    if (result != NVML_SUCCESS) {
        fprintf(stderr, "Failed to get memory info: %s\n", nvmlErrorString(result));
        nvmlShutdown();
        return -1;
    }

    // Clean up NVML
    nvmlShutdown();

    // Return total memory - later converted
    return memory.total;
}