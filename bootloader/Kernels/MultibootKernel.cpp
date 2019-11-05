#include <Kernels/MultibootKernel.h>
#include <Parsers/ELF/ELF32Parser.h>
#include <Display/Monitors/Print.h>
void Multiboot::BootProtocol::boot(void* loaded_file,e820map_entry_t* mmap,uint16_t mmap_entries_count,char* commandline,FramebufferResolution* preferred_resolution)
{
    if(this->detect((uint32_t*)loaded_file))
    {
        ELF32Parser* parser = new ELF32Parser();
        parser->parse((Elf32MainHeader*)loaded_file,(uint32_t)kmalloc(reinterpret_cast<Elf32MainHeader *>(loaded_file)->section_header_table_entries_count*
                                                                    reinterpret_cast<Elf32MainHeader *>(loaded_file)->section_header_table_entry_size)
                                                            );
        this->boot_structure = (Multiboot::multiboot_info_t*)kmalloc_aligned();
        this->load(commandline,mmap,mmap_entries_count,nullptr);
        if(preferred_resolution != nullptr)
        {
            stdout::set_resolution(preferred_resolution->get_width(),
                                preferred_resolution->get_height(),
                                preferred_resolution->get_bpp());
            this->load_vbe(stdout::get_screen());
        }
        this->jump(reinterpret_cast<Elf32MainHeader *>(loaded_file)->program_entry_position);
    }
    else
    {
        stdout::print("Error: Not a Multiboot complaint image!\n");
    }
}
void Multiboot::BootProtocol::jump(uint32_t entry_point)
{
    asm volatile(
    "cli;"
    "movl %0,%%ebx;"
    "movl %1,%%ecx;" 
    "movl $0x2BADB002,%%eax;"
    "pushl %%ecx;"
    "ret"
    :
    : "d" ((uint32_t)this->boot_structure), "a" ((uint32_t)entry_point)
    :
    );
}

void Multiboot::BootProtocol::load_vbe(GenericScreen* screen)
{    
    this->get_multiboot_info()->framebuffer_addr = (uint32_t)screen->get_resolution_buffer();
    this->get_multiboot_info()->framebuffer_height = screen->get_resolution_height();
    this->get_multiboot_info()->framebuffer_width = screen->get_resolution_width();
    this->get_multiboot_info()->framebuffer_type = MULTIBOOT_FRAMEBUFFER_TYPE_RGB;
    this->get_multiboot_info()->framebuffer_bpp = screen->get_resolution_bpp();
    this->get_multiboot_info()->framebuffer_pitch = screen->get_resolution_pitch();
    this->get_multiboot_info()->vbe_mode = screen->get_current_videomode();
}

void Multiboot::BootProtocol::loadelf(Elf32MainHeader* main_header)
{
    this->get_multiboot_info()->u.elf_sec.addr = (uint32_t)main_header + main_header->section_header_table_position;
    this->get_multiboot_info()->u.elf_sec.num = main_header->section_header_table_entries_count;
    this->get_multiboot_info()->u.elf_sec.size = main_header->section_header_table_entry_size;
    this->get_multiboot_info()->u.elf_sec.shndx = main_header->section_header_table_names_index;
}

Multiboot::multiboot_info_t* Multiboot::BootProtocol::get_multiboot_info()
{
    return reinterpret_cast<Multiboot::multiboot_info_t*>(this->boot_structure);
}

void Multiboot::BootProtocol::load(char* commandline,e820map_entry_t* mmap,uint32_t mmap_size, char* bootloader_name)
{
    this->get_multiboot_info()->cmdline = (uint32_t)commandline;
    this->get_multiboot_info()->flags = this->get_multiboot_info()->flags | MULTIBOOT_INFO_CMDLINE | (1<<5) | (1<<6) | (1<<11) | (1<<12);

    this->get_multiboot_info()->boot_loader_name = (uint32_t)bootloader_name;

    multiboot_memory_map_t* multiboot_mmap = (multiboot_memory_map_t*)kcalloc(sizeof(multiboot_memory_map_t)*mmap_size);
    
    for(uint32_t i=0; i< mmap_size;++i)
    {
        multiboot_mmap[i].size = MULTIBOOT_MEMORY_DEFAULT_ENTRY_SIZE;
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
    this->get_multiboot_info()->mmap_addr = (uint32_t)multiboot_mmap;
    this->get_multiboot_info()->mmap_length = sizeof(multiboot_memory_map_t)*(mmap_size);
}

bool Multiboot::BootProtocol::detect(uint32_t* buf)
{
    for(int i=0; i<2048;++i) // 8192 bytes / sizeof(uint32_t)
    {
        if(buf[i] == MULTIBOOT_HEADER_MAGIC)
            return true;
    }
    return false;
}
