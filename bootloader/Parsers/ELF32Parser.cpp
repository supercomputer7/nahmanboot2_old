#include <Parsers/ELF32Parser.h>
#include <LibC/stdstring.h>
void ELF32Parser::parse(Elf32MainHeader* main_header,uint32_t section_data)
{
    Elf32ProgramHeader* progs = (Elf32ProgramHeader*)((uint32_t)main_header + (main_header->program_header_table_position));
    for(int i=0; i < main_header->program_header_table_entries_count ; ++i)
    {
        // zeroing p_memsz bytes at p_vaddr
        memset((uint8_t*)(progs[i].p_vaddr),0,progs[i].p_memsz);
        // copy p_filesz bytes from p_offset to p_vaddr
        strcopy((char*)((uint32_t)main_header + (progs[i].p_offset)),
                (char*)(progs[i].p_vaddr),
                progs[i].p_filesz
                );
    }
    for(int i=0; i < main_header->section_header_table_entries_count ; ++i)
    {
        // copy p_filesz bytes from p_offset to p_vaddr
        strcopy((char*)((uint32_t)main_header + main_header->section_header_table_position),
                (char*)(section_data),
                (main_header->section_header_table_entries_count*main_header->section_header_table_entry_size)
                );
    }
}
uint32_t ELF32Parser::get_entry_addr(Elf32MainHeader* main_header)
{
    return main_header->program_entry_position;
}