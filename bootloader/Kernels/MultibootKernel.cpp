#include <Kernels/MultibootKernel.h>

void MultibootProtocol::jump(multiboot_info_t* ptr,uint32_t entry_point)
{
    asm volatile(
    "cli;"
    "movl %0,%%ebx;"
    "movl %1,%%ecx;" 
    "movl $0x2BADB002,%%eax;"
    "pushl %%ecx;"
    "ret"
    :
    : "d" ((uint32_t)ptr), "a" ((uint32_t)entry_point)
    :
    );
}

void MultibootProtocol::load_vbe(multiboot_info_t* ptr)
{
    // hardcode things for now
    ptr->vbe_mode = 0x118;
    ptr->framebuffer_addr = 0xfd000000;
    ptr->framebuffer_height = 768;
    ptr->framebuffer_width = 1024;
    ptr->framebuffer_type = 0x0;
    ptr->framebuffer_bpp = 24;
    ptr->framebuffer_pitch = 768*4;
}

void MultibootProtocol::loadelf(multiboot_info_t* ptr,Elf32MainHeader* main_header)
{
    ptr->u.elf_sec.addr = (uint32_t)main_header + main_header->section_header_table_position;
    ptr->u.elf_sec.num = main_header->section_header_table_entries_count;
    ptr->u.elf_sec.size = main_header->section_header_table_entry_size;
    ptr->u.elf_sec.shndx = main_header->section_header_table_names_index;
}

void MultibootProtocol::load(multiboot_info_t* ptr,char* commandline,e820map_entry_t* mmap,uint32_t mmap_size, char* bootloader_name)
{
    ptr->cmdline = (uint32_t)commandline;
    ptr->flags = ptr->flags | MULTIBOOT_INFO_CMDLINE | (1<<5) | (1<<6) | (1<<11) | (1<<12);

    ptr->boot_loader_name = (uint32_t)bootloader_name;

    multiboot_memory_map_t* multiboot_mmap = (multiboot_memory_map_t*)0x205000;//kcalloc(sizeof(multiboot_memory_map_t)*mmap_size);
    
    for(uint32_t i=0; i< mmap_size;++i)
    {
        multiboot_mmap[i].size = 20;
        multiboot_mmap[i].addr1 = mmap[i].base_addr1;
        multiboot_mmap[i].len1 = mmap[i].length1;
        switch (mmap[i].type)
        {
        case 1:
            multiboot_mmap[i].type = MULTIBOOT_MEMORY_AVAILABLE;
            break;
        case 2:
            multiboot_mmap[i].type = MULTIBOOT_MEMORY_RESERVED;
            break;
        case 3:
            multiboot_mmap[i].type = MULTIBOOT_MEMORY_ACPI_RECLAIMABLE;
            break;
        case 4:
            multiboot_mmap[i].type = MULTIBOOT_MEMORY_NVS;
            break;
        case 5:
            multiboot_mmap[i].type = MULTIBOOT_MEMORY_BADRAM;
            break;
        default:
            break;
        }

    }
    ptr->mmap_addr = (uint32_t)multiboot_mmap;
    ptr->mmap_length = sizeof(multiboot_memory_map_t)*(mmap_size);
}

bool MultibootProtocol::detect(uint32_t* buf)
{
    for(int i=0; i<2048;++i) // 8192 bytes / sizeof(uint32_t)
    {
        if(buf[i] == MULTIBOOT_HEADER_MAGIC)
            return true;
    }
    return false;
}