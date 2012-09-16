#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "network.h"

#include <vector>

class Algorithm
{
public:
    enum Mode {
        Simple,
        Replanning
    };

private:
    Algorithm();
public:
    Algorithm(Network& n) {
        network = n;
    }

    void setMode(Mode m) { mode = m; }
    Mode getMode() { return mode; }
public:
    virtual void schedule() = 0;
protected:
    Network network;
    Mode mode;
};

#endif // ALGORITHM_H
