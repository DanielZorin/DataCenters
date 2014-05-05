#include "prototype.h"

#include "request.h"
#include "element.h"
#include "network.h"
#include "criteria.h"
#include "operation.h"

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

void PrototypeAlgorithm::scheduleRequest(Request * r) {
    Elements unassigned = r->elementsToAssign();
    while ( !unassigned.empty() ) {
        Element * unassignedSeed = getSeedElement(unassigned);
        std::queue<Element *> queue;
        queue.push(unassignedSeed);
        while ( !queue.empty() ) {
            Element * nextToAssign = queue.front();
            queue.pop();
            
            if ( nextToAssign->isAssigned() )
                continue;

        }
    }
}

Element * PrototypeAlgorithm::getSeedElement(Elements & e) {
    std::vector<Element *> tmp(e.begin(), e.end());
    std::sort(tmp.begin(), tmp.end(), Criteria::elementWeightDescending);
    Element * result = tmp[0];
    return result;
} 
