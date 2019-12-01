#include "init.h"

extern "C" void kmain()
{
    kmalloc_init(E820_MemoryMapLocationDescriptor);
    kmalloc_aligned_init();
    dma_alloc_init(E820_MemoryMapLocationDescriptor);
    cache_alloc_init(E820_MemoryMapLocationDescriptor);
    stdout::init();
    ProgrammableInterruptController::initialize();
    init();
}

void init()
{
    
    PCI::Access& access = PCI::get_access();
    List<PCI::Device>& pci_devices = PCI::enum_devices(access);
    List<PCI::Device>& pci_storage_devices = PCI::find_storage_devices(pci_devices);
    List<GenericDiskController>& controllers = Disk::enum_storage_controllers(pci_storage_devices,access);
    List<StorageDevice>& storage_devices = Disk::enum_storage_devices(controllers);
    List<GenericStoragePartition>& partitions = Partition::enum_storage_device(*storage_devices.get_node(0));

}
void inline halt()
{
    asm volatile ("cli;"
    "hlt");
}

