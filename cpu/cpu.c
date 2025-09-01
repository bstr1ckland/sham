#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cpu.h"
#include "../utils/errors.h"
#include "../utils/find_hwmon.h"


/**
 * Returns the average speed of the CPU in MHz.
 */
double cpu_speed() {
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

/**
 * Returns the maximum speed for CPU in MHz
 */
double cpu_max_speed() {
    const char *path = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq";
    FILE *file = fopen(path, "r");
    if (!file) {
        perror("Failed to open cpuinfo_max_freq");
        return -1.0;
    }

    long khz;
    if (fscanf(file, "%ld", &khz) != 1) {
        fclose(file);
        return -1.0;
    }

    fclose(file);
    return (double)khz / 1000.0;
}


/**
 * Returns the average temperature of the CPU in celsius. 
 */
double cpu_temp() {
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
    if (fscanf(file, "%d", &temp_millideg) != 1) {
        perror("Failed to read CPU temperature");
        fclose(file);
        return -1;
    }

    fclose(file);

    // Convert millidegrees to degrees Celsius with decimal
    return temp_millideg / 1000.0;
}


/** 
 * Returns the average usage of the CPU in percentage.
 * 
 * Reference
 *   https://forums.raspberrypi.com/viewtopic.php?t=229992
 * 
 * Formula: CPU Utilization (%) = ((TOTAL - IDLE) / TOTAL) * 100
 *   IDLE = idle + iowait;
 *   USED = user + nice + system + irq + softirq + steal;
 *   TOTAL = IDLE + USED;
 *
 * /proc/stat file info (example):
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
typedef struct {
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
} cpu_snapshot;

static cpu_snapshot prev = {0};
static int first_call = 1;

int cpu_usage() {
    const char *path = "/proc/stat";
    cpu_snapshot current;

    FILE *file = fopen(path, "r");
    if (!file) return -1;
    fscanf(file, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
           &current.user, &current.nice, &current.system, &current.idle,
           &current.iowait, &current.irq, &current.softirq, &current.steal);
    fclose(file);

    if (first_call) {
        prev = current;
        first_call = 0;
        return 0; // no previous data to compare yet
    }

    unsigned long long idle_time = (current.idle + current.iowait) - (prev.idle + prev.iowait);
    unsigned long long used_time = (current.user + current.nice + current.system + 
                                    current.irq + current.softirq + current.steal) -
                                   (prev.user + prev.nice + prev.system + 
                                    prev.irq + prev.softirq + prev.steal);
    unsigned long long total_time = idle_time + used_time;

    prev = current;

    if (total_time == 0) return 0;
    return (int)(((double)used_time / total_time) * 100.0 + 0.5);
}