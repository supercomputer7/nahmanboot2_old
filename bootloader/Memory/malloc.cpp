#include <Memory/malloc.h>

void kmalloc_init()
{
    BumpAllocator* allocator = (BumpAllocator*)(BumpAllocatorLocation);
    allocator->initialize(0x300000);
}

void cache_alloc_init(uint32_t addr)
{
    BumpAllocator* allocator = (BumpAllocator*)(CacheBumpAllocationLocation);
    e820map_descriptor* descriptor = (e820map_descriptor*)addr;
    e820map_entry_t* e820_map = (e820map_entry_t*)descriptor->map_addr;
    uint32_t i;
    for(i=0;i<descriptor->entries_count;++i)
        if(e820_map[i].length1 > 0x1000000 && e820_map[i].type == 0x1)
            break;
    allocator->initialize(((e820_map[i].base_addr1 + e820_map[i].length1) - (0x200000)));
}
void* cache_calloc(size_t size)
{
    uint8_t* tmp = (uint8_t*)((BumpAllocator*)(CacheBumpAllocationLocation))->allocate((uint32_t)size);
    for(int i=0; i<(int)size;++i)
        tmp[i] = 0;
    return (void*)tmp;
}

void dma_alloc_init(uint32_t addr)
{
    BumpAllocator* allocator = (BumpAllocator*)(DMABumpAllocatorLocation);
    e820map_descriptor* descriptor = (e820map_descriptor*)addr;
    e820map_entry_t* e820_map = (e820map_entry_t*)descriptor->map_addr;
    uint32_t i;
    for(i=0;i<descriptor->entries_count;++i)
        if(e820_map[i].length1 > 0x1000000 && e820_map[i].type == 0x1)
            break;
    allocator->initialize(((e820_map[i].base_addr1 + e820_map[i].length1) - (DMARegionSize*NumberOfDMARegions)));
}

void* dma_calloc()
{
    uint8_t* tmp = (uint8_t*)((BumpAllocator*)(DMABumpAllocatorLocation))->allocate((uint32_t)DMARegionSize);
    for(int i=0; i<(int)DMARegionSize;++i)
        tmp[i] = 0;
    return (void*)tmp;
}

void* kmalloc(size_t size)
{
    return (void*)((BumpAllocator*)(BumpAllocatorLocation))->allocate((uint32_t)size);
}
void* kcalloc(size_t size)
{
    uint8_t* tmp = (uint8_t*)((BumpAllocator*)(BumpAllocatorLocation))->allocate((uint32_t)size);
    for(int i=0; i<(int)size;i++)
        tmp[i] = 0;
    return (void*)tmp;
}
void* current_allocation()
{
    return (void*)((BumpAllocator*)(BumpAllocatorLocation))->get_addr();
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