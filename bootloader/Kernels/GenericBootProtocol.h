#pragma once
#include <stdint.h>
#include <LibC/stdstring.h>
#include <LibC/stdbool.h>
#include <Memory/E820.h>
#include <Display/Protocols/FramebufferResolution.h>
class GenericBootProtocol {

public:
    virtual void boot(__attribute__((unused)) void* loaded_file,__attribute__((unused)) e820map_entry_t* mmap,__attribute__((unused)) uint16_t mmap_entries_count,__attribute__((unused)) char* commandline,__attribute__((unused)) FramebufferResolution* preferred_resolution)
    {

    }
    virtual bool detect(__attribute__((unused)) uint32_t* buf)
    {
        return false;
    }
protected:
    void* boot_structure;
};