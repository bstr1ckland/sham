/*
 * File: cpu_usage.c
 * Author: Ben Strickland
 * Date: 2025-07-19
 * Notes:
 *   https://forums.raspberrypi.com/viewtopic.php?t=229992
 * 
 *   Formula: CPU Utilization (%) = ((TOTAL - IDLE) / TOTAL) * 100
 *     IDLE = idle + iowait;
 *     USED = user + nice + system + irq + softirq + steal;
 *     TOTAL = IDLE + USED;
 *
 *   /proc/stat file info:
 *   cpu  user     nice    system    idle      iowait    irq    softirq    steal   guest  guest_nice
 *        204200   595     39028     4053660   3638      0      1222       0       0      0
 * 
 *   user:       Time spent in user mode (not including nice time)
 *   nice:       Time spent in user mode with low priority (nice value > 0)
 *   system:     Time spent in kernel mode
 *   idle:       Time spent idle (waiting for tasks)
 *   iowait:     Time waiting for I/O operations to complete
 *   irq:        Time servicing hardware interrupts
 *   softirq:    Time servicing software interrupts
 *   steal:      Time spent waiting for virtual CPU (when hypervisor is serving others)
 *   guest:      Time running a virtual CPU (for guests under KVM/Xen) - Not needed
 *   guest_nice: Time running a low-priority guest - Not needed
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../utils/errors.h"

double get_cpu_usage()
{
    // This file is the reported CPU usage
    const char *path = "/proc/stat";

    // Two readings are required to get usage over time - not instance
    unsigned long long user1, nice1, system1, idle1, iowait1, irq1, softirq1, steal1;
    unsigned long long user2, nice2, system2, idle2, iowait2, irq2, softirq2, steal2;

    FILE *file = fopen(path, "r");
    validate_file(file);
    fscanf(file, "cpu %llu %llu %llu %llu %llu %llu %llu %llu", &user1, &nice1, 
            &system1, &idle1, &iowait1, &irq1, &softirq1, &steal1);
    fclose(file);

    // Sleep for a second and let the file update
    sleep(1); 
    
    file = fopen(path, "r");
    validate_file(file);
    fscanf(file, "cpu %llu %llu %llu %llu %llu %llu %llu %llu", &user2, &nice2, 
            &system2, &idle2, &iowait2, &irq2, &softirq2, &steal2);
    fclose(file);

    // Create variables as described above to do calculation
    unsigned long long idle_time = (idle2 + iowait2) - (idle1 + iowait1);
    unsigned long long used_time = (user2 + nice2 + system2 + irq2 + softirq2 + steal2) 
            - (user1 + nice1 + system1 + irq1 + softirq1 + steal1);
    unsigned long long total_time = idle_time + used_time;

    if(total_time == 0)
    {
        return 0;
    }

    return ((double)(total_time - idle_time) / total_time) * 100.0;
}