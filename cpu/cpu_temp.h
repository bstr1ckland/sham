/*
 * File: cpu_temp.h
 * Author: Ben Strickland
 * Date: 2025-07-03
 * Description:
 *   Header file for cpu_temp.c. Declares the interface for
 *   retrieving the current CPU temperature, intended for
 *   use in system monitoring.
 *
 * Usage:
 *   Include this header in any source file that needs access to the
 *   get_cpu_temp() function:
 *
 *       #include "cpu_temp.h"
 *
 *   Then call get_cpu_temp() to retrieve the temperature in degrees Celsius.
 */


#ifndef CPU_TEMP_H
#define CPU_TEMP_H

double get_cpu_temp(void);

#endif