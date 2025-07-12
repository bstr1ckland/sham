#include <stdio.h>
#include "cpu/cpu_temp.h"

int main()
{
    double cpu_temp = get_cpu_temp();
    printf("CPU Tempurature: %0.1f °C\n", cpu_temp);

    return 0;
}