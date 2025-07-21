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
 *     TOTAL = IDLE + Used;
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
#include "../helpers/errors.h"

int get_cpu_usage()
{
    // This file is the reported CPU usage
    const char *path = "/proc/stat";
    FILE *file = fopen(path, "r");

    validate_file(file);

    // TODO: Read in data here for first reading. Use unsigned long long
    // Two readings are required to get usage over time - not instance

    // Sleep for a second and let the file update
    sleep(1); 

    fopen(path, "r");

    // TODO: Read in data here for second reading

    validate_file(file);
    
    
    fclose(file);
    return 69;
}