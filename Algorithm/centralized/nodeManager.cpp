#include "nodeManager.h"
#include "common/node.h"

NodeManager::NodeManager(Nodes & n)
:
    nodes(n)
{}

Nodes NodeManager::getVMAssignmentCandidates(Node * vm)
{
   Nodes result;
   unsigned long requestedCapacity = vm->getCapacity();

   for(Nodes::iterator i = nodes.begin(); i != nodes.end(); i++)
   {
      Node * candidate = *i;
      if ( candidate->getCapacity() >= requestedCapacity )
         result.insert(candidate);
   }

   return result;
}
