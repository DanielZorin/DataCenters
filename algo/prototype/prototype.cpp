#include "prototype.h"

#include "preprocessor.h"
#include "request.h"
#include "element.h"
#include "network.h"
#include "criteria.h"
#include "operation.h"
#include "link.h"
#include "leafnode.h"
#include "routing/bfsrouter.h"
#include "exhaustivesearcher.h"
#include "dcoverseer.h"

#include <stdio.h>

#include <vector>
#include <algorithm>
#include <queue>
#include <deque>
#include <map>

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
    std::vector<Request *> providers;
    std::vector<Request *> others;
    for(Requests::iterator i = r.begin(); i != r.end(); i++ ) { 
        if ( (*i)->isProvider() )
            providers.push_back(*i);
        else
            others.push_back(*i);
    }
    std::sort(providers.begin(), providers.end(), simpleIncreasing);
    std::sort(others.begin(), others.end(), simpleIncreasing);
    std::vector<Request *> result(providers);
    result.insert(result.end(), others.begin(), others.end());
    return result;
}

bool PrototypeAlgorithm::simpleIncreasing(Request * first, Request * second) {
    return first->getElements().size() > second->getElements().size();
}  


bool PrototypeAlgorithm::scheduleRequest(Request * r) {
    if ( r->isDCAffined() ) {
        if ( !dlRequestAssignment(r) ) {
            fprintf(stderr, "[ERROR] tenant affinity requirement failed\n");
            return false;
        }

    }

    Elements serverLayered = Operation::filter(r->elementsToAssign(), Criteria::isServerLayered);
    Elements pool = network->getNodes();
    if ( !slAssignment(serverLayered, pool, r) ) {
        fprintf(stderr, "[ERROR] server layer requirement failed\n");
        return false;
    }


    Elements unassignedNodes = Operation::filter(r->elementsToAssign(), Criteria::isComputational);
    return routedAssignment(unassignedNodes, pool, r);
}

bool PrototypeAlgorithm::dlRequestAssignment(Request * r) {
    Elements elements = r->elementsToAssign();
    DCOverseer overseer(network); 

    for( int i = 0; i < overseer.dcCount(); i++) {
        Elements pool = overseer.dcPositionPool(i);
        if ( slAssignment(elements, pool, r) ) 
            return true;

        Operation::forEach(elements, Operation::unassign);

        if ( routedAssignment(elements, pool, r )) 
            return true;

        Operation::forEach(elements, Operation::unassign);

    }

    return false;

}

bool PrototypeAlgorithm::routedAssignment(Elements & n, Elements & pool, Request * r)
{
    Elements nodes = n;
    while ( !nodes.empty() ) {
        Element * unassignedSeed = getSeedElement(nodes);
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
                result = assignSeedElement(nextToAssign, pool);
            }

            if ( !result ) {
                if ( !exhaustiveSearch(nextToAssign, pool) ) {
                    return false;
                }
            }

            Elements adjacentNodes = nextToAssign->adjacentNodes();
            for ( Elements::iterator i = adjacentNodes.begin(); i != adjacentNodes.end(); i++) {
                queue.push_back(*i);
            }
            nodes.erase(nextToAssign);
        }
    }

    return true;

}

bool PrototypeAlgorithm::slAssignment(Elements & nodes, Elements & pool, Request * r) {
    if ( nodes.empty() )
        return true;
    
    using std::map;
    map<int, Elements> layeredModel;
    for (Elements::iterator i = nodes.begin(); i != nodes.end(); i++) {
        LeafNode * l = (LeafNode *)(*i);
        layeredModel[l->sl()].insert(l);
    }
    
    for(map<int, Elements>::iterator i = layeredModel.begin(); i != layeredModel.end(); i++) {
        Elements & elements = i->second;
        Elements globalCandidates = Operation::filter(pool, *elements.begin(), Criteria::canHostAssignment);
        for (Elements::iterator c = globalCandidates.begin(); c != globalCandidates.end(); c++ ) {
            Elements localAssigned;
            for ( Elements::iterator a = (*c)->getAssignments().begin(); a != (*c)->getAssignments().end(); a++ ) {
                if ( Operation::isIn(*a, r->getElements()) )
                    localAssigned.insert(*a);
            }

            bool layerConflict = false;
            for ( Elements::iterator a = localAssigned.begin(); a != localAssigned.end(); a++ ) {
                if ( !Criteria::isServerLayered(*a) )
                    continue;

                LeafNode * l = (LeafNode *)(*a);
                if ( l->sl() | i->first )
                    layerConflict = true;
            }
            if ( layerConflict )
                continue;

            bool result = true;
            for ( Elements::iterator e = elements.begin(); e != elements.end(); e++) {
                result &= (*c)->assign(*e);    
            }

            if ( result )
                break;

            Operation::forEach(elements, Operation::unassign);
        }
        if ( !Operation::filter(elements, Criteria::isUnassigned).empty() )
            return false;
    }

    return false; 
}

bool PrototypeAlgorithm::dlAssignment(Elements & nodes, Elements & pool, Request * r) {
    return false;
}

bool PrototypeAlgorithm::exhaustiveSearch(Element * e, Elements & pool) {
    ExhaustiveSearcher searcher(pool, e, 3);
    return searcher.search();
}

bool PrototypeAlgorithm::assignSeedElement(Element * e, Elements & pool) {
    Elements candidates = Operation::filter(pool, e, Criteria::canHostAssignment);
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
