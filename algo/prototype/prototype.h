#pragma once

#include "algorithm.h"

class PrototypeAlgorithm : public Algorithm {
public:
    PrototypeAlgorithm(Network * n, const Requests & r)
    : Algorithm(n, r) {}

    virtual void schedule();
private:
    void prioritizeRequests(Requests & r);
    bool scheduleRequest(Request * r);
    bool exhaustiveSearch(Element * e);

    Element * getSeedElement(Elements & e);
};
