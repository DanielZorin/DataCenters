#include "bsearcher.h"

#include "element.h"
#include "edge.h"

BSearcher::BSearcher(Element * s, Element * e, Element * t)
:
    start(s),
    end(s),
    tunnel(t)
{
    if ( !start->isPhysical() ) throw;
    if ( !end->isPhysical() ) throw;
    if ( tunnel != 0 && !tunnel->isVirtual() && !tunnel->isLink() ) throw;
}

bool BSearcher::search() {
    unvisited.push(start);
    ancestors[start] = 0;

    while ( !unvisited.empty() ) {
        Element * next = unvisited.front();
        unvisited.pop();

        if ( next == end )
            return true;

        Elements adjacentNodes = next->adjacentNodes();
        for ( Elements::iterator i = adjacentNodes.begin(); i != adjacentNodes.end(); i++) {
            Element * node = *i;
            if ( ancestors.count(node) != 0 )
                continue;

            unvisited.push(node);
            ancestors[node] = next;
        }
    }

    return false;
}

Path BSearcher::getPath() const {
    if ( ancestors.count(end) == 0 )
        return Path();

    Path result(end, start);
    Element * next = end;
    while ( next != start ) {
        Element * ancestor = ancestors.at(next);
        Elements ancestorEdges = ancestor->adjacentEdges();
        Elements::iterator i;
        for ( i = ancestorEdges.begin(); i != ancestorEdges.end(); i++) {
            Edge * edge = (*i)->toEdge();
            if ( edge->connects(next) ) {
                result.addElement(edge);
                break;
            }
        }
        if ( ancestor != start )
            result.addElement(ancestor);
        next = ancestor;
    }

    result.revert();
    return result;
}
