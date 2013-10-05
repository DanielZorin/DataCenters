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

unsigned Criteria::exhaustiveSearchDepthNetwork()
{
    return 2;
}

unsigned Criteria::exhaustiveSearchDepthComputational()
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

#define NETWORK_CRITICAL_BORDER 0.1
void Criteria::identifyPackMode(Requests* requests, Network* network)
{
    // setting to NETWORK_CRITICAL if network is critical,
    // i.e. if ratio between requests virtual link capacities and
    // network link capacities for link coming from nodes or stores
    // is more then some border value (0.1 in our chosen).
    long networkCapacity = 0l;
    long nodesCapacity = 0l;
    long storesCapacity = 0l;
    long reqNetworkCapacity = 0l;
    long reqVmsCapacity = 0l;
    long reqStCapacity = 0l;
    Links::const_iterator it = network->getLinks().begin();
    Links::const_iterator itEnd = network->getLinks().end();
    for ( ; it != itEnd; ++it )
    {
        if ( (*it)->getFirst()->isComputational() || (*it)->getSecond()->isComputational() )
        {
            networkCapacity += (*it)->getCapacity();
            Element* elem = (*it)->getFirst()->isComputational() ? (*it)->getFirst() : (*it)->getSecond();
            if ( elem->isNode() )
                nodesCapacity += elem->getCapacity();
            else
                storesCapacity += elem->getCapacity();        
        }
    }

    Requests::const_iterator reqIt = requests->begin();
    Requests::const_iterator reqItEnd = requests->end();
    for ( ; reqIt != reqItEnd; ++reqIt )
    {
        it = (*reqIt)->getVirtualLinks().begin();
        itEnd = (*reqIt)->getVirtualLinks().end();
        for ( ; it != itEnd; ++it )
        {
            reqNetworkCapacity += (*it)->getCapacity();
            Element* elem = (*it)->getFirst()->isComputational() ? (*it)->getFirst() : (*it)->getSecond();
            if ( elem->isNode() )
                reqVmsCapacity += elem->getCapacity();
            else
                reqStCapacity += elem->getCapacity();        
        }
    }

    double netLoad = networkCapacity != 0l ? ((double)reqNetworkCapacity) / networkCapacity: 0.0;
    double vmsLoad = nodesCapacity != 0l ? ((double)reqVmsCapacity) / nodesCapacity : 0.0;
    double stLoad = storesCapacity != 0l ? ((double)reqStCapacity) / storesCapacity : 0.0;

    if ( netLoad > NETWORK_CRITICAL_BORDER || netLoad > vmsLoad && netLoad > stLoad )
        packMode = Criteria::NETWORK_CRITICAL;
    // otherwise default value is used
}
