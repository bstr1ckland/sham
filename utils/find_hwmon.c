#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

/**
 * Returns the directory that the wanted hardware is located.
 * @param target - hardware type being searched for.
 */
char* find_hwmon(const char* target) {
    const char* hwmon_root = "/sys/class/hwmon";
    DIR *dir = opendir(hwmon_root);
    struct dirent *entry;

    if (!dir) {
        perror("Error opening /sys/class/hwmon");
        return NULL;
    }

    // Array to store different types of hardwares
    const char* matches[3] = {NULL, NULL, NULL};

    if (strcmp(target, "cpu") == 0) {
        matches[0] = "k10temp";   // AMD CPU
        matches[1] = "coretemp";  // Intel CPU
    } else if (strcmp(target, "gpu") == 0) {
        matches[0] = "amdgpu";    // AMD GPU
        matches[1] = "i915";      // Intel GPU
        matches[2] = "nvidia";    // NVIDIA GPU
    } else {
        fprintf(stderr, "Unsupported target: %s\n", target);
        closedir(dir);
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Ignore current and prev dir's
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry -> d_name, "..") == 0)
            continue;

        // Large buffer to avoid warnings
        char name_file_path[512];
        
        snprintf(name_file_path, sizeof(name_file_path), "%s/%s/name", 
                 hwmon_root, entry -> d_name);

        FILE *name_file = fopen(name_file_path, "r");
        if (name_file) {
            char hw_name[128];
            if (fgets(hw_name, sizeof(hw_name), name_file)) {
                hw_name[strcspn(hw_name, "\n")] = '\0'; // Strip newline

                for (int i = 0; i < 3; i++) 
                {
                    if (matches[i] && strcmp(hw_name, matches[i]) == 0) 
                    {
                        fclose(name_file);
                        closedir(dir);

                        return strdup(entry -> d_name);
                    }
                }
            }
            fclose(name_file);
        }
    }

    closedir(dir);
    return NULL;
}