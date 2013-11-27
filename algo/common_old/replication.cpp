#include "replication.h"
#include "store.h"
#include "link.h"

void Replication::Remove()
{
    second->RemoveAssignment(first);

    long weight = storage->getReplicationCapacity();
    Link dummy("dummy", weight);
    for ( NetPath::iterator it = link.begin(); it != link.end(); ++it )
    {
        (*it)->RemoveAssignment(&dummy); // just increase capacity
    }
}
