#include "replication.h"
#include "store.h"
#include "link.h"

std::map<unsigned, long> Replication::consistencyBandwidths;

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

