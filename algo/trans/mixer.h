#pragma once

#include "defs.h"

class Snapshot;

class Mixer {
public:
    Mixer();
    bool read(const Snapshot & from, const Snapshot & to);
    void print() const;
    
};
