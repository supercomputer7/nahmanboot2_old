#include <PCI/List.h>

void PCI::List::initialize(PCI::Device* devices,uint32_t count)
{
    this->devices = devices;
    this->count = count;
}
PCI::Device* PCI::List::get_devices()
{
    return this->devices;
}
uint32_t PCI::List::get_count()
{
    return this->count;
}