#pragma once
#include <stdint.h>
#include "../LibC/stdstring.h"

typedef struct RSDPDescriptor {
 char Signature[8];
 uint8_t Checksum;
 char OEMID[6];
 uint8_t Revision;
 uint32_t RsdtAddress;
} __attribute__ ((packed)) RSDPDescriptor;

typedef struct RSDPDescriptor20 {
 RSDPDescriptor firstPart; 
 uint32_t Length;
 uint64_t XsdtAddress;
 uint8_t ExtendedChecksum;
 uint8_t reserved[3];
} __attribute__ ((packed)) RSDPDescriptor20;

class RSDP {

public:
    ~RSDP();
    
    bool initialize();
    RSDPDescriptor20* get_rsdt();
private:
    uint32_t find();
    uint32_t search_rsdt(uint32_t addr,uint16_t count);
    
    uint8_t cached_rsdp[sizeof(RSDPDescriptor20)];
};