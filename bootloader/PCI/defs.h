#pragma once
#include <stdint.h>

#define PCIeDeviceConfigSpaceSize 4096
#define PCIDeviceConfigSpaceSize 256
#define DevicesPerBus 32
#define FunctionsPerDevice 8

#define PCIMemoryAccess 2
#define PCIIOAccess 1

typedef uint16_t PCIVendor;
typedef uint16_t PCIDeviceID;

typedef uint8_t PCIHeaderType;

typedef uint8_t PCI_PROGIF;
typedef uint8_t PCI_CLASSCODE;
typedef uint8_t PCI_SUBCLASS;
typedef uint8_t PCI_REVISION_ID;

typedef uint16_t PCICommandReg;
typedef uint16_t PCIStatusReg;

typedef uint32_t PCI_BAR_REGISTER;

struct PCIBasicHeader {
    PCIVendor vendor_id;
    PCIDeviceID device_id;
    
    PCICommandReg command;
    PCIStatusReg status;

    PCI_REVISION_ID revision;
    PCI_PROGIF progif;
    PCI_SUBCLASS subclass;
    PCI_CLASSCODE class_code;

    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
};
typedef struct PCIBasicHeader PCIBasicHeader;

struct PCIHeaderType0 {
    PCIBasicHeader h;

    PCI_BAR_REGISTER bar0;
    PCI_BAR_REGISTER bar1;
    PCI_BAR_REGISTER bar2;
    PCI_BAR_REGISTER bar3;
    PCI_BAR_REGISTER bar4;
    PCI_BAR_REGISTER bar5;
    uint32_t cardbus_cis_ptr;

    uint16_t subsystem_vendor_id;
    uint16_t subsystem_id;

    uint32_t expansion_rom_base_address;

    uint8_t capabilites_register;
    uint8_t reserved[3];

    uint32_t reserved2;

    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t min_grant;
    uint8_t max_latency;

} __attribute__((__packed__));

struct PCIHeaderType1 {
    PCIBasicHeader h;

    uint32_t bar0;
    uint32_t bar1;

    uint8_t primary_bus_number;
    uint8_t secondary_bus_number;
    uint8_t subordinate_bus_number;
    uint8_t secondary_latency_timer;

    uint8_t io_base;
    uint8_t io_limit;
    uint16_t secondary_status;

    uint16_t memory_base;
    uint16_t memory_limit;

    uint16_t prefetchable_memory_base;
    uint16_t prefetchable_memory_limit;

    uint32_t prefetchable_base_upper;
    uint32_t prefetchable_limit_upper;

    uint16_t io_base_upper;
    uint16_t io_limit_upper;

    uint32_t reserved;

    uint32_t expansion_rom_base_address;

    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint16_t bridge_control;

} __attribute__((__packed__));

struct PCIHeaderType2 {
    PCIBasicHeader h;

    uint32_t cardbus_socket_base_address;

    uint8_t capabilites_list_offset;
    uint8_t reserved;
    uint16_t secondary_status;

    uint8_t pci_bus_number;
    uint8_t cardbus_bus_number;
    uint8_t subordinate_bus_number;
    uint8_t cardbus_latency_timer;

    uint32_t memory_base_addr0;
    uint32_t memory_limit0;
    uint32_t memory_base_addr1;
    uint32_t memory_limit1;

    uint32_t io_base_addr0;
    uint32_t io_limit0;
    uint32_t io_base_addr1;
    uint32_t io_limit1;

    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint16_t bridge_control;

    uint16_t subsystem_device_id;
    uint16_t subsystem_vendor_id;

    uint32_t pc_card_16bit_mode_base_addr;

} __attribute__((__packed__));

typedef struct PCIDeviceDescriptor {
    uint32_t uniq_id;
    uint16_t flags;

    uint32_t segment;
    uint8_t bus;
    uint8_t device_number;
    uint8_t function_number;

    PCIVendor vendor_id;
    PCIDeviceID device_id;

    PCI_CLASSCODE class_code;
    PCI_SUBCLASS subclass;
    PCI_PROGIF progif;
    PCI_REVISION_ID revision;

} PCIDeviceDescriptor;