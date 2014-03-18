#include "bfsqueue.h"

#include "element.h"

BFSQueue::BFSQueue(Element * s, Element * t) 
: 
    start(s)
{
    if ( !t->isLink() ) throw;
    tunnel = t;

    unvisited.push(s);
    ancestors[s] = 0;
}

Element * BFSQueue::processNextItem() {
    if ( isExhausted() ) return 0;

    Element * next = unvisited.front();
    unvisited.pop();

    Elements adjacent = next->adjacent();
    for( Elements::iterator i = adjacent.begin(); i != adjacent.end(); i++) {
        Element * a = *i;
        if ( ancestors.find(a) != ancestors.end() )
            continue;

        if ( a->isNetwork() )
            if ( !a->canHostAssignment(tunnel) )
                continue;

        unvisited.push(a);
        ancestors[a] = next;
    }

    return next;
}

Element * BFSQueue::getNextCandidate() {
    while ( !isExhausted() ) {
        Element * processed = processNextItem();
        if ( processed->isComputational() ) {
            return processed;
        }
    }

    return 0;
}

Path BFSQueue::getPath(Element * target) const {
    if ( ancestors.find(target) == ancestors.end() )
        return Path();

    Path result(target, start);
    Element * next = target;
    while ( next != start ) {
        Element * ancestor = ancestors.at(next);
        if ( !result.addElement(ancestor) ) throw;
        next = ancestor;
    }

    result.revert();
    return result;
}
