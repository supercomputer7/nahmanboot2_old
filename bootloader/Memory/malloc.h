#include <stddef.h>

void kmalloc_init();
void* kmalloc(size_t size);
inline void* operator new(size_t, void* p) { return p; }
inline void* operator new[](size_t, void* p) { return p; }

inline void operator delete(__attribute__((unused)) void *p,size_t);
inline void operator delete[](__attribute__((unused)) void *p,size_t);