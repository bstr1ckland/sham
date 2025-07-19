/*
 * File: cpu_usage.c
 * Author: Ben Strickland
 * Date: 2025-07-19
 * Notes:
 *  https://forums.raspberrypi.com/viewtopic.php?t=229992
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int get_cpu_usage()
{
    // This file is the reported CPU usage
    const char *path = "/proc/stat";
    FILE *file = fopen(path, "r");

    if(!file)
    {
        perror("Failed to open usage file");
        return -1;
    }

    // Sleep for a second and let the file update
    sleep(1); 

    fopen(path, "r");

    if(!file)
    {
        perror("Failed to open usage file");
        return -1;
    }

    // TODO: Take data from this file - store in vars and then apply formula

    return 69;
}