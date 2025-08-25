#ifndef NVIDIA_USAGE_H
#define NVIDIA_USAGE_H

#ifdef USE_NVML
int get_nvidia_gpu_usage();
double nvidia_vram_total();
double nvidia_vram_used();
#else
// If NVML not available
static inline int get_nvidia_gpu_usage() {
    return -1;
}
static inline double nvidia_vram_total() {
    return -1;
}
static inline double nvidia_vram_used() {
    return -1;
}
#endif
#endif