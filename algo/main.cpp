#include <stdio.h>
#include <vector>
#include "interface/snapshot.h"
#include "test/testalgorithm.h"
#include "prototype/prototype.h"


#include "defs.h"
#include "request.h"
#include "network.h"
#include "operation.h"
#include "criteria.h"

#include <QString>

enum ReturnCodes {
   SUCCESS = 0,
   PARTIAL_FAILURE,
   INVALID_INPUT,
   INVALID_USAGE
};

int main(int argc, char ** argv)
{
    if ( argc != 3 )
    {
        printf("Usage: %s <input file> <output file>\n", *argv);
        return INVALID_USAGE;
    }

    Snapshot snapshot;
    if ( !snapshot.read(argv[1]) )
       return INVALID_INPUT;

    snapshot.print();

    Requests requests = snapshot.getRequests();
    PrototypeAlgorithm algorithm(snapshot.getNetwork(), requests);
    
    //
    algorithm.setResources(snapshot.getResources());
    algorithm.setTenants(snapshot.getTenants());
    //
    
    algorithm.schedule(); 

    int assignedRequests = 0;
    int nodeAssignedRequests = 0;
    for ( Requests::iterator i = requests.begin(); i != requests.end(); i++ ) {
        Request * r = *i;
        if ( r->isAssigned() )
            assignedRequests++;    
        Elements unassignedComputational = Operation::filter(r->elementsToAssign(), Criteria::isComputational);
        if ( unassignedComputational.empty() )
            nodeAssignedRequests++;
    }

    snapshot.write(argv[2]);
    snapshot.print();

    if ( nodeAssignedRequests != requests.size())
       return PARTIAL_FAILURE;

    return SUCCESS;

}
