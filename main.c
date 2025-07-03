#include <stdio.h>
#include "cpu_temp.h"

int main()
{
    int cpu_temp = get_cpu_temp();
    printf("CPU Tempurature: %dC\n", cpu_temp);
    return 0;
}