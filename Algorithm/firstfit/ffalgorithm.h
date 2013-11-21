#ifndef FFALGORITHM_H
#define FFALGORITHM_H

#include "common/publicdefs.h"
#include "common/algorithm.h"

class FirstFitAlgorithm : public Algorithm
{
private:
    FirstFitAlgorithm();
public:
    FirstFitAlgorithm(Network * n, Requests const & r);

    virtual ~FirstFitAlgorithm() {}
    virtual Algorithm::Result schedule();
private:
    Link * createDummyLink(Link * virtualLink, Assignment * assignment);
    Element * getCastedAssignment(Element * element, Assignment * assignment);
};

#endif // ALGORITHM_H
