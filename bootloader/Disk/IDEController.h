#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>

#include <Disk/defs.h>
#include <Disk/GenericDiskController.h>

#include <PCI/Device.h>
#include <PCI/PCI.h>
#include <IO/IO.h>

#include <Memory/malloc.h>

typedef struct {
    uint32_t data_buffer;
    uint16_t byte_count;
    uint16_t reserved;
} __attribute__((__packed__)) ATA_DMA_PRDT;

class IDEController : public GenericDiskController {

public:
    IDEController(PCI::Device* device,PCI::Access* access);
    ~IDEController();
    bool probe_port_connected(bool is_primary,bool is_slave);
    void read(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount);
    uint16_t get_logical_sector_size(bool is_primary,bool is_slave);
    uint16_t get_physical_sector_size(bool is_primary,bool is_slave);
    uint8_t get_physical_logical_sector_alignment(bool is_primary,bool is_slave);
private:
    void initialize(PCI::Device* device,PCI::Access* access);
    void enable_pci_bus_master();
    void disable_pci_bus_master();

    void small_read(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t bytesOffset,uint16_t* buf,uint16_t bytesCount);
    void transfer_data(uint16_t offset,uint16_t bytesCount,uint8_t* buf);

    void read_pio_lba28(bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t bytesCount);
    void read_pio_lba48(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t bytesCount);
    void read_dma_lba28(bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t bytesCount);
    void read_dma_lba48(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t bytesCount);

    uint16_t get_sector_count(bool is_primary,bool is_slave,uint16_t bytesCount);
    bool identify(bool is_primary,bool is_slave,uint16_t* buf);
    void do_400ns_delay();
    uint16_t primary_bus_io_port;
    uint16_t secondary_bus_io_port;
    ATA_IDENTIFY_DATA cached_identify_data;
    uint8_t bus_select;
    PCI::Access* access;
    PCI::Device* device;

    uint32_t bus_master_register;

    uint8_t* tmpbuffer;
    uint16_t tmpbuffer_size;

    ATA_DMA_PRDT* prdt;
};