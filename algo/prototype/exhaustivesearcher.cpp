#include "exhaustivesearcher.h"

#include "element.h"
#include "network.h"
#include "criteria.h"
#include "operation.h"
#include "path.h"
#include "link.h"
#include "port.h"
#include "routing/bsearcher.h"

#include <algorithm>
using std::vector;

#include <stdio.h>

ExhaustiveSearcher::ExhaustiveSearcher(Network * n, Element * t, int d, int ma) 
:
    network(n),
    target(t),
    maxAttempts(ma),
    attempt(0),
    depth(d)
{
    Elements cand = Operation::filter(network->getElements(), target, Criteria::isExhaustiveCandidate);
    if ( cand.size() < depth )
        depth = cand.size();

    candidates = vector<Element *>(cand.begin(), cand.end());
    indices = new int[depth + 1];
    for ( int i = 0; i < depth; i++ )
        indices[i] = i;
    indices[depth] = candidates.size();
    printf("Created exhaustive search environment to assign element %p, %d possible candidates\n", t, candidates.size());
}

ExhaustiveSearcher::~ExhaustiveSearcher() {
    printf("Exhaustive search decomposing, attempted %d of %d attempts,", attempt, maxAttempts);
    if ( isExhausted() )
        printf(" is exhausted\n");
    else
        printf(" not exhausted\n");

    delete indices;
}

bool ExhaustiveSearcher::isValid() const {
    if ( !network ) return false;
    if ( !target ) return false;
    if ( !target->isVirtual() ) return false;
    if ( !target->isNode() ) return false;
    return true;
}

bool ExhaustiveSearcher::isExhausted() const {
    return indices[depth - 1] == indices[depth];
}

bool ExhaustiveSearcher::search() {
    if ( !isValid() )
       return false;

    while( !isExhausted() ) {
        attempt++;
        if ( attempt > maxAttempts )
            return false;
        if ( makeAttempt() )
            return true;
    }

    return false;
}

bool ExhaustiveSearcher::makeAttempt() {
    if ( isExhausted() )
        return false;

    Elements cortege = getNextCortege();
    Assignments cache = getAssignmentsCache(cortege);
    Elements assignmentPack = getAssignmentPack(cache);
    Operation::forEach(assignmentPack, Operation::unassign);

    assignmentPack.insert(target);
    if ( performGreedyAssignment(assignmentPack, cortege) ) {
        if ( updatePathes(assignmentPack) )
            return true;
    }

    for(Assignments::iterator i = cache.begin(); i != cache.end(); i++ ) 
        i->second->assign(i->first);

    return false;
}

Elements ExhaustiveSearcher::getNextCortege() {
    Elements result;
    for ( int i = 0; i < depth; i++ )
        result.insert(candidates[indices[i]]);

    advanceCursors();
    return result;
}

void ExhaustiveSearcher::advanceCursors() {
    int border = 0;
    for ( int i = depth - 1; i >= 0; i-- ) {
        if ( indices[i] != indices[i+1] - 1) {
            border = i;
            break;
        }
    }

    indices[border]++;
    for (int i = border + 1; i < depth; i++) {
        indices[i] = indices[i-1] + 1;
    }
}

ExhaustiveSearcher::Assignments ExhaustiveSearcher::getAssignmentsCache(Elements & resources) {
    Assignments result;
    for (Elements::iterator i = resources.begin(); i != resources.end(); i++) {
        Element * resource = *i;
        Elements assignments = resource->getAssignments();
        for(Elements::iterator a = assignments.begin(); a != assignments.end(); a++ ) {
            Element * assignment = *a;
            result[assignment] = resource;
        }
    }
    return result;
}

Elements ExhaustiveSearcher::getAssignmentPack(ExhaustiveSearcher::Assignments & assignments) {
    Elements result;
    for (Assignments::iterator i = assignments.begin(); i != assignments.end(); i++) {
        result.insert(i->first);
    }
    return result;
}

bool ExhaustiveSearcher::performGreedyAssignment(Elements & t, Elements & p) {
    std::vector<Element *> targets(t.begin(), t.end());
    std::vector<Element *> physical(p.begin(), p.end());
    std::sort(targets.begin(), targets.end(), Criteria::elementWeightDescending);
    std::sort(physical.begin(), physical.end(), Criteria::elementWeightDescending);
    for(vector<Element *>::iterator i = targets.begin(); i != targets.end(); i++) {
        Element * element = *i;
        bool result;
        for (vector<Element *>::iterator j = physical.begin(); j != physical.end(); j++) {
            Element * assignee = *j;
            result = assignee->assign(element);
            if ( result )
                break;
        }
        if ( !result ) {
            Operation::forEach(t, Operation::unassign);
            return false;
        }

        std::sort(physical.begin(), physical.end(), Criteria::elementWeightDescending);
    }

    return true;;
}

bool ExhaustiveSearcher::updatePathes(Elements & assignments) {
    std::map<Link *, Path> oldPathes;
    for(Elements::iterator i = assignments.begin(); i != assignments.end(); i++ ) {
        Element * assignment = *i;
        Elements edges = assignment->adjacentEdges();
        for(Elements::iterator e = edges.begin(); e != edges.end(); e++ ) {
            Element * edge = *e;
            Link * tunnel = edge->toLink();
            Path oldPath = tunnel->getRoute();
            oldPathes[tunnel] = oldPath;

            Element * start = tunnel->getFirst()->getParentNode()->getAssignee();
            Element * end = tunnel->getSecond()->getParentNode()->getAssignee();
            BSearcher searcher(start, end, tunnel);
            if ( !searcher.isValid() ) 
                continue;
            if ( !searcher.search() ) {
                for(std::map<Link *, Path>::iterator p = oldPathes.begin();
                        p != oldPathes.end(); p++) 
                    p->first->setRoute(p->second);
                return false;
            }
            Path route = searcher.getPath();
            tunnel->setRoute(route);
        }
    }

    return true;
}
