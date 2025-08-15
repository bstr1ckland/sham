#ifndef NVIDIA_USAGE_H
#define NVIDIA_USAGE_H

#ifdef USE_NVML
int get_nvidia_gpu_usage();
#else
// If NVML not available
static inline int get_nvidia_gpu_usage() {
    return -1;
}
#endif

#endif