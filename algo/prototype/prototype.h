#pragma once

#include "algorithm.h"

#include <vector>
#include <deque>

class PrototypeAlgorithm : public Algorithm {
public:
    PrototypeAlgorithm(Network * n, const Requests & r)
    : Algorithm(n, r) {}

    virtual void schedule();
private:
    std::vector<Request *> prioritizeRequests(Requests & r);
    bool scheduleRequest(Request * r);
    bool exhaustiveSearch(Element * e);
    bool assignSeedElement(Element * e);
    Elements connectedComponent(Element * e);
    void tweakQueue(std::deque<Element *> & queue, Request * r);

    static bool simpleIncreasing(Request * first, Request * second);

    Element * getSeedElement(Elements & e, bool isVirtual = true);
};
