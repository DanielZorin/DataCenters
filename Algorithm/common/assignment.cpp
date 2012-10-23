#include "assignment.h"

Node * Assignment::GetAssignment(Node * virtualMachine)
{
    if ( nodeAssignment.count(virtualMachine) )
        return nodeAssignment[virtualMachine];
    else
        return 0;
}

Assignment::Nodes Assignment::GetAssigned(Node * physicalMachine)
{
    Nodes virtualMachines;
    for ( NodeAssignment::iterator i = nodeAssignment.begin(); i != nodeAssignment.end(); i++)
        if ( (*i).second == physicalMachine )
            virtualMachines.insert((*i).first);

    return virtualMachines;
}

Store * Assignment::GetAssignment(Store * virtualStorage)
{
    if ( storeAssignment.count(virtualStorage) )
        return storeAssignment[virtualStorage];
    else
        return 0;
}

Assignment::Stores Assignment::GetAssigned(Store * physicalStore)
{
    Stores virtualStores;
    for ( StoreAssignment::iterator i = storeAssignment.begin(); i != storeAssignment.end(); i++)
        if ( (*i).second == physicalStore )
            virtualStores.insert((*i).first);

    return virtualStores;
}

Assignment::NetPath Assignment::GetAssignment(Link * netLink)
{
    if ( linkAssignment.count(netLink) )
        return linkAssignment[netLink];
    else
        return Assignment::NetPath();
}

Assignment::Links Assignment::GetAssigned(NetworkingElement * networkingElement)
{
    Links virtualLinks;
    for ( LinkAssignment::iterator i = linkAssignment.begin(); i != linkAssignment.end(); i++)
    {
        Link * link = (*i).first;
        NetPath & netPath = (*i).second;
        for (NetPath::iterator j = netPath.begin(); j != netPath.end(); j++)
        {
            if ( (*j) == networkingElement )
            {
                virtualLinks.insert(link);
                break;
            }
        }
    }
    return virtualLinks;
}

string Assignment::GetXMLNode()
{
    return string();
}

string Assignment::GetFormattedXMLNode()
{
    return string();
}
