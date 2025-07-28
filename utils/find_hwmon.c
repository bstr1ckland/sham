/*
 * Make helper function that will find the right 'hwmon' file for the 
 * temp input of the target hardware. 
 * @param will be the target hardware, "cpu", "gpu"
 * 
 * TODO: switch on the input param, and have cases that will find
 * the hwmonX directory that contains "k10temp", "amdgpu", etc.
 * 
 */

char* find_hwmon(const char* target)
{
    const char *path = "/sys/class/hwmon";

    /*
     * TODO: 
     * 1. loop through each subdir
     * 2. check the @name file in each sub-dir and compare with target
     * 3. return if found
     */ 


    return 0;
}