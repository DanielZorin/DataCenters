#include "replication.h"
#include "store.h"
#include "link.h"

std::map<unsigned, long> Replication::consistencyBandwidths = std::map<unsigned, long>();

long Replication::GetLinkBandwidth(unsigned typeOfStore)
{
   if ( consistencyBandwidths.find(typeOfStore) != consistencyBandwidths.end() )
      return consistencyBandwidths[typeOfStore];
   return 0l;
}

Replication::~Replication()
{
    second->RemoveAssignment(first);

    long weight = GetLinkBandwidth(first->getTypeOfStore());
    Link dummy("dummy", weight);
    for ( NetPath::iterator it = link.begin(); it != link.end(); ++it )
    {
        (*it)->RemoveAssignment(&dummy); // just increase capacity
    }
}


void Replication::SetLinkBandwidth(unsigned typeOfStore, long bandwidth)
{
   if ( typeOfStore != 0 ) // 0 - not database, but memory in RAM
      consistencyBandwidths[typeOfStore] = bandwidth;
}


