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
    bool probe_port_connected(uint8_t port) override;
    bool read(uint8_t commandset,uint8_t port_number,uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount) override;
    uint16_t get_controller_type() override;
    uint16_t get_logical_sector_size(uint8_t port) override;
    uint16_t get_physical_sector_size(uint8_t port) override;
    uint8_t get_physical_logical_sector_alignment(uint8_t port) override;
private:
    void initialize(PCI::Device* device,PCI::Access* access);

    uint16_t native_get_logical_sector_size(bool is_primary,bool is_slave);
    uint16_t native_get_physical_sector_size(bool is_primary,bool is_slave);
    uint8_t native_get_physical_logical_sector_alignment(bool is_primary,bool is_slave);
    void native_read(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount);
    bool native_probe_port_connected(bool is_primary,bool is_slave);

    void enable_pci_bus_master();
    void disable_pci_bus_master();

    void small_read(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t bytesOffset,uint16_t* buf,uint16_t bytesCount);
    void transfer_data(uint16_t offset,uint16_t bytesCount,uint8_t* buf);

    void read_pio_lba28(bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t bytesCount);
    void read_pio_lba48(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t bytesCount);
    void read_dma_lba28(bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t bytesCount);
    void read_dma_lba48(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t bytesCount);

    uint16_t get_sectors_per_bytes_count(bool is_primary,bool is_slave,uint16_t bytesCount);

    bool identify(bool is_primary,bool is_slave,uint16_t* buf);
    bool identify_atapi(bool is_primary,bool is_slave,uint16_t* buf);
    
    void do_400ns_delay();

    uint32_t bus_master_register;
    ATA_DMA_PRDT* prdt;
    uint16_t primary_bus_io_port;
    uint16_t secondary_bus_io_port;

    ATA_IDENTIFY_DATA cached_identify_data;
    uint8_t bus_select;
    PCI::Access* access;
    PCI::Device* device;

    uint8_t* tmpbuffer;
    uint16_t tmpbuffer_size;
};