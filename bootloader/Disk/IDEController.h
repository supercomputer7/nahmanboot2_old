#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>

#include <Disk/defs.h>
#include <Disk/GenericDiskController.h>

#include <PCI/Device.h>
#include <PCI/PCI.h>
#include <IO/IO.h>

class IDEController : public GenericDiskController {

public:
    ~IDEController();
    void initialize(PCI::Device* device,PCI::Access* access);
    bool probe_port_connected(bool is_primary,bool is_slave);
    void read(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t bytesCount);
    uint16_t get_sector_size(bool is_primary,bool is_slave);
private:
    void read_lba28(bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t bytesCount);
    void read_lba48(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t bytesCount);
    void read_lba28_atapi(bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t bytesCount);

    uint16_t get_sector_count(bool is_primary,bool is_slave,uint16_t bytesCount);
    bool identify(bool is_primary,bool is_slave,uint16_t* buf);
    void do_400ns_delay();
    uint16_t primary_bus_io_port;
    uint16_t secondary_bus_io_port;
    ATA_IDENTIFY_DATA cached_identify_data;
    uint8_t bus_select;
    PCI::Access* access;
    PCI::Device* device;
};