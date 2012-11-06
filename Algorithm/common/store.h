#ifndef STORE_H
#define STORE_H

#include "computationalElement.h"

class Store : public ComputationalElement
{
public:
    Store(string name = "unnamed_store", unsigned long capacity = 0, unsigned long max = 0, unsigned typeOfStore = 0)
        : ComputationalElement(name, capacity, max),
        typeOfStore(typeOfStore)
    {
        setType(Element::STORE);
    }

    unsigned getTypeOfStore()
    {
        return typeOfStore;
    }

private:
    // Type of store/storage
    unsigned typeOfStore;
};

#endif // STORE_H
