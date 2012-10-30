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

        Result result;
        result = buildVMAssignment();
        if ( result != SUCCESS )
        {
            delete currentAssignment;
            continue;
        }

        result = buildStorageAssignment();
        if ( result != SUCCESS )
        {
            delete currentAssignment;
            continue;
        }

        assignments.insert(currentAssignment);
        
    }

    return SUCCESS;
}

std::vector<Request *> CentralizedAlgorithm::prioritizeRequests()
{
    return std::vector<Request *>();
}

Algorithm::Result CentralizedAlgorithm::buildVMAssignment()
{
    return SUCCESS;
}

Algorithm::Result CentralizedAlgorithm::buildStorageAssignment()
{
    return SUCCESS;
}
