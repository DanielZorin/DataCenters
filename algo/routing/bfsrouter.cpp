#include "bfsrouter.h"

#include "operation.h"
#include "request.h"
#include "criteria.h"

BFSRouter::BFSRouter(Request & r, Element * t)
:
    target(t)    
{
    if ( !Operation::isIn(target, r.getNodes())) throw;

    Elements tunnels = r.getTunnels();
    Elements adjacentTunnels = Operation::filter(tunnels, target, Criteria::isAdjacent);

    for(Elements::iterator i = adjacentTunnels.begin(); i != adjacentTunnels.end(); i++) {
        Element * tunnel = *i;
        Element * adjacent = tunnel->toEdge()->getAdjacent(target);
        if ( !adjacent->isAssigned() )
            continue;
        
        candidates[adjacent] = new Elements();
        searchers[adjacent] = new BFSQueue(adjacent, tunnel);
    }
}

BFSRouter::~BFSRouter() {
    for(Candidates::iterator i = candidates.begin(); i != candidates.end(); i++)
        delete i->second;
    for(Searchers::iterator i = searchers.begin(); i != searchers.end(); i++)
        delete i->second;
}

bool BFSRouter::isExhausted() const {
    for(Searchers::const_iterator i = searchers.begin(); i != searchers.end(); i++) {
        BFSQueue * searcher = i->second;
        if ( !searcher->isExhausted() )
            return false;
    }

    return true;
}

bool BFSRouter::search() {
    Searchers::iterator i = findNextNonExhausted(searchers.end());
    
    while( i != searchers.end() ) {
        Element * start = i->first;
        BFSQueue * queue = i->second;
        Element * candidate = queue->getNextCandidate();

        while ( candidate != 0  ) {
            if ( candidate->canHostAssignment(target) )
                break;
            candidate = queue->getNextCandidate();
        }

        if ( candidate != 0 ) {
            Elements * c = candidates.at(start);
            c->insert(candidate);
            Elements intersection = intersectCandidates();
            if ( !intersection.empty() )
                if ( commit(intersection) )
                    return true;
                else
                    discard(intersection);
        }

        i = findNextNonExhausted(i);
    } 

    return false;
}

Elements BFSRouter::intersectCandidates() {
    Elements result = *(candidates.begin()->second);
    for ( Candidates::iterator i = candidates.begin(); i != candidates.end(); i++) {
        Elements * c = i->second;
        result = Operation::intersect(result, *c);
    }
    return result;
}

bool BFSRouter::commit(Elements & c) {
    return false;
}

void BFSRouter::discard(Elements & d) {
    for ( Candidates::iterator i = candidates.begin(); i != candidates.end(); i ++) {
        Elements * c = i->second;
        Elements newElements = Operation::minus(*c, d);
        *c = newElements;
    }

}
