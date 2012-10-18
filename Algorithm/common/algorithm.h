#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>

class Network;

class Algorithm
{
public:

private:
    Algorithm();
public:
    Algorithm(Network * n) {
        network = n;
    }

public:
    virtual void schedule() = 0;
protected:
    Network * network;
};

#endif // ALGORITHM_H
