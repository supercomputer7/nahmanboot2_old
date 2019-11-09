#pragma once
#include <Memory/Node.h>
#include <Memory/List.h>
#include <PCI/Device.h>
#include <Disk/StorageDevice.h>
#include <Disk/GenericDiskController.h>

namespace Disk {
    List<GenericDiskController>* enum_storage_controllers(List<PCI::Device>* devices, PCI::Access* access);
    List<StorageDevice>* enum_storage_controller(Node<GenericDiskController>* storage_controller);
    List<StorageDevice>* enum_storage_devices(List<GenericDiskController>* controllers);
}