#include "link.h"

void Link::bindElements(Element * a, Element * b) 
{
    if ( a->isLink() || b->isLink() )
        return;

    first = a;
    second = b;
}
