#include <Memory/malloc.h>

#define General_Allocator   0x0
#define DMA_Allocator       0x1
#define Cache_Allocator     0x2
#define Aligned_Allocator   0x3

static BumpAllocator s_allocators[4];

void kmalloc_init(uint32_t addr)
{
    BumpAllocator& allocator = s_allocators[General_Allocator];
    e820map_descriptor* descriptor = (e820map_descriptor*)addr;
    e820map_entry_t* e820_map = (e820map_entry_t*)descriptor->map_addr;
    uint32_t i;
    for(i=0;i<descriptor->entries_count;++i)
        if(e820_map[i].length1 > 0x2500000 && e820_map[i].type == 0x1)
            break;
    allocator.initialize(((e820_map[i].base_addr1 + e820_map[i].length1) - (0x2000000)));
}
void kmalloc_aligned_init()
{
    BumpAllocator& allocator = s_allocators[Aligned_Allocator];
    allocator.initialize(0x50000);
}

void* kmalloc_aligned()
{
    size_t size = 4096;
    return (void*)s_allocators[Aligned_Allocator].allocate((uint32_t)size);
}
void* kcalloc_aligned()
{
    size_t size = 4096;
    uint8_t* tmp = (uint8_t*)s_allocators[Aligned_Allocator].allocate((uint32_t)size);
    for(int i=0; i<(int)size;i++)
        tmp[i] = 0;
    return (void*)tmp;
}

void cache_alloc_init(uint32_t addr)
{
    BumpAllocator& allocator = s_allocators[Cache_Allocator];
    e820map_descriptor* descriptor = (e820map_descriptor*)addr;
    e820map_entry_t* e820_map = (e820map_entry_t*)descriptor->map_addr;
    uint32_t i;
    for(i=0;i<descriptor->entries_count;++i)
        if(e820_map[i].length1 > 0x1000000 && e820_map[i].type == 0x1)
            break;
    allocator.initialize(((e820_map[i].base_addr1 + e820_map[i].length1) - (0x200000)));
}
void* cache_calloc(size_t size)
{   
    uint8_t* tmp = (uint8_t*)s_allocators[Cache_Allocator].allocate((uint32_t)size);
    for(int i=0; i<(int)size;++i)
        tmp[i] = 0;
    return (void*)tmp;
}

void dma_alloc_init(uint32_t addr)
{
    BumpAllocator& allocator = s_allocators[DMA_Allocator];
    e820map_descriptor* descriptor = (e820map_descriptor*)addr;
    e820map_entry_t* e820_map = (e820map_entry_t*)descriptor->map_addr;
    uint32_t i;
    for(i=0;i<descriptor->entries_count;++i)
        if(e820_map[i].length1 > 0x1000000 && e820_map[i].type == 0x1)
            break;
    allocator.initialize(((e820_map[i].base_addr1 + e820_map[i].length1) - (DMARegionSize*NumberOfDMARegions)));
}

void* dma_calloc()
{
    uint8_t* tmp = (uint8_t*)s_allocators[DMA_Allocator].allocate((uint32_t)DMARegionSize);
    for(int i=0; i<(int)DMARegionSize;++i)
        tmp[i] = 0;
    return (void*)tmp;
}

void* kmalloc(size_t size)
{
    return (void*)s_allocators[General_Allocator].allocate((uint32_t)size);
}
void* kcalloc(size_t size)
{
    uint8_t* tmp = (uint8_t*)s_allocators[General_Allocator].allocate((uint32_t)size);
    for(int i=0; i<(int)size;i++)
        tmp[i] = 0;
    return (void*)tmp;
}
void* current_allocation()
{
    return (void*)s_allocators[General_Allocator].get_addr();
}

BumpAllocator* allocators()
{
    return s_allocators;
}

void *operator new(size_t size)
{
    return kmalloc(size);
}
 
void *operator new[](size_t size)
{
    return kmalloc(size);
}

inline void operator delete(__attribute__((unused)) void* p)
{

}
inline void operator delete[](__attribute__((unused)) void* p)
{

}
 
void operator delete(__attribute__((unused)) void *p,__attribute__((unused))size_t size)
{
    
}
 
void operator delete[](__attribute__((unused)) void *p,__attribute__((unused))size_t size)
{
    
}