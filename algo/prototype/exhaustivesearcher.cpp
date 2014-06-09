#include "exhaustivesearcher.h"

#include "element.h"
#include "network.h"
#include "criteria.h"
#include "operation.h"

using std::vector;

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
}

ExhaustiveSearcher::~ExhaustiveSearcher() {
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

bool ExhaustiveSearcher::makeAttempt() {
    if ( isExhausted() )
        return false;

    Elements cortege = getNextCortege();
    return true;
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

