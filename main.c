#include <stdio.h>
#include "cpu/cpu_temp.h"

int main()
{
    int cpu_temp = get_cpu_temp();
    printf("CPU Tempurature: %d°C\n", cpu_temp);
    return 0;
}