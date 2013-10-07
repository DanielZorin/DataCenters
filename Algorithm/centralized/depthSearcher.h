#ifndef DEPTHSEARCHER_H
#define DEPTHSEARCHER_H

#include "publicdefs.h"

#include <map>
using std::map;

class DepthSearcher
{
public:
    typedef map<Element *, Elements *> AdjacentElements;
    DepthSearcher(Network & network, const Elements & elements);
    ~DepthSearcher();
    void increaseSearchSpace();
    bool isExhausted();
    Elements getElementCandidates();
private:
    Elements getAdjacentElements(Element * element);
    Elements intersect(const Elements & core, const Elements & projection);

    Network & network;
    AdjacentElements adjacentElements;
    AdjacentElements::iterator lastIncreased;
    bool hasBeenModified;
};

#endif // DEPTHSEARCHER_H
