#include "storeManager.h"
#include <store.h>

StoreManager::StoreManager(Stores & s)
:
    stores(s)
{}

Stores StoreManager::getStoreAssignmentCandidates(Store * s)
{
    Stores result;
    unsigned long requestedCapacity = s->getCapacity();
    unsigned requestedTypeOfStore = s->getTypeOfStore();

    for (Stores::iterator store = stores.begin(); store != stores.end(); store++ )
    {
        Store * candidate = *store;
        if ( candidate->getCapacity() >= requestedCapacity 
                && candidate->getTypeOfStore() == requestedTypeOfStore )
            result.insert(candidate);
    } 

    return result;
}

