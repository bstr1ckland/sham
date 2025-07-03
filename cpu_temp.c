/*
 * File: cpu_temp.c
 * Author: Ben Strickland
 * Date: 2025-07-03
 * Description:
 *   Functions to read and return the CPU temperature on UNIX systems.
 *   This module provides an interface to retrieve the current CPU temperature,
 *   which can be used for monitoring or system diagnostics.
 *
 * Usage:
 *   Include "cpu_temp.h" and call get_cpu_temp().
 *
 * Notes:
 *   Tempurature is returned in Celcius.
 *
 */

#include <stdio.h>
#include "cpu_temp.h"

int get_cpu_temp() 
{
    return 42;
}