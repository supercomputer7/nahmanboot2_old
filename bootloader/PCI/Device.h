#pragma once
#include <PCI/PCI.h>
#include <stdint.h>
#include <PCI/defs.h>

class PCI::Device{

public:
    void initialize(PCI::Access* access,uint32_t seg,uint8_t bus,uint8_t device,uint8_t function);
    ~Device();

    uint32_t get_segment() const;
    uint8_t get_bus() const;
    uint8_t get_device_number() const;
    uint8_t get_function_number() const;

    uint16_t get_vendor_number() const;
    uint16_t get_device_id() const;

    uint8_t get_class_code() const;
    uint8_t get_subclass() const;
    uint8_t get_progif() const;
    uint8_t get_revision_id() const;
private:
    void set_segment(uint32_t seg);
    void set_bus(uint8_t bus);
    void set_device_number(uint8_t device);
    void set_function_number(uint8_t func);

    void set_vendor_number(uint16_t vendor);
    void set_device_id(uint16_t device);

    void set_class_code(uint8_t classcode);
    void set_subclass(uint8_t subclass);
    void set_progif(uint8_t progif);
    void set_revision_id(uint8_t revision);
    PCIDeviceDescriptor descriptor;
};