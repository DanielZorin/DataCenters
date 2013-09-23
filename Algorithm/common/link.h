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

    Element * getFirst() const
    {
        return first;
    }

    Element * getSecond() const
    {
        return second;
    }

    Element * getLinkedComputationalElement()
    {
        if ( first->isComputational() )
            return first;
        if ( second->isComputational() )
            return second;
    }

    bool connectsElement(const Element * e)
    {
        return ( e == first) || ( e == second );
    }

    Element * getAdjacentElement(const Element * e)
    {
        if ( e == first ) return second;
        if ( e == second ) return first;
        return 0; 
    }

private:
    Element * first;
    Element * second;


};

#endif // LINK_H
