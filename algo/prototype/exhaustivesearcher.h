#pragma once

#include <defs.h>
#include <vector>
#include <map>

class ExhaustiveSearcher {
public:
    typedef std::map<Element *, Element *> Assignments; 
    ExhaustiveSearcher(Network * network, Element * target, int depth = 3, int maxAttempts = 1000);
    ~ExhaustiveSearcher();


    bool isValid() const;
    bool isExhausted() const;
    bool search();
private:
    bool makeAttempt();
    Elements getNextCortege();
    void advanceCursors();
    Assignments getAssignmentsCache(Elements &);
    Elements getAssignmentPack(Assignments &);

    bool performGreedyAssignment(Elements & targets, Elements & physical);
    bool updatePathes(Elements & assignments);
private:
    Network * network;
    Element * target;
    int maxAttempts;
    int attempt;
    int depth;

    std::vector<Element *> candidates;
    int* indices;
};
