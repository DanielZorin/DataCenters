#include "centralizedalgorithm.h"
#include "request.h"
#include "assignment.h"

Algorithm::Result CentralizedAlgorithm::schedule()
{
    std::vector<Request *> prioritizedRequests = prioritizeRequests();     

    for (std::vector<Request *>::iterator i = prioritizedRequests.begin(),
            e = prioritizedRequests.end();
            i != e ; i++ )
    {
        Request * request = *i;
        currentAssignment = new Assignment(request); 

        Result assignmentResult;
        assignmentResult = buildVMAssignment(request);
        if ( assignmentResult != SUCCESS )
        {
            delete currentAssignment;
            continue;
        }

        assignmentResult = buildStorageAssignment(request);
        if ( assignmentResult != SUCCESS )
        {
            delete currentAssignment;
            continue;
        }

        assignments.insert(currentAssignment);
        
    }

    if ( assignments.size() == requests.size() )
        return SUCCESS;
    else if ( assignments.size() != 0 )
        return PARTIAL;
    else
        return FAILURE;
}

std::vector<Request *> CentralizedAlgorithm::prioritizeRequests()
{
    return std::vector<Request *>();
}

std::vector<Node *> CentralizedAlgorithm::prioritizeVms(Request::VirtualMachines & vms)
{
    return std::vector<Node *>();
}

std::vector<Node *> CentralizedAlgorithm::getVMAssignmentCandidates(Node * wm)
{
    return std::vector<Node *>();
}

Algorithm::Result CentralizedAlgorithm::buildVMAssignment(Request * request)
{
    Request::VirtualMachines & vms = request->getVirtualMachines(); 
    std::vector<Node *> prioritizedVms = prioritizeVms(vms);

    for (std::vector<Node *>::iterator i = prioritizedVms.begin(), e = prioritizedVms.end();
            i != e; i++)
    {
        Node * w = *i;
        std::vector<Node *> assignmentCandidates = getVMAssignmentCandidates(w);
    }
    
    return SUCCESS;
}

Algorithm::Result CentralizedAlgorithm::buildStorageAssignment(Request * request)
{
    return SUCCESS;
}
