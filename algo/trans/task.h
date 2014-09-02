#pragma once

#include "defs.h"
#include <list>

class Task {
public:
    Task(Element * target, Element * destination); 
    bool isValid() const;
    bool isComplete() const;
    bool move(Element * newDestinaton);
private:
    Element * target;
    Element * destination;
    std::list<Element *> history;
};
