#pragma once

#include "defs.h"
#include "path.h"

#include <map>
#include <queue>

class BFSQueue {
public:
    BFSQueue(Element * start, Element * mask);
    inline bool isExhausted() const { return unvisited.empty(); }
    Element * getNextCandidate();
    Path getPath(Element * target) const; 
private:
    Element * processNextItem();
private:
    Element * start;
    Element * mask;
    std::queue<Element *> unvisited;
    std::map<Element *, Element *> ancestors;
};
