#ifndef STORE_H
#define STORE_H

#include "computationalElement.h"

class Store : public ComputationalElement
{
public:
    Store(string name = "unnamed_store", unsigned long capacity = 0, unsigned long max = 0,
        unsigned typeOfStore = 0, unsigned replicationCapacity = 0)
        : ComputationalElement(name, capacity, max),
        typeOfStore(typeOfStore),
        replicationCapacity(replicationCapacity)
    {
        setType(Element::STORE);
    }

    unsigned getTypeOfStore()
    {
        return typeOfStore;
    }

    // Replication capacity, which is used for storages only.
    //
    unsigned getReplicationCapacity()
    {
        return replicationCapacity;
    }

    void setTypeOfStore(unsigned type)
    {
        typeOfStore = type;
    }
private:
    // Type of store/storage
    unsigned typeOfStore;

    // Replication capacity
    unsigned replicationCapacity;
};

#endif // STORE_H
