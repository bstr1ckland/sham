#ifndef NVIDIA_H
#define NVIDIA_H

#ifdef USE_NVML
double get_nvidia_gpu_usage();
double nvidia_vram_total();
double nvidia_vram_used();
#else
// Fallback stubs if NVML not available
static inline double get_nvidia_gpu_usage() { return -1.0; }
static inline double nvidia_vram_total()   { return -1.0; }
static inline double nvidia_vram_used()    { return -1.0; }
#endif

#endif // NVIDIA_H
