#include "decentralizedAlgorithm.h"
#include "computationalElement.h"
#include "networkingElement.h"
#include "request.h"

DecentralizedAlgorithm::ResultEnum::Result DecentralizedAlgorithm::schedule()
{
    Requests::iterator it = requests.begin();
    Requests::iterator itEnd = requests.end();

    // generate the set of nodes
    ComputationalElements nodes;
    for ( ; it != itEnd; ++it )
    {
        nodes.insert((RequestComputationalElements*)(&(*it)->getVirtualMachines()));
    }

    Requests assignedRequests = assignComputationalElements(nodes);

    // generate the set of stores
    it = assignedRequests.begin();
    itEnd = assignedRequests.end();

    ComputationalElements stores;
    for ( ; it != itEnd; ++it )
    {
        stores.insert((RequestComputationalElements*)&((*it)->getStorages()));
    }

    assignedRequests = assignComputationalElements(stores);

    // generate the set of virtual links
    it = assignedRequests.begin();
    itEnd = assignedRequests.end();

    NetworkingElements networkingElements;
    for ( ; it != itEnd; ++it )
    {
        networkingElements.insert((RequestNetworkingElements*)&((*it)->getVirtualLinks()));
    }

    ResultEnum::Result result = assignVirtualLinks(networkingElements);

    return result;
}

Algorithm::Requests DecentralizedAlgorithm::assignComputationalElements(ComputationalElements& computationalElements)
{
    return Requests();
}

Algorithm::ResultEnum::Result DecentralizedAlgorithm::assignVirtualLinks(NetworkingElements& networkingElements)
{
    return ResultEnum::SUCCESS;
}
