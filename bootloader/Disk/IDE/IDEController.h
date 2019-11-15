#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>
#include <Disk/GenericDiskController.h>
#include <Disk/defs.h>
#include <PCI/Device.h>
#include <PCI/PCI.h>

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
    bool read(uint8_t transfer_mode,uint8_t commandset,uint8_t port_number,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size) override;
    uint16_t get_controller_type() override;
    uint16_t get_logical_sector_size(uint8_t port) override;
private:
    void initialize(PCI::Device* device,PCI::Access* access);
    void native_read(uint8_t transfer_mode,bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size);
    uint16_t native_get_logical_sector_size(bool is_primary,bool is_slave);
    void read_48bit(uint8_t transfer_mode,bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size);
    void read_28bit(uint8_t transfer_mode,bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size);

    void enable_pci_bus_master();
    void disable_pci_bus_master();   
    bool ata_identify(bool is_primary,bool is_slave,uint16_t* buf);
    void read_ata_pio_lba28(bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size);
    void read_ata_pio_lba48(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size);
    void read_ata_dma_lba28(bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size);
    void read_ata_dma_lba48(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size);
    void do_400ns_delay();

    uint32_t bus_master_register;
    ATA_DMA_PRDT* prdt;
    uint16_t primary_bus_io_port;
    uint16_t secondary_bus_io_port;
    ATA_IDENTIFY_DATA cached_identify_data;
    uint8_t bus_select;
};