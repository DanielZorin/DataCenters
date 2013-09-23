#ifndef STOREMANAGER_H
#define STOREMANAGER_H

#include "publicdefs.h"

class StoreManager
{
public:
    StoreManager(Stores &);
    std::vector<Store *> getStoreAssignmentCandidates(Store *); 
private:
    Stores stores;
};

#endif // STOREMANAGER_H
