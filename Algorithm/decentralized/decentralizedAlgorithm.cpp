#include "decentralizedAlgorithm.h"
#include "virtualMachinesAssigner.h"
#include "storagesAssigner.h"

DecentralizedAlgorithm::~DecentralizedAlgorithm()
{
}

Algorithm::Result DecentralizedAlgorithm::schedule()
{
    VirtualMachinesAssigner virtualMachinesAssigner(network);
    Requests assignedRequests = virtualMachinesAssigner.PerformAssignment(requests);

    StoragesAssigner storagesAssigner(network);
    assignedRequests = storagesAssigner.PerformAssignment(assignedRequests);

    printf("Number of assigned requests: %d\n", assignedRequests.size());
    return assignedRequests.size() == requests.size() ? SUCCESS : (assignedRequests.size() == 0 ? FAILURE : PARTIAL);
}