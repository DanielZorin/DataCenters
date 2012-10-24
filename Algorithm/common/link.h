#ifndef LINK_H
#define LINK_H

#include "networkingElement.h"

class Link : public NetworkingElement
{
public:
    Link(string name = "unnamed_link", unsigned long capacity = 0)
        : NetworkingElement(name, capacity)
    {
        setType(Element::LINK);
    }

};

#endif // LINK_H
