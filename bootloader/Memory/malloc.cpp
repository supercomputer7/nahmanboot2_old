#include <Memory/malloc.h>

void kmalloc_init()
{
    BumpAllocator* allocator = (BumpAllocator*)(0x2000);
    allocator->initialize(0x300000);
}
void* kmalloc(size_t size)
{
    return (void*)((BumpAllocator*)(0x2000))->allocate((uint32_t)size);
}
void* kcalloc(size_t size)
{
    uint8_t* tmp = (uint8_t*)((BumpAllocator*)(0x2000))->allocate((uint32_t)size);
    for(int i=0; i<(int)size;i++)
        tmp[i] = 0;
    return (void*)tmp;
}
void* current_allocation()
{
    return (void*)((BumpAllocator*)(0x2000))->get_addr();
}

void *operator new(size_t size)
{
    return kmalloc(size);
}
 
void *operator new[](size_t size)
{
    return kmalloc(size);
}
 
void operator delete(__attribute__((unused)) void *p,size_t size)
{
    
}
 
void operator delete[](__attribute__((unused)) void *p,size_t size)
{
    
}