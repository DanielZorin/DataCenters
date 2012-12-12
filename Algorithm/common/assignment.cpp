#include "assignment.h"

#include "request.h"
#include <stdio.h>
#include <assert.h>

Assignment::~Assignment()
{
    Replications::iterator rit = replications.begin();
    Replications::iterator ritEnd = replications.end();
    for ( ; rit != ritEnd; ++rit )
        delete (*rit);
    replications.clear();
}

string Assignment::getName()
{
    return request->getName();
}

Node * Assignment::GetAssignment(Node * virtualMachine)
{
    if ( nodeAssignments.count(virtualMachine) )
        return nodeAssignments[virtualMachine];
    else
        return 0;
}

Nodes Assignment::GetAssigned(Node * physicalMachine)
{
    Nodes virtualMachines;
    for ( NodeAssignments::iterator i = nodeAssignments.begin(); i != nodeAssignments.end(); i++)
        if ( (*i).second == physicalMachine )
            virtualMachines.insert((*i).first);

    return virtualMachines;
}

Store * Assignment::GetAssignment(Store * virtualStorage)
{
    if ( storeAssignments.count(virtualStorage) )
        return storeAssignments[virtualStorage];
    else
        return 0;
}

Stores Assignment::GetAssigned(Store * physicalStore)
{
    Stores virtualStores;
    for ( StoreAssignments::iterator i = storeAssignments.begin(); i != storeAssignments.end(); i++)
        if ( (*i).second == physicalStore )
            virtualStores.insert((*i).first);

    return virtualStores;
}

NetPath Assignment::GetAssignment(Link * netLink)
{
    if ( linkAssignments.count(netLink) )
        return linkAssignments[netLink];
    else
        return NetPath();
}

Links Assignment::GetAssigned(NetworkingElement * networkingElement)
{
    Links virtualLinks;
    for ( LinkAssignments::iterator i = linkAssignments.begin(); i != linkAssignments.end(); i++)
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

bool Assignment::isReplicaOnStore(Storage * storage, Store * store)
{
   if ( GetAssignment(storage) == store )
      return false;

   // Searching the replication to be sure
   Replications::iterator it = replications.begin();
   for ( ; it != replications.end(); ++it )
      if ( (*it)->getStorage() == storage )
      {
         assert((*it)->getSecondStore() == store);
         return true;
      }
//   assert(false); // replication should be found
   return false; // unfeasible
}

bool Assignment::checkReplicaOnStore(Storage * storage, Store * store)
{
   if ( GetAssignment(storage) == store )
      return false;

   // Searching the replication to be sure
   Replications::iterator it = replications.begin();
   for ( ; it != replications.end(); ++it )
      if ( (*it)->getStorage() == storage )
      {
         assert((*it)->getSecondStore() == store);
         return true;
      }
   return false;
}
