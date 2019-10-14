#include <PCI/Device.h>
#include <PCI/Access.h>
#include <PCI/IOAccess.h>
#include <PCI/MemoryAccess.h>
#include <PCI/defs.h>


void PCI::Device::initialize(PCI::Access* access,uint32_t seg,uint8_t bus,uint8_t device,uint8_t function)
{
    if(access->access_type == PCIMemoryAccess)
    {
        PCI::MemoryAccess* pcie_access = (PCI::MemoryAccess*)access;
        if(pcie_access->read(seg,bus,device,function,0) != 0xffff && pcie_access->read(seg,bus,device,function,2) != 0xffff)
        {
            this->set_segment(seg);
            this->set_bus(bus);
            this->set_device_number(device);
            this->set_function_number(function);

            this->set_vendor_number(pcie_access->read(seg,bus,device,function,0));
            this->set_device_id(pcie_access->read(seg,bus,device,function,2));
            
            this->set_class_code((pcie_access->read(seg,bus,device,function,10) & 0xFF00) >> 8);
            this->set_subclass(pcie_access->read(seg,bus,device,function,10) & 0xFF);
            this->set_progif((pcie_access->read(seg,bus,device,function,8) & 0xFF00) >> 8);
            this->set_revision_id(pcie_access->read(seg,bus,device,function,8) & 0xFF);
        }
    }
    else if(access->access_type == PCIIOAccess)
    {
        PCI::IOAccess* pci_access = (PCI::IOAccess*)access;
        if(pci_access->read(bus,device,function,0) != 0xffff && pci_access->read(bus,device,function,2) != 0xffff)
        {
            this->set_segment(seg);
            this->set_bus(bus);
            this->set_device_number(device);
            this->set_function_number(function);

            this->set_vendor_number(pci_access->read(bus,device,function,0));
            this->set_device_id(pci_access->read(bus,device,function,2));
            
            this->set_class_code((pci_access->read(bus,device,function,10) & 0xFF00) >> 8);
            this->set_subclass(pci_access->read(bus,device,function,10) & 0xFF);
            this->set_progif((pci_access->read(bus,device,function,8) & 0xFF00) >> 8);
            this->set_revision_id(pci_access->read(bus,device,function,8) & 0xFF);
        }
    }
}

uint32_t PCI::Device::get_segment() const { return this->descriptor.segment; }
uint8_t PCI::Device::get_bus() const { return this->descriptor.bus; }
uint8_t PCI::Device::get_device_number() const { return this->descriptor.device_number; }
uint8_t PCI::Device::get_function_number() const { return this->descriptor.function_number; }

uint16_t PCI::Device::get_vendor_number() const { return this->descriptor.vendor_id; }
uint16_t PCI::Device::get_device_id() const { return this->descriptor.device_id; }

uint8_t PCI::Device::get_class_code() const { return this->descriptor.class_code; }
uint8_t PCI::Device::get_subclass() const { return this->descriptor.subclass; }
uint8_t PCI::Device::get_progif() const { return this->descriptor.progif; }
uint8_t PCI::Device::get_revision_id() const { return this->descriptor.revision; }

void PCI::Device::set_segment(uint32_t seg) { this->descriptor.segment = seg; }
void PCI::Device::set_bus(uint8_t bus) { this->descriptor.bus = bus; }
void PCI::Device::set_device_number(uint8_t device) { this->descriptor.device_number = device; }
void PCI::Device::set_function_number(uint8_t func) { this->descriptor.function_number = func; }

void PCI::Device::set_vendor_number(uint16_t vendor) { this->descriptor.vendor_id = vendor; }
void PCI::Device::set_device_id(uint16_t device) { this->descriptor.device_id = device; }

void PCI::Device::set_class_code(uint8_t classcode) { this->descriptor.class_code = classcode; }
void PCI::Device::set_subclass(uint8_t subclass) { this->descriptor.subclass = subclass; }
void PCI::Device::set_progif(uint8_t progif) { this->descriptor.progif = progif; }
void PCI::Device::set_revision_id(uint8_t revision) { this->descriptor.revision = revision; }