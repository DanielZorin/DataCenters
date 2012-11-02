#ifndef STORE_H
#define STORE_H

#include "computationalElement.h"

class Store : public ComputationalElement
{
public:
    Store(string name = "unnamed_store", unsigned long capacity = 0, unsigned long max = 0)
        : ComputationalElement(name, capacity, max)
    {
        setType(Element::STORE);
    }
};

#endif // STORE_H
