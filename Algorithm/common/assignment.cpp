#include "assignment.h"

Node * Assignment::GetAssignment(Node * virtualMachine)
{
    if ( nodeAssignment.count(virtualMachine) )
        return nodeAssignment[virtualMachine];
    else
        return 0;
}

std::vector<Node *> Assignment::GetAssigned(Node * physicalMachine)
{
    std::vector<Node *> virtualMachines;
    for ( NodeAssignment::iterator i = nodeAssignment.begin(); i != nodeAssignment.end(); i++)
        if ( (*i).second == physicalMachine )
            virtualMachines.push_back((*i).first);

    return virtualMachines;
}

Store * Assignment::GetAssignment(Store * virtualStorage)
{
    if ( storeAssignment.count(virtualStorage) )
        return storeAssignment[virtualStorage];
    else
        return 0;
}

std::vector<Store *> Assignment::GetAssigned(Store * physicalStore)
{
    std::vector<Store *> virtualStores;
    for ( StoreAssignment::iterator i = storeAssignment.begin(); i != storeAssignment.end(); i++)
        if ( (*i).second == physicalStore )
            virtualStores.push_back((*i).first);

    return virtualStores;
}
