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