#include <stdio.h>
#include "cpu/cpu_temp.h"
#include <sys/sysinfo.h>

int main()
{
    // CPU
    double cpu_temp = get_cpu_temp();
    
    // Uptime
    struct sysinfo info;
    sysinfo(&info);

    int uptime_hours = info.uptime / 3600;
    int uptime_minutes = (info.uptime % 3600) / 60;
    int uptime_seconds = info.uptime % 60;

    // Print messages
    printf("CPU Temperature: %0.1f°C\n", cpu_temp);
    printf("System uptime: %dh %dm %ds\n", uptime_hours, 
            uptime_minutes, uptime_seconds);

    return 0;
}