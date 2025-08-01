/*
 * File: errors.c
 * Author: Ben Strickland
 * Date: 2025-07-20
 */

#include <stdio.h>
#include <stdlib.h>

void validate_file(FILE *file)
{
    if(!file)
    {
        fclose(file);
        perror("Failed to open file. Check compatibility");
        exit(EXIT_FAILURE);
    }
}