#pragma once
#include <stddef.h>
#include <Memory/Allocator.h>
#include <Memory/E820.h>

#define BumpAllocatorLocation 0x2000
#define DMABumpAllocatorLocation 0x2500
#define E820_MemoryMapLocationDescriptor 0x20000
#define DMARegionSize 16384
#define NumberOfDMARegions 200

void kmalloc_init();

void dma_alloc_init(uint32_t addr);
void* dma_calloc();
void* kmalloc(size_t size);
void* kcalloc(size_t size);
void* current_allocation();

inline void* operator new(size_t, void* p) { return p; }
inline void* operator new[](size_t, void* p) { return p; }

inline void operator delete(__attribute__((unused)) void *p,size_t);
inline void operator delete[](__attribute__((unused)) void *p,size_t);