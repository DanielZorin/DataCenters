#ifndef CENTRALIZEDALGORITHM_H
#define CENTRALIZEDALGORITHM_H

#include "algorithm.h"

#include <vector>

class CentralizedAlgorithm : public Algorithm
{
private:
    CentralizedAlgorithm();
public:
    CentralizedAlgorithm(Network * n, Requests const & r)
        : Algorithm(n, r)
    {}
private:
    std::vector<Request *> prioritizeRequests();

    Result buildVMAssignment();
    Result buildStorageAssignment();
public:
    virtual Algorithm::Result schedule();
private:
    Assignment * currentAssignment;
};

#endif // CENTRALIZEDALGORITHM_H
