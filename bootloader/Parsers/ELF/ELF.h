#pragma once

#include <stdint.h>
typedef struct {
    char magic[4];
    uint8_t bit_width;
    uint8_t endianess;
    uint8_t header_version;
    uint8_t os_abi;

    char padding[8];

    uint16_t file_type;
    uint16_t instruction_set;

    uint32_t elf_version;
    uint32_t program_entry_position;
    uint32_t program_header_table_position;
    uint32_t section_header_table_position;

    uint32_t flags;

    uint16_t header_size;
    uint16_t program_header_table_entry_size;
    uint16_t program_header_table_entries_count;

    uint16_t section_header_table_entry_size;
    uint16_t section_header_table_entries_count;

    uint16_t section_header_table_names_index;
} __attribute__((__packed__)) Elf32MainHeader;

typedef struct {
    char magic[4];
    uint8_t bit_width;
    uint8_t endianess;
    uint8_t header_version;
    uint8_t os_abi;

    char padding[8];

    uint16_t file_type;
    uint16_t instruction_set;

    uint32_t elf_version;
    uint64_t program_entry_position;
    uint64_t program_header_table_position;
    uint64_t section_header_table_position;

    uint32_t flags;

    uint16_t header_size;
    uint16_t program_header_table_entry_size;
    uint16_t program_header_table_entries_count;

    uint16_t section_header_table_entry_size;
    uint16_t section_header_table_entries_count;

    uint16_t section_header_table_names_index;
} __attribute__((__packed__)) Elf64MainHeader;

typedef struct {
    uint32_t segment_type;
    
    uint32_t p_offset;

    uint32_t p_vaddr;
    uint32_t undefined;

    uint32_t p_filesz;

    uint32_t p_memsz;
    uint32_t flags;

    uint32_t required_alignment;
} __attribute__((__packed__)) Elf32ProgramHeader;

typedef struct {
    uint32_t segment_type;
    
    uint32_t p_offset;

    uint32_t p_vaddr;
    uint32_t undefined;

    uint32_t p_filesz;

    uint32_t p_memsz;
    uint32_t flags;

    uint32_t required_alignment;
} __attribute__((__packed__)) Elf64ProgramHeader;

typedef struct {
    uint32_t sh_name;
    uint32_t sh_type;

    uint32_t sh_flags;
    uint32_t sh_addr;

    uint32_t sh_offset;
    uint32_t sh_size;

    uint32_t sh_link;
    uint32_t sh_info;

    uint32_t sh_addralign;
    uint32_t sh_entsize;
} __attribute__((__packed__)) Elf32_SectionHeader;