#pragma once

#include <stdint.h>
#include <LibC/stdbool.h>
#include <PCI/Access.h>
#include <PCI/Device.h>

#define GenericStorageController 0x0
#define IDEStorageController 0x1
#define AHCIStorageController 0x2
#define NVMeStorageController 0x3

#define DefaultCommandSet 0x0
#define ATACommandSet 0x1
#define ATAPICommandSet 0x2

#define DefaultTransferMode 0x0
#define DMATransferMode 0x1
#define PIOTransferMode 0x2

class GenericDiskController {

public:
    ~GenericDiskController();
    GenericDiskController(PCI::Device& device,PCI::Access& access)
        : m_device (device),
          m_access (access)
    {
    }
    virtual uint16_t get_controller_type() = 0;
    virtual bool probe_port_connected(uint32_t port) = 0;
    virtual bool read( uint8_t transfer_mode, uint8_t commandset,__attribute__((unused)) uint32_t port_number, uint32_t lbal, uint32_t lbah, uint16_t* buf, uint16_t sectors_count, uint16_t sector_size) = 0;
    virtual uint16_t get_logical_sector_size(uint32_t port) = 0;
protected:
    uint8_t default_transfer_mode;
    PCI::Device& m_device;
    PCI::Access& m_access;
private:
    
};