#ifndef CENTRALIZEDALGORITHM_H
#define CENTRALIZEDALGORITHM_H

#include "algorithm.h"

class CentralizedAlgorithm : public Algorithm
{
private:
    CentralizedAlgorithm();
public:
    CentralizedAlgorithm(Network * n, Requests const & r)
        : Algorithm(n, r)
    {}
public:
    virtual Algorithm::ResultEnum::Result schedule();
};

#endif // CENTRALIZEDALGORITHM_H
