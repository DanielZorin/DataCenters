#ifndef STORE_H
#define STORE_H

#include "computationalElement.h"

class Store : public ComputationalElement
{
    Store(string name = "unnamed_store", unsigned long capacity = 0)
        : ComputationalElement(name, capacity)
    {
        setType(Element::STORE);
    }
};

#endif // STORE_H
