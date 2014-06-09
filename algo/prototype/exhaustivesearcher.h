#pragma once

#include <defs.h>
#include <vector>

class ExhaustiveSearcher {
public:
    ExhaustiveSearcher(Network * network, Element * target, int depth = 2, int maxAttempts = 1000);
    ~ExhaustiveSearcher();


    bool isValid() const;
    bool isExhausted() const;
    bool makeAttempt();
private:
    Elements getNextCortege();
    void advanceCursors();
private:
    Network * network;
    Element * target;
    int maxAttempts;
    int attempt;
    int depth;

    std::vector<Element *> candidates;
    int* indices;
};
