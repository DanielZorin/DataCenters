#include "prototype.h"

#include "request.h"
#include "element.h"
#include "network.h"
#include "criteria.h"
#include "operation.h"
#include "routing/bfsrouter.h"

#include <vector>
#include <algorithm>
#include <queue>

void PrototypeAlgorithm::schedule() {
    prioritizeRequests(requests);
    for (Requests::iterator i = requests.begin();
            i != requests.end(); i++) 
    {
        scheduleRequest(*i);   
    }
}

void PrototypeAlgorithm::prioritizeRequests(Requests & r) {
    
}

bool PrototypeAlgorithm::scheduleRequest(Request * r) {
    Elements unassignedNodes = Operation::filter(r->elementsToAssign(), Criteria::isNode);
    while ( !unassignedNodes.empty() ) {
        Element * unassignedSeed = getSeedElement(unassignedNodes);
        std::queue<Element *> queue;
        queue.push(unassignedSeed);
        while ( !queue.empty() ) {
            Element * nextToAssign = queue.front();
            queue.pop();
            
            if ( nextToAssign->isAssigned() )
                continue;

            BFSRouter router(*r, nextToAssign);
            if ( !router.search() ) {
                if ( !exhaustiveSearch(nextToAssign) ) {
                    r->purgeAssignments();
                    return false;
                }
            }

            Elements adjacentNodes = nextToAssign->adjacentNodes();
            for ( Elements::iterator i = adjacentNodes.begin(); i != adjacentNodes.end(); i++) {
                queue.push(*i);
            }
            unassignedNodes.erase(nextToAssign);
        }
    }

    return true;
}

bool PrototypeAlgorithm::exhaustiveSearch(Element * e) {
    return false;
}

Element * PrototypeAlgorithm::getSeedElement(Elements & e) {
    std::vector<Element *> tmp(e.begin(), e.end());
    std::sort(tmp.begin(), tmp.end(), Criteria::elementWeightDescending);
    Element * result = tmp[0];
    return result;
} 
