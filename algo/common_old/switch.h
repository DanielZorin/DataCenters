#ifndef SWITCH_H
#define SWITCH_H

#include "networkingElement.h"

class Switch : public NetworkingElement
{
public:
    Switch(string name = "unnamed_switch", unsigned long capacity = 0, unsigned long max = 0)
        : NetworkingElement(name, capacity, max)
    {
        setType(Element::SWITCH);
    }

};

#endif // SWITCH_H
