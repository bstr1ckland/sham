/*
 * File: main.c
 * Author: Ben Strickland
 * Date: 2025-07-03
 * Notes:
 *   Look into ncurses library for tui
 */

#include <stdio.h>
#include <sys/sysinfo.h>
#include "cpu/cpu_temp.h"
#include "cpu/cpu_usage.h"
#include "gpu/gpu_temp.h"
#include "gpu/gpu_usage.h"

int main()
{
    // CPU
    double cpu_temp = get_cpu_temp();
    int cpu_usage = get_cpu_usage();

    // GPU
    double gpu_temp = get_gpu_temp();
    int gpu_usage = get_gpu_usage();
    
    // Uptime
    struct sysinfo info;
    sysinfo(&info);

    int uptime_hours = info.uptime / 3600;
    int uptime_minutes = (info.uptime % 3600) / 60;
    int uptime_seconds = info.uptime % 60;

    // Print messages
    printf("CPU Temperature: %0.1f°C\n", cpu_temp);
    printf("CPU USAGE: %d%%\n", cpu_usage);
    printf("GPU Temperature: %0.1f°C\n", gpu_temp);
    printf("GPU Vendor ID: %d\n", gpu_usage);
    printf("System uptime: %dh %dm %ds\n", uptime_hours, 
            uptime_minutes, uptime_seconds);
    
    return 0;
}