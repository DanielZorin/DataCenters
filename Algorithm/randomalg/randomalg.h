#ifndef RANDOMALG_H
#define RANDOMALG_H

#include "../common/algorithm.h"
#include "../common/element.h"
#include "../common/network.h"
#include "../common/request.h"
#include "../common/publicdefs.h"
#include "../common/node.h"
#include "../common/store.h"
#include "../common/assignment.h"
#include "../common/link.h"
#include <vector>

struct SequenceElement
{
    Element * request;
    Element * resource;

    SequenceElement(Element * req, Element * res)
    : request(req)
    , resource(res)
    {
    }

    SequenceElement()
    : request(NULL)
    , resource(NULL)
    {
    }
};

// algorithm that assign requests into a node randomly chosen from a set of fitting nodes
// links are assigned randomly as well
class RandomAlgorithm: public Algorithm
{
    public:
    RandomAlgorithm(Network * n, Requests const & r, unsigned long tr = 50);
    ~RandomAlgorithm();

    virtual Algorithm::Result schedule();

    private:
    Element * findResource(Element * request, std::vector<SequenceElement *> seq, unsigned int start);
    // copy network to restore it between iterations
    Network * copyNetwork;
    // number of tries
    unsigned long tries;
    // current best sequence of elements, a list of assigned requests and a map with assigned channels
    std::vector<SequenceElement *> bestSequence;
    std::vector<Request *> bestReq;
    std::map<Link *, NetPath> bestChan;
    // amount of vms and storages
    unsigned int vmCount, stCount;
};

#endif
