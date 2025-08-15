#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cpu.h"
#include "../utils/errors.h"
#include "../utils/find_hwmon.h"

double cpu_speed()
{
    const char *path = "/proc/cpuinfo";
    FILE *file = fopen(path, "r");
    validate_file(file);

    char line[256];
    double total_mhz = 0.0;
    int core_count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "cpu MHz", 7) == 0) {
            double mhz = 0.0;
            if (sscanf(line, "cpu MHz\t: %lf", &mhz) == 1) {
                total_mhz += mhz;
                core_count++;
            }
        }
    }

    if(core_count == 0) return -1.0;

    fclose(file);
    return total_mhz / core_count;
}

double cpu_temp() 
{
    // Find correct hwmon directory for the CPU
    char *hwmon_dir = find_hwmon("cpu");
    if (!hwmon_dir) {
        fprintf(stderr, "Could not find matching hwmon directory for CPU.\n");
        return -1;
    }

    // Construct path to temperature input file
    char path[512];
    snprintf(path, sizeof(path), "/sys/class/hwmon/%s/temp1_input", hwmon_dir);
    free(hwmon_dir); // cleanup

    FILE *file = fopen(path, "r");
    validate_file(file);

    int temp_millideg;
    if (fscanf(file, "%d", &temp_millideg) != 1) 
    {
        perror("Failed to read CPU temperature");
        fclose(file);
        return -1;
    }

    fclose(file);

    // Convert millidegrees to degrees Celsius with decimal
    return temp_millideg / 1000.0;
}

/*
 * https://forums.raspberrypi.com/viewtopic.php?t=229992
 * 
 * Formula: CPU Utilization (%) = ((TOTAL - IDLE) / TOTAL) * 100
 *   IDLE = idle + iowait;
 *   USED = user + nice + system + irq + softirq + steal;
 *   TOTAL = IDLE + USED;
 *
 * /proc/stat file info:
 * cpu  user     nice    system    idle      iowait    irq    softirq    steal   guest  guest_nice
 *      204200   595     39028     4053660   3638      0      1222       0       0      0
 * 
 * user:       Time spent in user mode (not including nice time)
 * nice:       Time spent in user mode with low priority (nice value > 0)
 * system:     Time spent in kernel mode
 * idle:       Time spent idle (waiting for tasks)
 * iowait:     Time waiting for I/O operations to complete
 * irq:        Time servicing hardware interrupts
 * softirq:    Time servicing software interrupts
 * steal:      Time spent waiting for virtual CPU (when hypervisor is serving others)
 * guest:      Time running a virtual CPU (for guests under KVM/Xen) - Not needed
 * guest_nice: Time running a low-priority guest - Not needed
 */
int cpu_usage()
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

    // Avoid division by 0
    if(total_time == 0) return 0;

    return (int)(((double)used_time / total_time) * 100.0 + 0.5);;
}