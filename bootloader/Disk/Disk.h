#pragma once
#include <Memory/malloc.h>
#include <Memory/Node.h>
#include <Memory/List.h>
#include <stdint.h>
#include <PCI/PCI.h>
#include <PCI/Device.h>
#include <Disk/StorageDevice.h>

namespace Disk {
    List<GenericDiskController>* enum_storage_controllers(List<PCI::Device>* devices, PCI::Access* access);
    List<StorageDevice>* enum_storage_controller(Node<GenericDiskController>* storage_controller);
    List<StorageDevice>* enum_storage_devices(List<GenericDiskController>* controllers);
}