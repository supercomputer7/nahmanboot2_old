#pragma once
#include <stddef.h>
#include <Memory/Allocator.h>
#include <Memory/E820.h>

#define E820_MemoryMapLocationDescriptor 0x60000
#define DMARegionSize 16384
#define PageRegionSize 4096
#define NumberOfDMARegions 200

void kmalloc_init(uint32_t addr);
void kmalloc_aligned_init();

void cache_alloc_init(uint32_t addr);
void* cache_calloc(size_t size);

void dma_alloc_init(uint32_t addr);
void* dma_calloc();

void* kmalloc_aligned();
void* kcalloc_aligned();

void* kmalloc(size_t size);
void* kcalloc(size_t size);
void* current_allocation();

BumpAllocator* allocators();

inline void* operator new(size_t, void* p) { return p; }
inline void* operator new[](size_t, void* p) { return p; }

inline void operator delete(__attribute__((unused)) void* p);
inline void operator delete[](__attribute__((unused)) void* p);

inline void operator delete(__attribute__((unused)) void *p,__attribute__((unused)) size_t size);
inline void operator delete[](__attribute__((unused)) void *p,__attribute__((unused)) size_t size);