#include <PCI/IOAccess.h>
void PCI::IOAccess::initialize()
{
    this->access_type = PCIIOAccess;
}
void PCI::IOAccess::write(uint8_t bus,uint8_t device,uint8_t func,uint8_t offset,uint16_t value)
{
    uint32_t lbus  = (uint32_t)bus;
    uint32_t ldevice = (uint32_t)device;
    uint32_t lfunc = (uint32_t)func;
    uint8_t loffset = (uint8_t)(offset & 0xff);
    uint32_t address;
    /* create configuration address */
    address = (uint32_t)((lbus << 16) | (ldevice << 11) |
        (lfunc << 8) | (loffset & 0xfc) | ((uint32_t)0x80000000));   
    IO::outl(0xCF8, address);
    /* write in the data */
    IO::outl(0xCFC,(uint32_t)(value & 0xffff));
}
uint16_t PCI::IOAccess::read(uint8_t bus,uint8_t device,uint8_t func,uint8_t offset)
{
    // Read using IO ports, legacy method
    uint16_t tmp;
    /* write out the address */
    uint32_t lbus  = (uint32_t)bus;
    uint32_t ldevice = (uint32_t)device;
    uint32_t lfunc = (uint32_t)func;
    uint8_t loffset = (uint8_t)(offset & 0xff);
    uint32_t address;
    /* create configuration address */
    address = (uint32_t)((lbus << 16) | (ldevice << 11) |
        (lfunc << 8) | (loffset & 0xfc) | ((uint32_t)0x80000000));   

    IO::outl(0xCF8, address);
    /* read in the data */
    tmp = (uint16_t)((IO::inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}