#pragma once

#include "algorithm.h"

class PrototypeAlgorithm : public Algorithm {
public:
    PrototypeAlgorithm(Network * n, const Requests & r)
    : Algorithm(n, r) {}

    virtual void schedule();
private:
    void prioritizeRequests(Requests & r);
    void scheduleRequest(Request * r);

    Element * getSeedElement(Elements & e);
};
