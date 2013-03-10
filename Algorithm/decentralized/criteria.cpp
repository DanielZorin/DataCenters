#include "criteria.h"
#include "node.h"
#include "store.h"
#include "link.h"
#include "replication.h"
#include "virtualLinkRouter.h"
#include "network.h"

Criteria::PackMode Criteria::packMode = Criteria::BFD; // default value

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
    return 2;
}

unsigned Criteria::kShortestPathDepth()
{
    return 5;
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

long Criteria::replicationPathCost(Store* initialStore, Store* store, Network * network, NetPath& path,
                                   unsigned replicationCapacity)
{
    long result;
    Link link("dummy_virtual_link", replicationCapacity);
    link.bindElements(initialStore, store);
    path = VirtualLinkRouter::routeKShortestPaths(&link, network);

    result = pathCost(path);
    return result;
}

long Criteria::replicationPathCost(VirtualLink* virtualLink, Network * network, NetPath& path)
{
    long result;
    path = VirtualLinkRouter::routeKShortestPaths(virtualLink, network);

    result = pathCost(path);
    return result;
}

#define NETWORK_CRITICAL_BORDER 0.3
void Criteria::identifyPackMode(Requests* requests, Network* network)
{
    // setting to NETWORK_CRITICAL if network is critical,
    // i.e. if ratio between requests virtual link capacities and
    // network link capacities for link coming from nodes or stores
    // is more then some border value (0.3 in our chosen).
    long networkCapacity = 0l;
    long requestsCapacity = 0l;
    Links::const_iterator it = network->getLinks().begin();
    Links::const_iterator itEnd = network->getLinks().end();
    for ( ; it != itEnd; ++it )
    {
        if ( (*it)->getFirst()->isComputational() || (*it)->getSecond()->isComputational() )
            networkCapacity += (*it)->getCapacity();
    }

    Requests::const_iterator reqIt = requests->begin();
    Requests::const_iterator reqItEnd = requests->end();
    for ( ; reqIt != reqItEnd; ++reqIt )
    {
        it = (*reqIt)->getVirtualLinks().begin();
        itEnd = (*reqIt)->getVirtualLinks().end();
        for ( ; it != itEnd; ++it )
            requestsCapacity += (*it)->getCapacity();
    }

    if ( networkCapacity == 0l || ((double)requestsCapacity) / networkCapacity > NETWORK_CRITICAL_BORDER )
        packMode = Criteria::NETWORK_CRITICAL;
    // otherwise default value is used
}