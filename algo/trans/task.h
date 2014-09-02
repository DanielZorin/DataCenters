#pragma once

#include "defs.h"

class Task {
public:
    Task(Element * target, Element * destination); 
    bool isValid() const;
    bool isComplete() const;
private:
    Element * target;
    Element * destination;
};
