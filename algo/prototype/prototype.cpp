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
    std::vector<Request *> pRequests = prioritizeRequests(requests);
    for (std::vector<Request *>::iterator i = pRequests.begin();
            i != pRequests.end(); i++) 
    {
        scheduleRequest(*i);   
    }
}

std::vector<Request *> PrototypeAlgorithm::prioritizeRequests(Requests & r) {
    std::vector<Request *> result(r.begin(), r.end());
    std::sort(result.begin(), result.end(), simpleIncreasing);
    return result;
}

bool PrototypeAlgorithm::simpleIncreasing(Request * first, Request * second) {
    return first->getElements().size() > second->getElements().size();
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
            bool result = false;

            if ( router.isValid() ) {
                result = router.search();
            } else {
                result = assignSeedElement(nextToAssign);
            }

            if ( !result ) {
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

bool PrototypeAlgorithm::assignSeedElement(Element * e) {
    Elements candidates = Operation::filter(network->getNodes(), e, Criteria::canHostAssignment);
    if ( candidates.empty() )
        return false;

    Element * candidate = getSeedElement(candidates);
    return candidate->assign(e);
}

Element * PrototypeAlgorithm::getSeedElement(Elements & e) {
    std::vector<Element *> tmp(e.begin(), e.end());
    std::sort(tmp.begin(), tmp.end(), Criteria::elementWeightDescending);
    Element * result = tmp[0];
    return result;
} 
