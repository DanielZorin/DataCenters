#ifndef STOREMANAGER_H
#define STOREMANAGER_H

#include "publicdefs.h"

class StoreManager
{
public:
    StoreManager(Stores &);
    Stores getStoreAssignmentCandidates(Store *); 
private:
    Stores stores;
};

#endif // STOREMANAGER_H
