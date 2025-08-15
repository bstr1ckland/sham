/*
 * File: main.c
 * Author: Ben Strickland
 * Date: 2025-07-03
 * Notes:
 *   Look into ncurses library for tui (text user interface)
 */

#include <stdio.h>
#include <sys/sysinfo.h>
#include "cpu/cpu.h"
#include "gpu/gpu.h"
#include "utils/find_hwmon.h"

int main()
{
    // CPU
    double cpuTemp = cpu_temp(); 
    int cpuUsage = cpu_usage();
    double cpuSpeed = cpu_speed();

    // GPU
    double gpuTemp = gpu_temp();
    int gpuUsage = gpu_usage();
    double vramUsed = vram_used();
    double vramTotal = vram_total();
    
    // Uptime
    struct sysinfo info;
    sysinfo(&info);

    int uptime_hours = info.uptime / 3600;
    int uptime_minutes = (info.uptime % 3600) / 60;
    int uptime_seconds = info.uptime % 60;

    // Print metrics
    printf("CPU Temperature: %0.1f°C\n", cpuTemp);
    printf("CPU Usage: %d%%\n", cpuUsage);
    printf("CPU Speed: %0.1fMHz\n\n", cpuSpeed);

    printf("GPU Temperature: %0.1f°C\n", gpuTemp);
    printf("GPU Usage: %d%%\n", gpuUsage);
    printf("GPU VRAM Total: %.1f GB\n", vramTotal);
    printf("GPU VRAM Used: %.1f GB\n", vramUsed);

    double vramPercent = (vramUsed / vramTotal) * 100.0;
    printf("GPU VRAM Used Percent: %.1f%%\n\n", vramPercent);

    printf("System uptime: %dh %dm %ds\n", uptime_hours, 
            uptime_minutes, uptime_seconds);
    
    return 0;
}