#include "criteria.h"
#include "leafnode.h"

bool Criteria::isServerLayered(const Element * e) {
    if ( e->isPhysical() || !e->isComputational() )
        return false;

    LeafNode * l = (LeafNode *)e;
    if ( l->sl() == 0 )
        return false;

    return true;
}
