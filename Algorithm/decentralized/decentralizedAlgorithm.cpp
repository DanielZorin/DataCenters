#include "decentralizedAlgorithm.h"
#include "virtualMachinesAssigner.h"
#include "storagesAssigner.h"
#include "virtualLinksAssigner.h"
#include "assignment.h"

DecentralizedAlgorithm::~DecentralizedAlgorithm()
{
    Assignments::iterator it = assignments.begin();
    Assignments::iterator itEnd = assignments.end();
    for ( ; it != itEnd; ++it )
        delete (*it);
    assignments.clear();

    Replications::iterator rit = replications.begin();
    Replications::iterator ritEnd = replications.end();
    for ( ; rit != ritEnd; ++rit )
        delete (*rit);
    replications.clear();
}

Algorithm::Result DecentralizedAlgorithm::schedule()
{
    VirtualMachinesAssigner virtualMachinesAssigner(network);
    Requests assignedRequests = virtualMachinesAssigner.PerformAssignment(requests);

    StoragesAssigner storagesAssigner(network);
    assignedRequests = storagesAssigner.PerformAssignment(assignedRequests);

    VirtualLinksAssigner virtualLinksAssigner(network, virtualMachinesAssigner.GetRequestAssignment(), 
        storagesAssigner.GetRequestAssignment());
    assignedRequests = virtualLinksAssigner.PerformAssignment(assignedRequests);

    replications = virtualLinksAssigner.getReplications();

    // remove all assigned requests on previous steps
    restoreNetwork(requests, assignedRequests, virtualLinksAssigner);

    Requests::iterator it = assignedRequests.begin();
    Requests::iterator itEnd = assignedRequests.end();
    for ( ; it != itEnd; ++it )
    {
        Assignment * assignment = new Assignment;
        // add vms assignments
        Assignment * vmAssignment = virtualMachinesAssigner.GetRequestAssignment(*it);
        
        Request::VirtualMachines vms = (*it)->getVirtualMachines();
        Request::VirtualMachines::iterator vmIt = vms.begin();
        Request::VirtualMachines::iterator vmItEnd = vms.end();
        
        for ( ; vmIt != vmItEnd; ++vmIt )
            assignment->AddAssignment(*vmIt, vmAssignment->GetAssignment(*vmIt));

        // add storages assignments
        Assignment * stAssignment = storagesAssigner.GetRequestAssignment(*it);
        
        Request::Storages sts = (*it)->getStorages();
        Request::Storages::iterator stIt = sts.begin();
        Request::Storages::iterator stItEnd = sts.end();
        
        for ( ; stIt != stItEnd; ++stIt )
            assignment->AddAssignment(*stIt, stAssignment->GetAssignment(*stIt));

        // add vls assignments
        Assignment * vlAssignment = virtualLinksAssigner.GetRequestAssignment(*it);
        
        Request::VirtualLinks vls = (*it)->getVirtualLinks();
        Request::VirtualLinks::iterator vlIt = vls.begin();
        Request::VirtualLinks::iterator vlItEnd = vls.end();
        
        for ( ; vlIt != vlItEnd; ++vlIt )
            assignment->AddAssignment(*vlIt, vlAssignment->GetAssignment(*vlIt));

        assignments.insert(assignment);
    }

    printf("Number of assigned requests: %d\n", assignedRequests.size());
    return assignedRequests.size() == requests.size() ? SUCCESS : (assignedRequests.size() == 0 ? FAILURE : PARTIAL);
}

void DecentralizedAlgorithm::restoreNetwork(Requests& initialRequests, Requests& assignedRequests,
                                            VirtualLinksAssigner& virtualLinksAssigner)
{
    Requests::iterator it = initialRequests.begin();
    Requests::iterator itEnd = initialRequests.end();
    for ( ; it != itEnd; ++it )
    {
        if ( assignedRequests.find(*it) == assignedRequests.end() )
            virtualLinksAssigner.removeAssignment(*it);
    }
}