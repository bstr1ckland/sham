/*
 * Make helper function that will find the right 'hwmon' file for the 
 * temp input of the target hardware. 
 * @param will be the target hardware, "cpu", "gpu"
 * 
 * TODO: switch on the input param, and have cases that will find
 * the hwmonX directory that contains "k10temp", "amdgpu", etc.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

char* find_hwmon(const char* target)
{
    const char* file_path = "/sys/class/hwmon";

    DIR *dir = opendir(file_path);
    struct dirent *entry;
    const char* name_path;

    if(!dir) perror("Error: Failed to open directory.\n");

    while ((entry = readdir(dir)) != NULL)
    {
        // Skip . and .. 'directories'
        if (strcmp(entry -> d_name, ".") == 0 || strcmp(entry -> d_name, "..") == 0) continue;

        // add checks for amdgpu, k10temp, etc.
        
        const char* file_name;
        
        // entry -> d_name is the current working directory
        //snprintf(name_path, sizeof(name_path), "%s/%s", file_path, entry -> d_name);


    }



    closedir(dir);
    return "hello";
    
}