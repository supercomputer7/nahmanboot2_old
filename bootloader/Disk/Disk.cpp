#include <Disk/Disk.h>
#include <LibC/stdbool.h>
List<GenericDiskController>* Disk::enum_storage_controllers(List<PCI::Device>* devices, PCI::Access* access)
{
    List<GenericDiskController>* list = new List<GenericDiskController>(nullptr,0);
    if(devices->get_count() == 0)
        return list;
    for(uint32_t i = 0; i < devices->get_count(); i++)
    {
        GenericDiskController* controller;
        
        if(devices->get_node(i)->get_object()->get_subclass() == 0x1)
            controller = new IDEController(devices->get_node(i)->get_object(),access);
        else if (devices->get_node(i)->get_object()->get_subclass() == 0x6)
            controller =  new AHCIController(devices->get_node(i)->get_object(),access);            
        else
        {
            controller = new GenericDiskController();
        }
        list->insert_node(controller);
    }
    return list;
}

List<StorageDevice>* Disk::enum_storage_controller(Node<GenericDiskController>* storage_controller)
{
    List<StorageDevice>* tmp = new List<StorageDevice>(nullptr,0);
    if(storage_controller->get_object()->get_controller_type() == IDEStorageController )
    {
        IDEController* controller = (IDEController*)storage_controller->get_object();
        for(int i=0 ;i<4;i++)
            if(controller->probe_port_connected(i))
                tmp->insert_node(new ATADevice(controller,(uint8_t)i));
    }
    else if (storage_controller->get_object()->get_controller_type() == AHCIStorageController)
    {
        AHCIController* controller = (AHCIController*)storage_controller->get_object();
        for(uint8_t i=0 ;i<AHCI_MAXIMUM_PORTS;++i)
            if(controller->probe_port_connected(i))
                tmp->insert_node(new SATADevice(controller,i));
    }
    else if (storage_controller->get_object()->get_controller_type() ==  NVMeStorageController)
    {
        /* TODO: Add NVMe Support */
    }
    return tmp;
}

List<StorageDevice>* Disk::enum_storage_devices(List<GenericDiskController>* controllers)
{
    List<StorageDevice>* tmp = new List<StorageDevice>(nullptr,0);
    if(controllers->get_count() == 0)
        return tmp;
    for(uint32_t i = 0; i < controllers->get_count(); i++)
        tmp->adpot_list(enum_storage_controller(controllers->get_node(i)));
    return tmp;
}