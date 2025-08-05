/*
 * File: gpu_temp.h
 * Author: Ben Strickland
 * Date: 2025-07-22
 */

#ifndef GPU_USAGE_H
#define GPU_USAGE_H

// Translated hex values
#define AMD_GPU1 4098
#define AMD_GPU2 4130
#define INTEL_GPU 32902
#define NVIDIA_GPU 4318

int get_gpu_usage(void);

#endif