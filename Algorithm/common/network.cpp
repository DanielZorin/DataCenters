#include "network.h"
#include "node.h"
#include "store.h"
#include "switch.h"
#include "link.h"

Network::~Network()
{
   for (Nodes::iterator i = nodes.begin(); i != nodes.end(); i++)
      delete *i;      
   for (Switches::iterator i = switches.begin(); i != switches.end(); i++)
      delete *i;      
   for (Stores::iterator i = stores.begin(); i != stores.end(); i++)
      delete *i;      
   for (Links::iterator i = links.begin(); i != links.end(); i++)
      delete *i;      
}
