#include "criteria.h"
#include "node.h"
#include "store.h"

long Criteria::requestVirtualMachinesWeight(Request::VirtualMachines* virtualMachines)
{
    int result = 0;
    Request::VirtualMachines::iterator it = virtualMachines->begin();
    Request::VirtualMachines::iterator itEnd = virtualMachines->end();

    for ( ; it != itEnd; ++it )
    {
        result += (*it)->getCapacity();
    }

    return result;
}

long Criteria::virtualMachineWeight(Node* virtualMachine)
{
    return virtualMachine->getCapacity();
}

long Criteria::requestStoragesWeight(Request::Storages* storages)
{
    int result = 0;
    Request::Storages::iterator it = storages->begin();
    Request::Storages::iterator itEnd = storages->end();

    for ( ; it != itEnd; ++it )
    {
        result += (*it)->getCapacity();
    }

    return result;
}

long Criteria::storageWeight(Store* storage)
{
    return storage->getCapacity();
}

unsigned Criteria::exhaustiveSearchDepth()
{
    return 4;
}