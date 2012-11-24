#include "criteria.h"
#include "node.h"
#include "store.h"
#include "link.h"
#include "replication.h"
#include "virtualLinkRouter.h"

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

long Criteria::requestVirtualLinksWeight(Request::VirtualLinks * virtualLinks)
{
    int result = 0;
    Request::VirtualLinks::iterator it = virtualLinks->begin();
    Request::VirtualLinks::iterator itEnd = virtualLinks->end();

    for ( ; it != itEnd; ++it )
    {
        result += (*it)->getCapacity();
    }

    return result;
}

long Criteria::virtualLinkWeight(Link* virtualLink)
{
    return virtualLink->getCapacity();
}

unsigned Criteria::exhaustiveSearchDepth()
{
    return 4;
}

unsigned Criteria::kShortestPathDepth()
{
    return 10;
}

long Criteria::pathCost(NetPath& path)
{
    long result = 0l;
    NetPath::iterator it = path.begin();
    NetPath::iterator itEnd = path.end();
    for ( ; it != itEnd; ++it )
        result += (*it)->getCapacity();
    return result;
}

long Criteria::replicationPathCost(Store* initialStore, Store* store, Network * network, NetPath& path)
{
    long result;
    Link link("dummy_virtual_link", Replication::GetLinkBandwidth(store->getTypeOfStore()));
    link.bindElements(initialStore, store);
    path = VirtualLinkRouter::routeDejkstra(&link, network);

    result = pathCost(path);
    return result;
}

long Criteria::replicationPathCost(VirtualLink* virtualLink, Network * network, NetPath& path)
{
    long result;
    path = VirtualLinkRouter::routeDejkstra(virtualLink, network);

    result = pathCost(path);
    return result;
}