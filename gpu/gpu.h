/*
 * File: gpu.h
 * Author: Ben Strickland
 * Date: 2025-08-15
 * Description: Header for GPU temperature and usage functions.
 */

#ifndef GPU_H
#define GPU_H

// Translated hex values
#define AMD_GPU1 4098
#define AMD_GPU2 4130
#define INTEL_GPU 32902
#define NVIDIA_GPU 4318

double gpu_temp(void);
int gpu_usage(void);

#endif 