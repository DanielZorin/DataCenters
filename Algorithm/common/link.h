#ifndef LINK_H
#define LINK_H

#include "networkingElement.h"

class Link : public NetworkingElement
{
public:
    Link(string name = "unnamed_link", unsigned long capacity = 0, unsigned long max = 0)
        : NetworkingElement(name, capacity, max)
    {
        setType(Element::LINK);
    }

    void bindElements(Element * a, Element * b);
private:
    Element * first;
    Element * second;


};

#endif // LINK_H
