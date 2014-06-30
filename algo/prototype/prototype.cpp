#include "prototype.h"

#include "preprocessor.h"
#include "request.h"
#include "element.h"
#include "network.h"
#include "criteria.h"
#include "operation.h"
#include "link.h"
#include "routing/bfsrouter.h"
#include "exhaustivesearcher.h"

#include <stdio.h>

#include <vector>
#include <algorithm>
#include <queue>
#include <deque>

void PrototypeAlgorithm::schedule() {
    std::vector<Request *> pRequests = prioritizeRequests(requests);
    for (std::vector<Request *>::iterator i = pRequests.begin();
            i != pRequests.end(); i++) 
    {
        Request * r = *i;
        Request * fakeRequest = Preprocessor::fakeNetElements(r);
        if ( !scheduleRequest(fakeRequest))
            fprintf(stderr, "[ERROR] Failed to assign request %s.\n", r->getName().c_str());
        delete fakeRequest;   
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
    Elements unassignedNodes = Operation::filter(r->elementsToAssign(), Criteria::isComputational);
    while ( !unassignedNodes.empty() ) {
        Element * unassignedSeed = getSeedElement(unassignedNodes);
        std::deque<Element *> queue;
        tweakQueue(queue, r);
        queue.push_back(unassignedSeed);
        while ( !queue.empty() ) {
            Element * nextToAssign = queue.front();
            queue.pop_front();
            
            if ( nextToAssign->isAssigned() )
                continue;

            if ( nextToAssign->isSwitch() && !nextToAssign->isRouter())
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
                queue.push_back(*i);
            }
            unassignedNodes.erase(nextToAssign);
        }
    }

    return true;
}

bool PrototypeAlgorithm::exhaustiveSearch(Element * e) {
    ExhaustiveSearcher searcher(network, e, 3);
    return searcher.search();
}

bool PrototypeAlgorithm::assignSeedElement(Element * e) {
    Elements candidates = Operation::filter(network->getNodes(), e, Criteria::canHostAssignment);
    if ( candidates.empty() )
        return false;

    Element * candidate = getSeedElement(candidates, false);
    return candidate->assign(e);
}

Elements PrototypeAlgorithm::connectedComponent(Element * e) {
    Elements result;

    if ( e->isLink() )
        e = e->toLink()->getFirst()->getParentNode();

    std::queue<Element *> queue;
    queue.push(e);
    while ( !queue.empty() ) {
        Element * node = queue.front();
        queue.pop();
        result.insert(node);

        Elements adjacentNodes = node->adjacentNodes();
        for ( Elements::iterator i = adjacentNodes.begin(); i != adjacentNodes.end(); i++ ) {
            Element * a = *i;
            if ( !Operation::isIn(a, result) ) {
                queue.push(a);
            }
        }
    }

    return result;
}

void PrototypeAlgorithm::tweakQueue(std::deque<Element *> & queue, Request * r) {
    Elements links = r->getTunnels(); 
    for (Elements::iterator i = links.begin(); i != links.end(); i++) {
        Link * link = (*i)->toLink();
        if ( link->isAffine() ) {
            Element * first = link->getFirst()->getParentNode();
            Element * second = link->getSecond()->getParentNode();
            Element * vm, * store;
            if ( first->isComputer() ) {
                vm = first;
                store = second;
            } else {
                store = first;
                vm = second;
            }
            queue.push_back(store);
            queue.push_back(vm);
        }
    }
}

Element * PrototypeAlgorithm::getSeedElement(Elements & e, bool isVirtual) {
    std::vector<Element *> tmp;
    Elements stores = Operation::filter(e, Criteria::isStore);
    if ( !stores.empty() ) {
        tmp = std::vector<Element *>(stores.begin(), stores.end());
    } else {
        tmp = std::vector<Element *>(e.begin(), e.end());
    }

    if ( isVirtual ) {
        std::sort(tmp.begin(), tmp.end(), Criteria::elementWeightDescending);
    } else {
        std::sort(tmp.begin(), tmp.end(), Criteria::elementWeightAscending);
    }
    Element * result = tmp[0];
    return result;
} 
