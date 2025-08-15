/*
 * File: gpu.h
 * Author: Ben Strickland
 * Date: 2025-08-15
 * Description: Header for CPU temperature, usage functions and speed.
 */

#ifndef CPU_H
#define CPU_H

double cpu_temp(void);
int cpu_usage(void);
double cpu_speed(void);
double cpu_max_speed(void);

#endif 