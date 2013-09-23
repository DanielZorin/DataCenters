#include "criteria.h"

#include "node.h"
#include "store.h"
#include "link.h"
#include "request.h"

unsigned long Criteria::weight(Element * element)
{
   return element->getCapacity();
}

unsigned long Criteria::weight(Request * request)
{
   unsigned long result = 0;
   Nodes & vms = request->getVirtualMachines();
   Stores & stores = request->getStorages();
   Links & links = request->getVirtualLinks();

   for (Nodes::iterator i = vms.begin(); i != vms.end(); i++)
      result += (*i)->getCapacity();

   for (Stores::iterator i = stores.begin(); i != stores.end(); i++)
      result += (*i)->getCapacity();

   for (Links::iterator i = links.begin(); i != links.end(); i++)
      result += (*i)->getCapacity();

   return result;

}
