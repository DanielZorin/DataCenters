#include "assignment.h"
#include "element.h"
#include "link.h"
#include "node.h"
#include "store.h"

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

using namespace std;

map<int, int> Assignment::printAssignment() 
    {
		//getchar();
		map<int, int> result;
		cout << "======================" << endl;
		//cout << " L0 " << endl;
		string nameRequest = request->getName(); 
		cout << "assignment of request " << nameRequest << endl;
		long numRequest = nameRequest[nameRequest.size() - 1] - '0';
		//cout << " L1 ";
		for (NodeAssignments::iterator i = nodeAssignments.begin(); i != nodeAssignments.end(); ++i) {
			//cout << " L2 ";
			NodeAssignment p = *i;
			//cout << " L3 ";
			long numNode = (p.second)->getID();
			result.insert(pair<int, int>(numNode, numRequest));
			cout << endl << "nodes to: " << numNode << " ";
			
			//cout << "          " << (p.first)->getCapacity() << " " << (p.second)->getCapacity();
		}
		//cout << " L4 ";
		cout << endl;
		for (StoreAssignments::iterator i = storeAssignments.begin(); i != storeAssignments.end(); ++i) {
			//cout << " L5 ";
			StoreAssignment p = *i;
			cout << endl << "stores to: " << (p.second)->getID() <<" ";
			//cout << "          " << (p.first)->getCapacity() << " " << (p.second)->getCapacity();
		}
		cout << endl;
		return result;
	}

string Assignment::getName()
{
    return request->getName();
}

Element * Assignment::GetAssignment(Element * virtualResource)
{
   if ( virtualResource->isNode() )
      return GetAssignment((Node *)(virtualResource));

   if ( virtualResource->isStore() )
      return GetAssignment((Store *)(virtualResource));

   return 0;
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

void Assignment::forcedCleanup()
{
   for(NodeAssignments::iterator i = nodeAssignments.begin(); i != nodeAssignments.end(); i++)
      (i->second)->RemoveAssignment(i->first);
   for(StoreAssignments::iterator i = storeAssignments.begin(); i != storeAssignments.end(); i++)
      (i->second)->RemoveAssignment(i->first);
   for(LinkAssignments::iterator i = linkAssignments.begin(); i != linkAssignments.end(); i++)
   {
      NetPath & path = i->second;
      Link * link = i->first;
      for(NetPath::iterator n = path.begin(); n != path.end(); n++)
      {
         NetworkingElement * ne = *n;
         ne->RemoveAssignment(link);
      }
   }

   nodeAssignments.clear();
   storeAssignments.clear();
   linkAssignments.clear();

}
