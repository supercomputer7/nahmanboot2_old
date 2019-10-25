#pragma once
#include <stdint.h>
#include <LibC/stdstring.h>
#include <Parsers/ELF.h>

class ELF32Parser {

public:
    ~ELF32Parser();
    
    void initialize();
    void parse(Elf32MainHeader* main_header,uint32_t section_data);
    uint32_t get_entry_addr(Elf32MainHeader* main_header);
private:
    
    uint8_t cached_header[512];
};