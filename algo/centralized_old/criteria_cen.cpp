#include "criteria_cen.h"

#include "common/node.h"
#include "common/store.h"
#include "common/link.h"
#include "common/request.h"

unsigned long CriteriaCen::weight(Element * element)
{
   return element->getCapacity();
}

unsigned long CriteriaCen::weight(Request * request)
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

unsigned long CriteriaCen::computationalCount(Request * request)
{
   return request->getVirtualMachines().size() + request->getStorages().size();
}


