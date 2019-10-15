#pragma once
#include <Memory/Node.h>
#include <PCI/PCI.h>
#include <PCI/Device.h>
#include <stdint.h>
#include <PCI/Access.h>
#include <PCI/IOAccess.h>
#include <PCI/MemoryAccess.h>
class PCI::List{

public:
    ~List();
    void initialize(Node* devices,uint32_t count);
    Node* get_devices();
    uint32_t get_count();
private:
    Node* devices;
    uint32_t count;
};

