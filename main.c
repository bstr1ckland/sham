#include <stdio.h>
#include "cpu/cpu_temp.h"
#include <sys/sysinfo.h>

int main()
{
    double cpu_temp = get_cpu_temp();
    printf("CPU Temperature: %0.1f °C\n", cpu_temp);

    struct sysinfo info;
    sysinfo(&info);

    // Make variables that get the hours, 
    // minutes, seconds of uptime
    printf("Uptime = %ld\n", info.uptime);

    return 0;
}