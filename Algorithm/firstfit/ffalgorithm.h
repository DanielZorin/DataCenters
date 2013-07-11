#ifndef FFALGORITHM_H
#define FFALGORITHM_H

#include "publicdefs.h"
#include "algorithm.h"

class FirstFitAlgorithm : public Algorithm
{
private:
    FirstFitAlgorithm();
public:
    FirstFitAlgorithm(Network * n, Requests const & r);

    virtual ~FirstFitAlgorithm() {}
    virtual Algorithm::Result schedule();
};

#endif // ALGORITHM_H
