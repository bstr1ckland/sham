/*
 * File: nvidia_usage.c
 * Author: Ben Strickland
 * Date: 2025-08-13
 * Notes:
 *   NVIDIA systems have it's own header file
 */

#include <stdio.h>
#include <nvml.h>
#include "nvidia_usage.h"
#include "../utils/errors.h"

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