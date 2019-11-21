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
    GenericDiskController()
    {

    }
    virtual uint16_t get_controller_type()
    {
        return GenericStorageController;
    }
    virtual bool probe_port_connected(__attribute__((unused)) uint32_t port)
    {
        return false;
    }
    virtual bool read(__attribute__((unused)) uint8_t transfer_mode,__attribute__((unused)) uint8_t commandset,__attribute__((unused)) uint32_t port_number,__attribute__((unused)) uint32_t lbal,__attribute__((unused)) uint32_t lbah,__attribute__((unused)) uint16_t* buf,__attribute__((unused)) uint16_t sectors_count,__attribute__((unused)) uint16_t sector_size)
    {
        return false;
    }
    virtual uint16_t get_logical_sector_size(__attribute__((unused)) uint32_t port)
    {
        return 0x0;
    }
protected:
    uint8_t default_transfer_mode;
    PCI::Access* access;
    PCI::Device* device;
private:
    
};